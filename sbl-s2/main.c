// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <drivers/factory-reset/factory-reset.h>
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

#define PREFIX "SBL-S2"

static uint8_t fw_slot[PLAT_FW_SLOT_SIZE] __attribute__((section(".fw_slot"))) = { 0 };

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
extern uintptr_t __fw_slot_start;
extern uintptr_t __fw_slot_end;
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
static const uintptr_t fw_slot_start = (uintptr_t)&__fw_slot_start;
static const uintptr_t fw_slot_end = (uintptr_t)&__fw_slot_end;
static const uintptr_t ram_start = (uintptr_t)&__ram_start;
static const uintptr_t ram_end = (uintptr_t)&__ram_end;

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
	           ((lAddr >= fw_slot_start) && ((lAddr + size) < fw_slot_end)) ||
	           ((lAddr >= ram_start) && ((lAddr + size) < ram_end))));
}

static int check_exec_address(uintptr_t lAddr, unsigned int size, uintptr_t eAddr)
{
	return !((eAddr >= lAddr) && (eAddr < (lAddr + size)));
}

static int read_image(uintptr_t buffer, uintptr_t offset, size_t size)
{
	int ret;

	if (!buffer)
		return -ENULL;

	ret = spi_nor_read((void *)buffer, offset, size);
	if (ret)
		panic_handler("Failed to read data (0x%08X) to buffer (0x%08X), ret=%d\n", offset,
		              buffer, ret);

	return ret;
}

static int32_t init_env(env_ctx_t *sbl)
{
	if (!sbl)
		return -ENULL;

	env_init(sbl, PLAT_SBL_ENV_OFF, PLAT_ENV_SIZE, ENV_IO_SPI);
	env_import(sbl);

	char *bootvol = env_get(sbl, "bootvol");
	char *safe_bootvol = env_get(sbl, "safe_bootvol");
	if (!bootvol || !safe_bootvol) {
		env_set(sbl, "bootvol", "a");
		env_set(sbl, "safe_bootvol", "a");
		env_export(sbl);
	}

	return 0;
}

static int32_t prepare_recovery(void)
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

static int32_t a_or_b(void *data)
{
	int32_t use_slot_b = 0;
	env_ctx_t *sbl = (env_ctx_t *)data;

	if (!sbl)
		return -ENULL;

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
		use_slot_b = 1;

	NOTICE("Firmware %s slot is selected\n", ((!use_slot_b) ? "A" : "B"));

	return use_slot_b;
}

int main(void)
{
	int ret;
	otp_t otp;
	int32_t use_slot_b;
	env_ctx_t sbl;

#ifdef UART_ENABLE
	uart_param_t uart = { .uart_num = UART0 };

	console_ops_t uart_console_ops = { .init = uart_drv_config_default,
		                           .putchar = uart_drv_putchar,
		                           .getchar = uart_drv_getchar,
		                           .flush = uart_drv_flush,
		                           .deinit = uart_drv_deinit };

	console_t console = { .hw = &uart, .ops = &uart_console_ops };

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

	uint64_t last = timer_get_us();

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
	ret = setup_ddr_regions();
	if (ret)
		return ret;

#ifdef WDT_ENABLE
	// WDT setup configuration
	wdt_dev_t *wdt = wdt_get_instance();

	// Initialize and configure the watchdog
	ret = wdt_set_config(wdt, WDT_MAX_TIMEOUT);
	if (ret)
		return ret;

	ret = wdt_init(wdt);
	if (ret && (ret != -EALREADYINITIALIZED))
		panic_handler("WDT0: Not initialized, ret=%d\n", ret);

	ret = wdt_start(wdt);
	if (ret)
		panic_handler("WDT0: Timeout update failed, ret=%d\n", ret);
#endif

	ret = spi_nor_init();
	if (ret)
		panic_handler("SPI NOR: Initialization failed, ret=%d\n", ret);

	init_env(&sbl);

	NOTICE("Please wait. FW is loading...\n");

	ret = spi_nor_read(&otp, PLAT_OTP_VIRT_OFFSET, sizeof(otp));
	if (ret)
		panic_handler("SPI NOR: Failed to read otp, ret=%d\n", ret);

	uintptr_t img_offset = (uintptr_t)PLAT_OFFSET_FIRMWARE_R;

	sb_mem_t recovery_sbmem = {
		.chck_laddr_func = check_load_address,
		.chck_eaddr_func = check_exec_address,
		.cpy_func = (memcopy_t)memcpy,
		.read_image = (read_image_t)read_image,
		.image_offset = img_offset,
		.buffer = (uintptr_t)fw_slot,
		.otp = otp,
	};

	ret = sblimg_init(&recovery_sbmem);
	while (ret == ESBIMGBOOT_NO_ERR) {
		ret = sblimg_update();
#ifdef WDT_ENABLE
		wdt_reset(wdt);
#endif
	}
	if (ret == ESBIMGBOOT_LOAD_FINISH) {
		factory_reset_info_t factory_reset_info;

		ret = factory_reset_get_info((const void *)PLAT_UBOOT_DTB, &factory_reset_info);
		if (!ret) {
			ret = factory_reset_init(&factory_reset_info);
			if (!ret) {
				uint64_t target = PLAT_RECOVERY_TIMEOUT_SEC * USEC_IN_SEC + last;
				while (factory_reset_is_pressed(&factory_reset_info)) {
#ifdef WDT_ENABLE
					wdt_reset(wdt);
#endif
					if (timer_get_us() > target) {
						NOTICE("Run recovery FW ... \n");
						prepare_recovery();
						sblimg_finish(ESBIMGBOOT_LOAD_FINISH);
					}
				}
			} else {
				ERROR("Failed to init factory pin, ret=%d\n", ret);
			}
		} else if (ret != -FDT_ERR_BADMAGIC && ret != -FDT_ERR_NOTFOUND) {
			ERROR("Failed to get factory pin settings, ret=%d\n", ret);
		}
	} else if (ret != ESBIMGBOOT_IMAGE_BAD_HEADER_ID) {
		sblimg_print_return_code(ret);
	}

	sblimg_abort();

	use_slot_b = a_or_b(&sbl);
	env_deinit(&sbl);

	img_offset = (uintptr_t)(PLAT_OFFSET_FIRMWARE_A +
	                         use_slot_b * (PLAT_OFFSET_FIRMWARE_B - PLAT_OFFSET_FIRMWARE_A));

	sb_mem_t sbmem = {
		.chck_laddr_func = check_load_address,
		.chck_eaddr_func = check_exec_address,
		.cpy_func = (memcopy_t)memcpy,
		.read_image = (read_image_t)read_image,
		.image_offset = img_offset,
		.buffer = (uintptr_t)fw_slot,
		.otp = otp,
	};

	ret = sblimg_init(&sbmem);
	while (ret == ESBIMGBOOT_NO_ERR) {
		ret = sblimg_update();
#ifdef WDT_ENABLE
		wdt_reset(wdt);
#endif
	}
	sblimg_finish(ret);

	return ret;
}
