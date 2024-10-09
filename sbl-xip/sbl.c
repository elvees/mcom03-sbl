// Copyright 2020-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <drivers/cpu/cpu.h>
#include <drivers/hs-periph/hs-periph.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <drivers/top/top.h>
#include <libs/console/console.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

#ifdef UART_ENABLE
#include <drivers/uart/uart-config.h>
#include <drivers/uart/uart.h>
#endif

#define PREFIX "SBL-XIP"

#define DDRINIT_START_ADDR_VIRT 0xA0000000
#define TFA_START_ADDR_VIRT     0xC0300000
#define TFA_START_ADDR_PHYS     0x880300000
#define BL32_START_ADDR_VIRT    0xC1380000
#define UBOOT_START_ADDR_VIRT   0xC0080000
#define UBOOT_DTB_ADDR_VIRT     0xC0002000

extern unsigned long __ddrinit_start, __ddrinit_end;
extern unsigned long __tfa_start, __tfa_end;
extern unsigned long __dtb_start, __dtb_end;
#if defined(HAS_BL32)
extern unsigned long __bl32_start, __bl32_end;
#endif
extern unsigned long __uboot_start, __uboot_end;

int main(void)
{
	int ret;

	// Initialize and configure the TOP clock gate
	service_urb_regs_t *serv_urb = service_get_urb_registers();
	serv_urb->top_clkgate = SERVICE_TOP_CLK_GATE_ALL_CH_MASK;

#ifdef UART_ENABLE
	uart_param_t uart = { .uartNum = UART0 };

	console_ops_t uart_console_ops = { .init = uart_drv_config_default,
		                           .putchar = uart_drv_putchar,
		                           .getchar = uart_drv_getchar,
		                           .flush = uart_drv_flush,
		                           .deinit = uart_drv_deinit };

	console_t console = { .hw = &uart, .ops = &uart_console_ops };

	// Initialize and configure the UART0
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

	printf(PREFIX " (" __DATE__ " - " __TIME__ "): " COMMIT "\r\n");
#ifdef BUILD_ID
	printf(PREFIX ": Build: %s\n", BUILD_ID);
#endif

	// Initialize and configure the RISC0 clocking system
	ret = service_set_clock(SERVICE_UCG1_ALL_CH_MASK, SERVICE_UCG1_SYNC_MASK);
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

	// Initialize and configure the InterConnect clocking system
	ret = top_set_clock();
	if (ret)
		return ret;

	// Relocate ddrinit
	INFO("Load images... \n");
	unsigned long *start = (unsigned long *)&__ddrinit_start;
	unsigned long *end = (unsigned long *)&__ddrinit_end;
	uint32_t size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)DDRINIT_START_ADDR_VIRT, (void *)start, size);

	/* Note that ddrinit does memory mapping:
	 * +----------------------------+-------------------------+
	 * | 64Bit phys addrs           |   32bit virt addrs      |
	 * +----------------------------+-------------------------+
	 * | 0x890400000 - 0x8905FFFFF  | 0xC0000000 - 0xC01FFFFF |
	 * +----------------------------+-------------------------+
	 * | 0x880200000 - 0x88FFFFFFF  | 0xC0200000 - 0xCFFFFFFF |
	 * +----------------------------+-------------------------+
	 */
	void (*start_ddrinit)(void) = (void *)DDRINIT_START_ADDR_VIRT;
	start_ddrinit();

	/* Mark the first 256 MB of DDR High as Secure.
	 * This code is provided as example and doesn't affect on security levels at VS_EN = 1
	 * but can be useful for test purpose in case of boot with VS_EN = 0.
	 */
	set_secure_region();

	// Relocate DTB
	start = (unsigned long *)&__dtb_start;
	end = (unsigned long *)&__dtb_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_DTB_ADDR_VIRT, start, size);

	// Relocate TF-A
	start = (unsigned long *)&__tfa_start;
	end = (unsigned long *)&__tfa_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)TFA_START_ADDR_VIRT, (void *)start, size);

#if defined(HAS_BL32)
	// Relocate BL32
	start = (unsigned long *)&__bl32_start;
	end = (unsigned long *)&__bl32_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)BL32_START_ADDR_VIRT, (void *)start, size);
#endif

	// Relocate U-Boot
	start = (unsigned long *)&__uboot_start;
	end = (unsigned long *)&__uboot_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_START_ADDR_VIRT, (void *)start, size);

	/*
	 * Set PLL as clock source for all HSPERIPH UCGs
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

	// Initialize and configure the CPU clocking system and run it
	INFO("Done.\n");
	INFO("Run TF-A ... \n");
	ret = cpu_set_clock();
	if (ret)
		return ret;

	ret = cpu_start_arm0_core(TFA_START_ADDR_PHYS);
	if (ret)
		return ret;

	/* As SBL-XIP no need anymore disable RISC0 CPU here */
	service_disable_risc0_cpu();

	ERROR("Should not be here!!!!!\n");

	return 0;
}
