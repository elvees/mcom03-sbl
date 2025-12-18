// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>

#include <drivers/hs-periph/hs-periph.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/service/service.h>
#include <drivers/top/top.h>
#include <drivers/ucg/ucg.h>
#include <libs/console/console.h>
#include <libs/errors.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "platform-def.h"

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

#ifdef UART_ENABLE
#include <drivers/uart/uart.h>
#endif

#if defined(BOOTSTAGE_ENABLE)
#include <libs/bootstage/bootstage.h>
#endif

#if defined(BOOTSTAGE_ENABLE)
extern uintptr_t __bs_start;
extern uintptr_t __bs_end;

static const uintptr_t bs_start = (uintptr_t)&__bs_start;
static const uintptr_t bs_end = (uintptr_t)&__bs_end;
#endif

int main(void)
{
	int ret;

	// Register LSP1 TIMERS as system timer
	ret = lsp1_timer_register(true, 0, false);
	if (ret)
		return ret;

#if defined(BOOTSTAGE_ENABLE)
	bootstage_init(BOOTSTAGE_ID_SBL_S1_START);
#endif

	// Initialize and configure the TOP clock gate
	uint32_t top_clkgate = BIT(SERVICE_TOP_CLK_GATE_SERVICE) | BIT(SERVICE_TOP_CLK_GATE_LSP0) |
	                       BIT(SERVICE_TOP_CLK_GATE_LSP1) | BIT(SERVICE_TOP_CLK_GATE_HSP) |
	                       BIT(SERVICE_TOP_CLK_GATE_DDR) |
	                       BIT(SERVICE_TOP_CLK_GATE_TOP_INTERCONNECT);

	service_urb_regs_t *serv_urb = service_get_urb_registers();
	serv_urb->top_clkgate = top_clkgate;

#ifdef UART_ENABLE
	uart_param_t uart;

	console_ops_t uart_console_ops = { .init = uart_drv_config,
		                           .putchar = uart_drv_putchar,
		                           .getchar = uart_drv_getchar,
		                           .flush = uart_drv_flush,
		                           .deinit = uart_drv_deinit };

	console_t console = { .hw = &uart, .ops = &uart_console_ops };

	// Initialize and configure the UART0
	ret = uart_drv_config_default(&uart);
	if (ret)
		return ret;

	ret = uart0_hw_enable(&uart);
	if (ret)
		return ret;

	ret = console_register(&console);
	if (ret)
		return ret;
#endif

	ret = console_init();
	if (ret)
		return ret;

	printf(PREFIX " " BUILD_VERSION_STR "\n");

	// Initialize and configure the RISC0 clocking system
	uint32_t ch_mask =
		BIT(SERVICE_UCG1_CHANNEL_CLK_APB) | BIT(SERVICE_UCG1_CHANNEL_CLK_CORE) |
		BIT(SERVICE_UCG1_CHANNEL_CLK_QSPI0) | BIT(SERVICE_UCG1_CHANNEL_CLK_RISC0) |
		BIT(SERVICE_UCG1_CHANNEL_CLK_BPAM) | BIT(SERVICE_UCG1_CHANNEL_CLK_MFBSP0) |
		BIT(SERVICE_UCG1_CHANNEL_CLK_MFBSP1) | BIT(SERVICE_UCG1_CHANNEL_CLK_MAILBOX0) |
		BIT(SERVICE_UCG1_CHANNEL_CLK_PVTCTR) | BIT(SERVICE_UCG1_CHANNEL_CLK_I2C4) |
		BIT(SERVICE_UCG1_CHANNEL_CLK_TRNG) | BIT(SERVICE_UCG1_CHANNEL_CLK_QSPI0_EXT);

	ret = service_set_clock(ch_mask, ch_mask);
	if (ret)
		return ret;

#ifdef WDT_ENABLE
	wdt_dev_t *wdt = wdt_get_instance();

	// Initialize and configure the watchdog
	ret = wdt_config_default(wdt);
	if (ret)
		return ret;

	ret = wdt0_hw_enable(wdt);
	if (ret)
		return ret;

	ret = wdt_start(wdt);
	if (ret && (ret != -EALREADYINITIALIZED))
		return ret;

	if (ret == -EALREADYINITIALIZED)
		printf("WDT0 is already enabled\n");
	else
		printf("WDT0 is initialized successfully\n");
#endif

	// Initialize and configure the top clocking system
	ret = top_set_clock();
	if (ret)
		return ret;

	/**
	 * Set PLL as clock source for all HSPERIPH UCGs
	 * Some HSperiph UCGs use CLK125 external pad as default source
	 * (this is considered as chip bug).
	 * Some boards might not have CLK125 routed so it's not possible
	 * to read/write UCG registers (UCG configuration hangs).
	 * This is a temporary solution.
	 * All clocks should be configured by clock driver.
	 */
	hsp_refclk_setup();

	// TODO: May be necessary to block writing to the flash memory

#ifdef WDT_ENABLE
	wdt_reset(wdt);
#endif

#if defined(BOOTSTAGE_ENABLE)
	bootstage_export((void *)bs_start, bs_end - bs_start);
#endif

	return 0;
}
