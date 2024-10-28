// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <string.h>
#include <log.h>
#include <mmio.h>
#include <platform_def.h>
#include <platform_helpers.h>
#include <drivers/service_subs/urb.h>
#include <drivers/top/top.h>
#include <drivers/iommu/iommu.h>
#include <drivers/qspi/qspi.h>
#include <drivers/pll/pll.h>
#include <drivers/ucg/ucg.h>
#include <drivers/nvm/nvm.h>
#include <drivers/factory_reset/factory_reset.h>
#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif /* WDT_ENABLE */
#ifdef ENV_ENABLE
#include <lib/env/env.h>
#else
#include <lib/ab_storage/ab_storage.h>
#endif
#include <lib/sbimage/sbexecutor.h>
#include <lib/sbimage/sbimage.h>

#define PFX "SBL-TL"

#define DDRMC_MAX_NUMBER      2U
#define MAX_MEM_REGIONS	      4U
#define MEM_REGIONS_VIRT_ADDR 0xC0000000U
#define MEM_REGIONS_PHYS_ADDR 0x890400000ULL

#define CTR_SECURE_REGION	     (BASE_ADDR_DDR_SYS_URB + 0x80U)
#define BASE_SECURE_REGION_LOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x90U + (num * 0x10U)))
#define BASE_SECURE_REGION_HIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x94U + (num * 0x10U)))
#define MASK_SECURE_REGION_LOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x98U + (num * 0x10U)))
#define MASK_SECURE_REGION_HIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x9CU + (num * 0x10U)))

#define SECURE_REGIONS_ADDR_START 0x880000000ULL
#define SECURE_REGIONS_ADDR_SIZE  0x10000000ULL
#define SECURE_REGIONS_ADDR_MASK  (~(SECURE_REGIONS_ADDR_SIZE - 1ULL))
#define SECURE_REGIONS_ADDR_END	  (SECURE_REGIONS_ADDR_START + SECURE_REGIONS_ADDR_SIZE)

struct ddrinfo {
	uint64_t dram_size[DDRMC_MAX_NUMBER];
	uint64_t total_dram_size;
	struct {
		bool enable;
		int channels;
		int size;
	} interleaving;
	int speed[DDRMC_MAX_NUMBER];
	/* RAM configuration */
	struct {
		uint64_t start;
		uint64_t size;
	} mem_regions[MAX_MEM_REGIONS];
};

static uint8_t fw_slot[PLAT_FW_SLOT_SIZE] __attribute__((section(".fw_slot"))) = { 0 };

extern unsigned long __spram0_start;
extern unsigned long __spram0_end;
extern unsigned long __spram0_uncached_start;
extern unsigned long __spram0_uncached_end;
extern unsigned long __ddr_low_trusted0_start;
extern unsigned long __ddr_low_trusted0_end;
extern unsigned long __ddr_low_sdr0_start;
extern unsigned long __ddr_low_sdr0_end;
extern unsigned long __ddr_low_trusted1_start;
extern unsigned long __ddr_low_trusted1_end;
extern unsigned long __ddr_low_sdr1_start;
extern unsigned long __ddr_low_sdr1_end;
extern unsigned long __iommu_start;
extern unsigned long __iommu_end;
extern unsigned long __fw_slot_start;
extern unsigned long __fw_slot_end;
extern unsigned long __ram_start;
extern unsigned long __ram_end;

static const unsigned long *spram0_start = (unsigned long *)&__spram0_start;
static const unsigned long *spram0_end = (unsigned long *)&__spram0_end;
static const unsigned long *spram0_uncached_start = (unsigned long *)&__spram0_uncached_start;
static const unsigned long *spram0_uncached_end = (unsigned long *)&__spram0_uncached_end;
static const unsigned long *ddr_low_trusted0_start = (unsigned long *)&__ddr_low_trusted0_start;
static const unsigned long *ddr_low_trusted0_end = (unsigned long *)&__ddr_low_trusted0_end;
static const unsigned long *ddr_low_sdr0_start = (unsigned long *)&__ddr_low_sdr0_start;
static const unsigned long *ddr_low_sdr0_end = (unsigned long *)&__ddr_low_sdr0_end;
static const unsigned long *ddr_low_trusted1_start = (unsigned long *)&__ddr_low_trusted1_start;
static const unsigned long *ddr_low_trusted1_end = (unsigned long *)&__ddr_low_trusted1_end;
static const unsigned long *ddr_low_sdr1_start = (unsigned long *)&__ddr_low_sdr1_start;
static const unsigned long *ddr_low_sdr1_end = (unsigned long *)&__ddr_low_sdr1_end;
static const unsigned long *iommu_start = (unsigned long *)&__iommu_start;
static const unsigned long *iommu_end = (unsigned long *)&__iommu_end;
static const unsigned long *fw_slot_start = (unsigned long *)&__fw_slot_start;
static const unsigned long *fw_slot_end = (unsigned long *)&__fw_slot_end;
static const unsigned long *ram_start = (unsigned long *)&__ram_start;
static const unsigned long *ram_end = (unsigned long *)&__ram_end;

static int check_load_address(unsigned long lAddr, unsigned int size)
{
	//TODO: Add CRAM RISC1 region as allowed
	return !((((lAddr >= (unsigned long)spram0_start) &&
		   ((lAddr + size) < (unsigned long)spram0_end)) ||
		  ((lAddr >= (unsigned long)spram0_uncached_start) &&
		   ((lAddr + size) < (unsigned long)spram0_uncached_end)) ||
		  ((lAddr >= (unsigned long)ddr_low_trusted0_start) &&
		   ((lAddr + size) < (unsigned long)ddr_low_trusted0_end)) ||
		  ((lAddr >= (unsigned long)ddr_low_sdr0_start) &&
		   ((lAddr + size) < (unsigned long)ddr_low_sdr0_end)) ||
		  ((lAddr >= (unsigned long)ddr_low_trusted1_start) &&
		   ((lAddr + size) < (unsigned long)ddr_low_trusted1_end)) ||
		  ((lAddr >= (unsigned long)ddr_low_sdr1_start) &&
		   ((lAddr + size) < (unsigned long)ddr_low_sdr1_end))) &&
		 !(((lAddr >= (unsigned long)iommu_start) &&
		    ((lAddr + size) < (unsigned long)iommu_end)) ||
		   ((lAddr >= (unsigned long)fw_slot_start) &&
		    ((lAddr + size) < (unsigned long)fw_slot_end)) ||
		   ((lAddr >= (unsigned long)ram_start) &&
		    ((lAddr + size) < (unsigned long)ram_end))));
}

static int check_exec_address(unsigned long lAddr, unsigned int size, unsigned long eAddr)
{
	return !((eAddr >= lAddr) && (eAddr < (lAddr + size)));
}

static int check_fw_counter(uint8_t trusted_fw_counter)
{
#ifdef ENV_ENABLE
	return MCOM03_SUCCESS;
#else
	uint8_t curr_fw_counter;

	if (ab_storage_fw_counter_get(&curr_fw_counter))
		return MCOM03_ERROR_INTERNAL;

	if (trusted_fw_counter < curr_fw_counter)
		return MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_FW_COUNTER;

	NOTICE("Current firmware has counter = %d\r\n", curr_fw_counter);
	NOTICE("New image has counter = %d\r\n", trusted_fw_counter);

	return MCOM03_SUCCESS;
#endif
}

static uint32_t set_fw_counter(uint8_t trusted_fw_counter)
{
#ifdef ENV_ENABLE
	return 0;
#else
	return ab_storage_fw_counter_set(trusted_fw_counter);
#endif
}

static int read_image(uintptr_t buffer, uintptr_t offset, size_t size)
{
	int ret;

	if (!buffer)
		return MCOM03_ERROR_NULL;

	ret = nvm_read(&nvm_device, (void *)buffer, offset, size);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("Failed to read data (0x%08X) to buffer (0x%08X), ret=%d\r\n",
				   offset, buffer, ret);
	}

	return ret;
}

#ifdef ENV_ENABLE
static int32_t init_env(env_ctx_t *sbl)
{
	if (!sbl)
		return -1;

	env_init(sbl, PLAT_SBL_ENV_OFFSET, PLAT_ENV_SIZE);
	env_import(sbl);

	char *bootvol = env_get(sbl, "bootvol");
	char *safe_bootvol = env_get(sbl, "safe_bootvol");
	if (bootvol == NULL || safe_bootvol == NULL) {
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

	env_init(&uboot, PLAT_UBOOT_ENV_OFFSET, PLAT_ENV_SIZE);
	env_invalidate(&uboot);

	env_init(&saved, PLAT_UBOOT_BACKUP_ENV_OFFSET, PLAT_ENV_SIZE);
	env_invalidate(&saved);

	return 0;
}

static int32_t a_or_b(void *data)
{
	int32_t use_slot_b = 0;
	env_ctx_t *sbl = (env_ctx_t *)data;

	if (!sbl)
		return -1;

	NOTICE("Firmware slot selection...\r\n");

	char *bootvol = env_get(sbl, "bootvol");
	char *safe_bootvol = env_get(sbl, "safe_bootvol");
	if (strcmp(bootvol, safe_bootvol) != 0) {
		env_ctx_t uboot;
		env_init(&uboot, PLAT_UBOOT_ENV_OFFSET, PLAT_ENV_SIZE);
		env_ctx_t saved;
		env_init(&saved, PLAT_UBOOT_BACKUP_ENV_OFFSET, PLAT_ENV_SIZE);

		char *keep_uboot_env = env_get(sbl, "keep_uboot_env");
		char *tried_to_boot = env_get(sbl, "tried_to_boot");
		if (tried_to_boot) {
			env_set(sbl, "bootvol", safe_bootvol);
			env_set(sbl, "tried_to_boot", NULL);

			if (!keep_uboot_env) {
				// Restore reserved copy of U-Boot environment
				env_import(&saved);
				env_move(&uboot, &saved);
				env_export(&uboot);

				env_destroy(&uboot);
			}
		} else {
			env_set(sbl, "tried_to_boot", "true");

			if (!keep_uboot_env) {
				// Make reserved copy of U-Boot environment
				env_import(&uboot);
				env_move(&saved, &uboot);
				env_export(&saved);

				// Invalidate U-Boot environment
				env_invalidate(&uboot);

				env_destroy(&saved);
			}
		}
		env_export(sbl);
	}

	bootvol = env_get(sbl, "bootvol");
	if (strcmp(bootvol, "b") == 0) {
		use_slot_b = 1;
	}

	NOTICE("Firmware %s slot is selected\r\n", ((use_slot_b == 0) ? "A" : "B"));

	return use_slot_b;
}
#else
static int32_t a_or_b(void *data)
{
	(void)data;

	int32_t use_slot_b = 0;

	NOTICE("Firmware slot selection...\r\n");

	use_slot_b = ab_storage_before_boot(0);
	if (use_slot_b == SFS2_NO_DATA_ERROR) {
		NOTICE("AB storage is empty. Initializing...\r\n");
		use_slot_b = ab_storage_init();
	} else {
		plat_panic_handler("Problem with Storage, error code %ld\r\n", use_slot_b);
	}

	NOTICE("Firmware %s slot is selected\r\n", ((use_slot_b == 0) ? "A" : "B"));

	return use_slot_b;
}
#endif

int main(void)
{
	mcom_err_t ret;
	otp_t otp;
	int32_t use_slot_b;
#ifdef ENV_ENABLE
	env_ctx_t sbl;
#endif

	NOTICE(PFX " (" __DATE__ " - " __TIME__ "): " COMMIT "\r\n");
#ifdef BUILD_ID
	NOTICE(PFX ": Build: %s\r\n", BUILD_ID);
#endif

	uint64_t last = plat_get_ticks();

	/* Setup Secure Mode TOP Subsystem */
	top_urb_regs_t *top_urb = (top_urb_regs_t *)top_get_urb_registers();

	/* All (HSPeriph, GIC500, MEDIA, LSPeriph0, LSPeriph1, SDR PCIE Low) set to non-secure mode */
	top_set_nonsecure_to_secure(top_urb, TOP_MEDIA_SUBS_DP1_SECURE_POS);

	/* All (CPU_SUB_SYS, HS_PERIPH, LS_PERIPH0, LS_PERIPH1, PCIE) are set to secure */
	top_reset_trusted_to_secure(top_urb, TOP_TRUSTEDTOSECURE_MASK);

	uint32_t is_trusted_mask =
		(TOP_CPU_SUBS_DEBUG_TRUSTED_POS | TOP_TOP_TRUSTED_POS |
		 TOP_DDR_SUBS_DDR0_TRUSTED_POS | TOP_DDR_SUBS_DDR1_TRUSTED_POS |
		 TOP_DDR_SUBS_SYS_TRUSTED_POS | TOP_SERVICE_SUBS_MAILBOX0_SF_TRUSTED_POS |
		 TOP_SERVICE_SUBS_SYS_TRUSTED_POS);

	top_set_trusted_to_secure(top_urb, is_trusted_mask);

	/* Set DDR Low as Trusted and SDR */
	top_set_low_ddr_secure_bit(top_urb, (TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_0_POS |
					     TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_1_POS |
					     TOP_DDR_LOW_SDR_TO_SECURE_RANGE_0_POS |
					     TOP_DDR_LOW_SDR_TO_SECURE_RANGE_1_POS));

	/* Mark the first 256 MB of DDR High as Secure */
	mmio_write_32(BASE_SECURE_REGION_LOW(0U), (uint32_t)SECURE_REGIONS_ADDR_START);
	mmio_write_32(BASE_SECURE_REGION_HIGH(0U), (uint32_t)(SECURE_REGIONS_ADDR_START >> 32ULL));
	mmio_write_32(MASK_SECURE_REGION_LOW(0U), (uint32_t)SECURE_REGIONS_ADDR_MASK);
	mmio_write_32(MASK_SECURE_REGION_HIGH(0U), (uint32_t)(SECURE_REGIONS_ADDR_MASK >> 32ULL));
	mmio_write_32(CTR_SECURE_REGION, 0x1U);

	iommu_t *iommu_reg = (iommu_t *)iommu_get_registers();
	iommu_deinit(iommu_reg);
	iommu_init(iommu_reg, (void *)0x40000000);

	ret = iommu_map_64bit_address(iommu_reg, MEM_REGIONS_VIRT_ADDR, MEM_REGIONS_PHYS_ADDR);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("iommu: address mapping failed, ret=%ld\r\n", ret);
	}

	struct ddrinfo *info = (struct ddrinfo *)MEM_REGIONS_VIRT_ADDR;

	/* TODO: Currently we are support one secure region started from the beginning of
	 * DDR High. The region size must be a power of 2.
	 * It is required to modify of ddrinfo struct to add several region dynamically.
	 */
	for (int i = 0; i < MAX_MEM_REGIONS; ++i) {
		uint64_t start = info->mem_regions[i].start;
		uint64_t end = info->mem_regions[i].start + info->mem_regions[i].size;
		if ((SECURE_REGIONS_ADDR_START >= start) && (SECURE_REGIONS_ADDR_END <= end)) {
			info->mem_regions[i].start = SECURE_REGIONS_ADDR_END;
			info->mem_regions[i].size = end - SECURE_REGIONS_ADDR_END;
		}
	}

	ret = iommu_map_range_64bit_address(iommu_reg, 0xC0200000, 0xFE00000, 0x880200000);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("iommu: range mapping failed, ret=%ld\r\n", ret);
	}

#ifdef WDT_ENABLE
	/* WDT setup configuration */
	service_urb_regs_t *ss_urb = getServiceURBRegisters();
	uint32_t freq_mul = 0;
	uint32_t freq_div = 0;

	pll_get_freq_mult((pll_cfg_reg_t *)&ss_urb->service_subs_pllcnfg, &freq_mul);
	ucg_regs_t *ss_ucg = ucg_get_service_registers(0);
	ucg_get_divider(ss_ucg, SERVICE_UCG1_CHANNEL_CLK_APB, &freq_div);

	wdt_dev_t wdt;
	wdt.id = WDT0;
	wdt.rmod = WDT_RST_MODE;
	wdt.rpl = WDT_RST_PULSE_LEN_2;
	wdt.timeout = WDT_MAX_TIMEOUT;
	wdt.wdt_freq = ((XTI_CLK_HZ * freq_mul) / freq_div);

	ret = wdt_init(&wdt);
	if ((ret != MCOM03_SUCCESS) && (ret != MCOM03_ERROR_ALREADY_INITIALIZED)) {
		plat_panic_handler("WDT0: Not initialized, ret=%ld\r\n", ret);
	}

	ret = wdt_start(&wdt);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("WDT0: Timeout update failed, ret=%ld\r\n", ret);
	}
#endif /* WDT_ENABLE */

	ret = nvm_deinit(&nvm_device);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("NVM: Deinitialization failed, ret=%ld\r\n", ret);
	}
	ret = nvm_init(&nvm_device);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("NVM: Initialization failed, ret=%ld\r\n", ret);
	}

#ifdef ENV_ENABLE
	init_env(&sbl);
#endif

	NOTICE("Please wait. FW is loading...\r\n");

	ret = nvm_read(&nvm_device, &otp, OTP_VIRT_OFFSET, sizeof(otp));
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("NVM: Failed to read otp, ret=%ld\r\n", ret);
	}

	uintptr_t img_offset = (uintptr_t)OFFSET_FIRMWARE_R;

	sb_mem_t recovery_sbmem = {
		.chck_laddr_func = check_load_address,
		.chck_eaddr_func = check_exec_address,
		.chck_fw_func = (chck_fw_t)check_fw_counter,
		.set_curr_fw_func = (set_curr_fw_t)set_fw_counter,
		.cpy_func = (memcopy_t)memcpy,
		.read_image = (read_image_t)read_image,
		.image_offset = img_offset,
		.buffer = (uintptr_t)fw_slot,
		.otp = otp,
	};

	ret = sblimg_init(&recovery_sbmem);
	while (ret == MCOM03_STATUS_SBIMG_BOOT_NO_ERR) {
		ret = sblimg_update();
#ifdef WDT_ENABLE
		wdt_reset(&wdt);
#endif
	}
	if (ret == MCOM03_STATUS_SBIMG_BOOT_LOAD_FINISH) {
		factory_reset_info_t factory_reset_info;

		ret = factory_reset_get_info((const void *)PLAT_UBOOT_DTB, &factory_reset_info);
		if (!ret) {
			ret = factory_reset_init(&factory_reset_info);
			if (!ret) {
				uint64_t core_freq = plat_get_cpu_frequency();
				uint64_t target =
					plat_ms_to_tick(RECOVERY_TIMEOUT, core_freq) + last;
				while (factory_reset_is_pressed(&factory_reset_info)) {
#ifdef WDT_ENABLE
					wdt_reset(&wdt);
#endif
					if (plat_get_ticks() > target + 1) {
						NOTICE("Run recovery FW ... \r\n");
#ifdef ENV_ENABLE
						prepare_recovery();
#endif
						sblimg_finish(MCOM03_STATUS_SBIMG_BOOT_LOAD_FINISH);
					}
				}
			} else {
				ERROR("Failed to init factory pin, ret=%ld\r\n", ret);
			}
		} else if (ret != -FDT_ERR_BADMAGIC && ret != -FDT_ERR_NOTFOUND) {
			ERROR("Failed to get factory pin settings, ret=%ld\r\n", ret);
		}
	} else if (ret != MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_HEADER_ID) {
		sblimg_print_status(ret);
	}

	sblimg_abort();

#ifdef ENV_ENABLE
	use_slot_b = a_or_b(&sbl);
	env_destroy(&sbl);
#else
	use_slot_b = a_or_b(NULL);
#endif

	img_offset = (uintptr_t)(OFFSET_FIRMWARE_A +
				 use_slot_b * (OFFSET_FIRMWARE_B - OFFSET_FIRMWARE_A));

	sb_mem_t sbmem = {
		.chck_laddr_func = check_load_address,
		.chck_eaddr_func = check_exec_address,
		.chck_fw_func = (chck_fw_t)check_fw_counter,
		.set_curr_fw_func = (set_curr_fw_t)set_fw_counter,
		.cpy_func = (memcopy_t)memcpy,
		.read_image = (read_image_t)read_image,
		.image_offset = img_offset,
		.buffer = (uintptr_t)fw_slot,
		.otp = otp,
	};

	ret = sblimg_init(&sbmem);
	while (ret == MCOM03_STATUS_SBIMG_BOOT_NO_ERR) {
		ret = sblimg_update();
#ifdef WDT_ENABLE
		wdt_reset(&wdt);
#endif
	}
	sblimg_finish(ret);

	return ret;
}
