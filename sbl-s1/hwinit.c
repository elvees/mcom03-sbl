// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>

#include <drivers/hs-periph/hs-periph.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <drivers/top/top.h>
#include <libs/console/console.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

#ifdef UART_ENABLE
#include <drivers/uart/uart-config.h>
#include <drivers/uart/uart.h>
#endif

#define PREFIX "SBL-S1"

int main(void)
{
	int ret;

	// Initialize and configure the TOP clock gate
	uint32_t top_clkgate = (SERVICE_TOP_CLK_GATE_SERVICE) | (SERVICE_TOP_CLK_GATE_LSP0) |
	                       (SERVICE_TOP_CLK_GATE_LSP1) | (SERVICE_TOP_CLK_GATE_HSP) |
	                       (SERVICE_TOP_CLK_GATE_DDR) | (SERVICE_TOP_CLK_GATE_TOP_INTERCONNECT);

	service_urb_regs_t *serv_urb = service_get_urb_registers();
	serv_urb->top_clkgate = top_clkgate;

#ifdef UART_ENABLE
	uart_param_t uart = { .uartNum = UART0 };

	console_ops_t uart_console_ops = { .init = uart_drv_config_default,
		                           .putchar = uart_drv_putchar,
		                           .getchar = uart_drv_getchar,
		                           .flush = uart_drv_flush,
		                           .deinit = uart_drv_deinit };

	console_t console = { .hw = &uart, .ops = &uart_console_ops };

	// Initialize LS Peripheral 1 for UART 0
	ret = uart_hw_enable();
	if (ret)
		return ret;

	ret = console_register(&console);
	if (ret)
		return ret;

#endif

	ret = console_init();
	if (ret)
		return ret;

	printf(PREFIX " (" __DATE__ " - " __TIME__ "): " COMMIT "\n");
#ifdef BUILD_ID
	printf(PREFIX ": Build: %s\n", BUILD_ID);
#endif

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
	wdt_dev_t *wdt0 = wdt_get_instance();

	// Initialize and configure the watchdog
	ret = wdt_set_config(wdt0, WDT_MAX_TIMEOUT);
	if (ret)
		return ret;

	ret = wdt_init(wdt0);
	if (ret && (ret != -EALREADYINITIALIZED))
		return ret;

#ifdef UART_ENABLE
	if (ret == -EALREADYINITIALIZED)
		printf("WDT0 is already enabled\n");
#endif

	ret = wdt_start(wdt0);
	if (ret)
		return ret;

#endif

	// Initialize and configure the top clocking system
	ret = top_set_clock();
	if (ret)
		return ret;

	// Disabling the debugging subsystem
	ret = soc_debug_if_disable();
	if (ret)
		return ret;

	/* Set PLL as clock source for all HSPERIPH UCGs
	 * Some HSperiph UCGs use CLK125 external pad as default source
	 * (this is considered as chip bug).
	 * Some boards might not have CLK125 routed so it's not possible
	 * to read/write UCG registers (UCG configuration hangs).
	 * This is a temporary solution.
	 * All clocks should be configured by clock driver.
	 */
	hsp_refclk_setup();

	// I2S RSTN must be enabled before LSP1 UCGs setup
	lsp1_i2s_ucg1_rstn();

	// TODO: May be necessary to block writing to the flash memory

#ifdef WDT_ENABLE
	// Reconfigure the watchdog
	ret = wdt_set_config(wdt0, 3000);
	if (ret)
		return ret;

	ret = wdt_start(wdt0);
	if (ret)
		return ret;
#endif

	return 0;
}
