// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <drivers/cpu/cpu.h>
#include <drivers/iommu/iommu.h>
#include <drivers/irq/irq.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mailbox/mailbox.h>
#include <drivers/pll/pll.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/console/console.h>
#include <libs/errors.h>
#include <libs/log.h>

#include "platform-def.h"
#include "risc0-ipc/server/api.h"

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

#ifdef UART_ENABLE
#include <drivers/uart/uart.h>
#endif

#if defined(BOOTSTAGE_ENABLE)
#include <libs/bootstage/bootstage.h>
#endif

int main(int argc, char **argv)
{
	int ret;

	ret = irq_init();
	if (ret)
		return ret;

	// Register LSP1 TIMERS as system timer.
	// Don't init timer as it is done early.
	ret = lsp1_timer_register(false, 0, true);
	if (ret)
		return ret;

#if defined(BOOTSTAGE_ENABLE)
	extern uintptr_t __bs_start;
	extern uintptr_t __bs_end;
	const uintptr_t bs_start = (uintptr_t)&__bs_start;
	const uintptr_t bs_end = (uintptr_t)&__bs_end;

	bootstage_import((void *)bs_start, bs_end - bs_start);
	bootstage_mark(BOOTSTAGE_ID_SBL_S3_START);
#endif

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

#ifdef WDT_ENABLE
	wdt_dev_t *wdt = wdt_get_instance();

	ret = wdt_set_config(wdt, WDT_MAX_TIMEOUT);
	if (ret)
		panic_handler("Failed to set wdt config, ret=%d\n", ret);

	ret = wdt_init(wdt);
	if (ret && (ret != -EALREADYINITIALIZED))
		panic_handler("WDT init failed, ret=%d\n", ret);

	ret = wdt_start(wdt);
	if (ret)
		panic_handler("WDT start failed, ret=%d\n", ret);
#endif

	// Initialize IOMMU
	iommu_regs_t *iommu_regs = iommu_get_registers();

	iommu_reset(iommu_regs);
	iommu_init(iommu_regs, (void *)(0x40000000));

	mailbox_regs_t *regs = mbox_get_regs();
	ret = mbox_init(regs);
	if (ret)
		panic_handler("Mailbox0 init failed, ret=%d\n", ret);

	ret = risc0_ipc_start();
	if (ret)
		panic_handler("Services start failed, ret=%d\n", ret);

	ret = cpu_set_clock();
	if (ret)
		panic_handler("Fail to setup ARM core freq, ret=%d\n", ret);

	// TODO: add macros to be able to change entrypoint during building
	ret = cpu_start_arm0_core(0x880300000);
	if (ret)
		panic_handler("Fail to start ARM core, ret=%d\n", ret);

	for (;;) {
		risc0_ipc_handler();

#if defined(WDT_ENABLE) && defined(WDT_RESET_INTERNAL)
		wdt_reset(wdt);
#endif

		__asm__ volatile("nop");
	}
}
