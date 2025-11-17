// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <drivers/factory-reset/factory-reset.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/spi-nor/spi-nor.h>
#include <drivers/timer/timer.h>
#include <drivers/top/top.h>
#include <libs/console/console.h>
#include <libs/env/env-io.h>
#include <libs/env/env.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/sbimage/sbexecutor.h>
#include <libs/sbimage/sbstatus-print.h>
#include <libs/sbimage/status.h>
#include <libs/utils-def.h>
#include <third-party/libfdt/libfdt.h>

#include "platform-def.h"

#ifdef UART_ENABLE
#include <drivers/uart/uart.h>
#endif

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

#if defined(BOOTSTAGE_ENABLE)
#include <libs/bootstage/bootstage.h>
#endif

extern uintptr_t __spram0_start;
extern uintptr_t __spram0_end;
extern uintptr_t __spram0_uncached_start;
extern uintptr_t __spram0_uncached_end;
extern uintptr_t __cram_start;
extern uintptr_t __cram_end;
extern uintptr_t __cram_uncached_start;
extern uintptr_t __cram_uncached_end;
extern uintptr_t __ddr_low_trusted0_start;
extern uintptr_t __ddr_low_trusted0_end;
extern uintptr_t __ddr_low_sdr0_start;
extern uintptr_t __ddr_low_sdr0_end;
extern uintptr_t __ddr_low_trusted1_start;
extern uintptr_t __ddr_low_trusted1_end;
extern uintptr_t __ddr_low_sdr1_start;
extern uintptr_t __ddr_low_sdr1_end;
extern uintptr_t __iommu_start;
extern uintptr_t __iommu_end;
#if defined(BOOTSTAGE_ENABLE)
extern uintptr_t __bs_start;
extern uintptr_t __bs_end;
#endif
extern uintptr_t __ram_start;
extern uintptr_t __ram_end;

static const uintptr_t spram0_start = (uintptr_t)&__spram0_start;
static const uintptr_t spram0_end = (uintptr_t)&__spram0_end;
static const uintptr_t spram0_uncached_start = (uintptr_t)&__spram0_uncached_start;
static const uintptr_t spram0_uncached_end = (uintptr_t)&__spram0_uncached_end;
static const uintptr_t cram_start = (uintptr_t)&__cram_start;
static const uintptr_t cram_end = (uintptr_t)&__cram_end;
static const uintptr_t cram_uncached_start = (uintptr_t)&__cram_uncached_start;
static const uintptr_t cram_uncached_end = (uintptr_t)&__cram_uncached_end;
static const uintptr_t ddr_low_trusted0_start = (uintptr_t)&__ddr_low_trusted0_start;
static const uintptr_t ddr_low_trusted0_end = (uintptr_t)&__ddr_low_trusted0_end;
static const uintptr_t ddr_low_sdr0_start = (uintptr_t)&__ddr_low_sdr0_start;
static const uintptr_t ddr_low_sdr0_end = (uintptr_t)&__ddr_low_sdr0_end;
static const uintptr_t ddr_low_trusted1_start = (uintptr_t)&__ddr_low_trusted1_start;
static const uintptr_t ddr_low_trusted1_end = (uintptr_t)&__ddr_low_trusted1_end;
static const uintptr_t ddr_low_sdr1_start = (uintptr_t)&__ddr_low_sdr1_start;
static const uintptr_t ddr_low_sdr1_end = (uintptr_t)&__ddr_low_sdr1_end;
static const uintptr_t iommu_start = (uintptr_t)&__iommu_start;
static const uintptr_t iommu_end = (uintptr_t)&__iommu_end;
#if defined(BOOTSTAGE_ENABLE)
static const uintptr_t bs_start = (uintptr_t)&__bs_start;
static const uintptr_t bs_end = (uintptr_t)&__bs_end;
#endif
static const uintptr_t ram_start = (uintptr_t)&__ram_start;
static const uintptr_t ram_end = (uintptr_t)&__ram_end;

#ifdef RECOVERY_ENABLE
static volatile uint64_t last = 0;
#endif

static volatile int recovery_mode = 0;

static int check_load_address(uintptr_t lAddr, unsigned int size)
{
	// TODO: Add CRAM RISC1 region as allowed
	return !((((lAddr >= spram0_start) && ((lAddr + size) < spram0_end)) ||
	          ((lAddr >= spram0_uncached_start) && ((lAddr + size) < spram0_uncached_end)) ||
	          ((lAddr >= ddr_low_trusted0_start) && ((lAddr + size) < ddr_low_trusted0_end)) ||
	          ((lAddr >= ddr_low_sdr0_start) && ((lAddr + size) < ddr_low_sdr0_end)) ||
	          ((lAddr >= ddr_low_trusted1_start) && ((lAddr + size) < ddr_low_trusted1_end)) ||
	          ((lAddr >= ddr_low_sdr1_start) && ((lAddr + size) < ddr_low_sdr1_end))) &&
	         !(((lAddr >= cram_start) && ((lAddr + size) < cram_end)) ||
	           ((lAddr >= cram_uncached_start) && ((lAddr + size) < cram_uncached_end)) ||
	           ((lAddr >= iommu_start) && ((lAddr + size) < iommu_end)) ||
#if defined(BOOTSTAGE_ENABLE)
	           ((lAddr >= bs_start) && ((lAddr + size) < bs_end)) ||
#endif
	           ((lAddr >= ram_start) && ((lAddr + size) < ram_end))));
}

static int check_exec_address(uintptr_t lAddr, unsigned int size, uintptr_t eAddr)
{
	return !((eAddr >= lAddr) && (eAddr < (lAddr + size)));
}

static int read_image(void *dst, signed long offset, size_t size)
{
	uint32_t flash_size = spi_nor_get_size();

	if (offset < 0)
		offset = (signed long)flash_size + offset;

	return spi_nor_read(dst, (uint32_t)offset, size);
}

#ifdef RECOVERY_ENABLE
static int check_recovery_image(const void *data, size_t data_size)
{
	factory_reset_info_t factory_reset_info;

	int ret = factory_reset_get_info(data, &factory_reset_info);
	if (!ret) {
		ret = factory_reset_init(&factory_reset_info);
		if (!ret) {
			uint64_t target = PLAT_RECOVERY_TIMEOUT_SEC * USEC_IN_SEC + last;
			while (factory_reset_is_pressed(&factory_reset_info)) {
#ifdef WDT_ENABLE
				wdt_dev_t *wdt = wdt_get_instance();
				wdt_reset(wdt);
#endif
				if (timer_get_us() > target) {
					recovery_mode = 1;
					return ESBIMGBOOT_LOAD_FINISH;
				}
			}
		} else {
			ERROR("Failed to init factory pin, ret=%d\n", ret);
		}
	} else if (ret != -FDT_ERR_BADMAGIC && ret != -FDT_ERR_NOTFOUND) {
		ERROR("Failed to get factory pin settings, ret=%d\n", ret);
	}

	return 0;
}

static int prepare_recovery(void)
{
	env_ctx_t uboot;
	env_ctx_t saved;

	env_init(&uboot, PLAT_UBOOT_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
	env_invalidate(&uboot);
	env_deinit(&uboot);

	env_init(&saved, PLAT_UBOOT_OLD_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
	env_invalidate(&saved);
	env_deinit(&saved);

	return 0;
}
#endif

static int prepare_env(env_ctx_t *sbl)
{
	bool need_export = false;

	env_init(sbl, PLAT_SBL_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
	env_import(sbl);

	char *bootvol = env_get(sbl, "bootvol");
	char *safe_bootvol = env_get(sbl, "safe_bootvol");
	if (!bootvol || !safe_bootvol) {
		env_set(sbl, "bootvol", "a");
		env_set(sbl, "safe_bootvol", "a");
		need_export = true;
	}

	char *sbl_s2_ver = env_get(sbl, "sbl-s2-ver");
	if (!sbl_s2_ver || strcmp(sbl_s2_ver, BUILD_VERSION_STR)) {
		env_set(sbl, "sbl-s2-ver", BUILD_VERSION_STR);
		env_set_flag(sbl, "sbl-s2-ver", "sr");
		need_export = true;
	}

	if (need_export)
		env_export(sbl);

	return 0;
}

static int select_slot(env_ctx_t *sbl, int *use_slot_b)
{
	if (!sbl || !use_slot_b)
		return -ENULL;

	*use_slot_b = 0;

	NOTICE("Firmware slot selection...\n");

	char *bootvol = env_get(sbl, "bootvol");
	char *safe_bootvol = env_get(sbl, "safe_bootvol");
	if (strcmp(bootvol, safe_bootvol)) {
		char *keep_uboot_env = env_get(sbl, "keep_uboot_env");
		char *tried_to_boot = env_get(sbl, "tried_to_boot");
		if (tried_to_boot) {
			env_set(sbl, "bootvol", safe_bootvol);
			env_set(sbl, "tried_to_boot", NULL);

			if (!keep_uboot_env) {
				// Restore reserved copy of U-Boot environment
				env_ctx_t saved;
				env_init(&saved, PLAT_UBOOT_OLD_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
				env_import(&saved);

				env_ctx_t uboot;
				env_init(&uboot, PLAT_UBOOT_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
				env_move(&uboot, &saved, false); // Move without invalidation
				env_deinit(&uboot);
			}
		} else {
			env_set(sbl, "tried_to_boot", "true");

			if (!keep_uboot_env) {
				// Make reserved copy of U-Boot environment
				env_ctx_t uboot;
				env_init(&uboot, PLAT_UBOOT_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
				env_import(&uboot);

				env_ctx_t saved;
				env_init(&saved, PLAT_UBOOT_OLD_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
				env_move(&saved, &uboot, true); // Move and invalidate
				env_deinit(&saved);
			}
		}
		env_export(sbl);
	}

	bootvol = env_get(sbl, "bootvol");
	if (!strcmp(bootvol, "b"))
		*use_slot_b = 1;

	NOTICE("Firmware %s slot is selected\n", ((!(*use_slot_b)) ? "A" : "B"));

	return 0;
}

static int secure_setup(void)
{
	// Setup Secure Mode TOP Subsystem
	top_urb_regs_t *top_urb = (top_urb_regs_t *)top_get_urb_registers();

	// All (HSPeriph, GIC500, MEDIA, LSPeriph0, LSPeriph1, SDR PCIE Low) set to non-secure mode
	top_set_nonsecure_to_secure(top_urb, BIT(TOP_MEDIA_SUBS_DP1_SECURE_POS));

	// All (CPU_SUB_SYS, HS_PERIPH, LS_PERIPH0, LS_PERIPH1, PCIE) are set to secure
	top_reset_trusted_to_secure(top_urb, TOP_TRUSTEDTOSECURE_MASK);

	uint32_t is_trusted_mask =
		BIT(TOP_CPU_SUBS_DEBUG_TRUSTED_POS) | BIT(TOP_TOP_TRUSTED_POS) |
		BIT(TOP_DDR_SUBS_DDR0_TRUSTED_POS) | BIT(TOP_DDR_SUBS_DDR1_TRUSTED_POS) |
		BIT(TOP_DDR_SUBS_SYS_TRUSTED_POS) | BIT(TOP_SERVICE_SUBS_MAILBOX0_SF_TRUSTED_POS) |
		BIT(TOP_SERVICE_SUBS_SYS_TRUSTED_POS);

	top_set_trusted_to_secure(top_urb, is_trusted_mask);

	// Set DDR Low as Trusted and SDR
	top_set_low_ddr_secure_bit(top_urb, TOP_DDR_LOW_RANGE_SECURE_CTR_MASK);

	// Setup DDR mapping and secure regions
	return setup_ddr_regions();
}

int main(void)
{
	int ret;
	env_ctx_t sbl;
	int use_slot_b = 0;
	sb_mem_t sbmem;
	otp_t otp;

	/* The timer data shared between boot stages is not in .bss section.
	 * So we have to reset it once here.
	 */
	timer_reset();

	ret = irq_init();
	if (ret)
		return ret;

	// Register LSP1 TIMERS as system timer.
	// Don't init timer as it is done early.
	ret = lsp1_timer_register(false, 0, true);
	if (ret)
		return ret;

#if defined(BOOTSTAGE_ENABLE)
	bootstage_import((void *)bs_start, bs_end - bs_start);
	bootstage_mark(BOOTSTAGE_ID_SBL_S2_START);
#endif

#ifdef RECOVERY_ENABLE
	last = timer_get_us();
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

	// Initialize and configure the watchdog
	ret = wdt_config_default(wdt);
	if (ret)
		panic_handler("Failed to apply default config to WDT instance, ret=%d\n", ret);

	ret = wdt0_hw_enable(wdt);
	if (ret)
		panic_handler("Failed to initialize WDT0 hardware, ret=%d\n", ret);

	ret = wdt_start(wdt);
	if (ret && (ret != -EALREADYINITIALIZED))
		panic_handler("WDT0 is failed to start, ret=%d\n", ret);
#endif

	ret = spi_nor_init();
	if (ret)
		panic_handler("SPI NOR init failed, ret=%d\n", ret);

#if defined(BOOTSTAGE_ENABLE)
	bootstage_mark(BOOTSTAGE_ID_SBL_S2_LOAD_START);
#endif

	ret = read_image(&otp, PLAT_OTP_OFFSET, sizeof(otp));
	if (ret)
		return ret;

#ifdef RECOVERY_ENABLE
	NOTICE("Please wait. FW is checking...\n");

	memset((void *)&sbmem, 0, sizeof(sbmem));
	sbmem.chck_laddr_func = NULL;
	sbmem.chck_eaddr_func = NULL;
	sbmem.chck_img = (chck_img_t)check_recovery_image;
	sbmem.cpy_func = (memcopy_t)memcpy;
	sbmem.read_img_func = (read_img_t)read_image;
	sbmem.image_offset = (uintptr_t)PLAT_OFFSET_FIRMWARE_R;
	sbmem.otp = otp;

	ret = sblimg_init(&sbmem);
	while (ret == ESBIMGBOOT_NO_ERR) {
		ret = sblimg_check();
#ifdef WDT_ENABLE
		wdt_reset(wdt);
#endif
	}

	sblimg_abort();

	if (ret != ESBIMGBOOT_IMAGE_BAD_HEADER_ID && ret != ESBIMGBOOT_IMAGE_BAD_HEADER_HASH)
		sblimg_print_return_code(ret);
#endif

	ret = secure_setup();
	if (ret)
		panic_handler("Failed to setup secure regions, ret=%d\n", ret);

	prepare_env(&sbl);

	memset((void *)&sbmem, 0, sizeof(sbmem));
	sbmem.chck_laddr_func = (chck_laddr_t)check_load_address;
	sbmem.chck_eaddr_func = (chck_eaddr_t)check_exec_address;
	sbmem.cpy_func = (memcopy_t)memcpy;
	sbmem.read_img_func = (read_img_t)read_image;
	sbmem.chck_img = NULL;
	sbmem.otp = otp;

	if (recovery_mode == 0) {
		select_slot(&sbl, &use_slot_b);

		sbmem.image_offset =
			(uintptr_t)(PLAT_OFFSET_FIRMWARE_A +
		                    use_slot_b * (PLAT_OFFSET_FIRMWARE_B - PLAT_OFFSET_FIRMWARE_A));

		NOTICE("Please wait. FW is loading...\n");
#ifdef RECOVERY_ENABLE
	} else {
		prepare_recovery();

		sbmem.image_offset = (uintptr_t)(PLAT_OFFSET_FIRMWARE_R);

		NOTICE("Please wait. Recovery FW is loading...\n");
#endif
	}

	ret = sblimg_init(&sbmem);
	while (ret == ESBIMGBOOT_NO_ERR) {
		ret = sblimg_update();
#ifdef WDT_ENABLE
		wdt_reset(wdt);
#endif
	}

#if defined(BOOTSTAGE_ENABLE)
	bootstage_mark(BOOTSTAGE_ID_SBL_S2_LOAD_COMPLETE);
	bootstage_export((void *)bs_start, bs_end - bs_start);
#endif

	sblimg_finish(ret);
}
