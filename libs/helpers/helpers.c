// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stdint.h>

#include <drivers/hs-periph/hs-periph.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "helpers.h"

#define PLAT_4KiB		    (0x1000ULL)
#define PLAT_4KiB_64BIT_OFFSET_MASK (PLAT_4KiB - 1ULL)
#define PLAT_4KiB_64BIT_ADDR_MASK   (~PLAT_4KiB_64BIT_OFFSET_MASK)

#define PLAT_2MiB		    (0x200000ULL)
#define PLAT_2MiB_32BIT_OFFSET_MASK (PLAT_2MiB - 1ULL)
#define PLAT_2MiB_32BIT_ADDR_MASK   (~PLAT_2MiB_32BIT_OFFSET_MASK)

#define DDRMC_MAX_NUMBER      2
#define MAX_MEM_REGIONS       4
#define MEM_REGIONS_VIRT_ADDR 0xC0000000

#define SECURE_REGIONS_PHYS_ADDR_START 0x880000000ULL
#define SECURE_REGIONS_PHYS_ADDR_SIZE  0x10000000ULL
#define SECURE_REGIONS_PHYS_ADDR_MASK  (~(SECURE_REGIONS_PHYS_ADDR_SIZE - 1))
#define SECURE_REGIONS_PHYS_ADDR_END \
	(SECURE_REGIONS_PHYS_ADDR_START + SECURE_REGIONS_PHYS_ADDR_SIZE)

struct ddrinfo {
	uint64_t dram_size[DDRMC_MAX_NUMBER];
	uint64_t total_dram_size;
	struct {
		bool enable;
		int channels;
		int size;
	} interleaving;
	int speed[DDRMC_MAX_NUMBER];
	// RAM configuration
	struct {
		uint64_t start;
		uint64_t size;
	} mem_regions[MAX_MEM_REGIONS];
};

static void cpu_delay_ticks(uint32_t ticks)
{
	if (!ticks) {
		return;
	}
	uint32_t start = mips_read_c0_register(C0_COUNT);
	while (mips_read_c0_register(C0_COUNT) - start < ticks) {
		__asm__ volatile("nop");
	}
}

static void cpu_delay_us(uint32_t us)
{
	uint32_t core_frequency = plat_get_cpu_frequency();
	uint32_t ticks = (uint64_t)us * core_frequency / USEC_IN_SEC;
	cpu_delay_ticks(ticks);
}

void plat_irq_handler(void)
{
	intmgr_handler();
}

/**
 * @brief plat_time_delay
 *
 * @param num_msec Delay time in milliseconds.
 *
 * This is not an accurate delay, it ensures at least num_msec passed when return.
 */
void plat_time_delay(uint32_t num_msec)
{
	cpu_delay_us(num_msec * USEC_IN_MSEC);
}

/**
 * @brief plat_time_delay_us
 *
 * @param num_usec Delay time in microseconds.
 *
 * Delay is not exactly accurate
 */
void plat_time_delay_us(uint32_t num_usec)
{
	cpu_delay_us(num_usec);
}

/**
 * @brief Get current CPU frequency
 *
 * @return Current CPU frequency
 */
uint32_t plat_get_cpu_frequency(void)
{
	service_urb_regs_t *ss_urb = getServiceURBRegisters();
	ucg_regs_t *ss_ucg = ucg_get_service_registers(0);
	uint32_t freq_mul = 0;
	uint32_t freq_div = 0;

	pll_get_freq_mult((pll_cfg_reg_t *)&ss_urb->service_subs_pllcnfg, &freq_mul);
	ucg_get_divider(ss_ucg, SERVICE_UCG1_CHANNEL_CLK_CORE, &freq_div);
	return XTI_CLK_HZ * freq_mul / freq_div;
}

void plat_disable_irq_global(void)
{
	intmgr_global_disable_irq();
}

void plat_enable_irq_global(void)
{
	intmgr_global_enable_irq();
}

uintptr_t plat_map64(uint64_t addr64)
{
	iommu_t *iommu_reg = NULL;
	uintptr_t iobase = (uintptr_t)NULL;
	uintptr_t offset = (uintptr_t)NULL;

	iommu_reg = (iommu_t *)iommu_get_registers();
	iobase = iommu_register_64bit_address(iommu_reg, (addr64 & PLAT_4KiB_64BIT_ADDR_MASK));
	if (iobase == (uintptr_t)NULL) {
		plat_panic_handler("iommu failed to register 64bit addr\r\n");
	}

	offset = addr64 & PLAT_4KiB_64BIT_OFFSET_MASK;

	return (uintptr_t)(iobase + offset);
}

void plat_unmap64(uintptr_t addr32)
{
	iommu_t *iommu_reg = NULL;
	uintptr_t iobase = (uintptr_t)(addr32 & PLAT_2MiB_32BIT_ADDR_MASK);

	iommu_reg = (iommu_t *)iommu_get_registers();
	if (iommu_unregister_64bit_address(iommu_reg, iobase) != 0) {
		plat_panic_handler("iommu failed to unregister 64bit addr\r\n");
	}
}

void *plat_allocate_memory(uint32_t size)
{
	return malloc(size);
}

void *plat_reallocate_memory(void *ptr, uint32_t size)
{
	return realloc(ptr, size);
}

void plat_free_memory(void *ptr)
{
	if (ptr) {
		free(ptr);
	}
void set_secure_region(void)
{
	/**
	 * Mark the first 256 MB of DDR High as Secure.
	 * This code is provided as example and doesn't affect on security levels at VS_EN = 1
	 * but can be useful for test purpose in case of boot with VS_EN = 0.
	 */
	mmio_write_32(DDR_SYS_URB_BASESECUREREGIONLOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_START);
	mmio_write_32(DDR_SYS_URB_BASESECUREREGIONHIGH(0),
	              (uint32_t)(SECURE_REGIONS_PHYS_ADDR_START >> 32ULL) & 0xF);
	mmio_write_32(DDR_SYS_URB_MASKSECUREREGIONLOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_MASK);
	mmio_write_32(DDR_SYS_URB_MASKSECUREREGIONHIGH(0),
	              (uint32_t)(SECURE_REGIONS_PHYS_ADDR_MASK >> 32ULL) & 0xF);
	mmio_write_32(DDR_SYS_URB_CTRSECUREREGION, 0x1);

	/**
	 * TODO: Currently we are support one secure region started from the beginning of
	 * DDR High. The region size must be a power of 2. It is required to modify of
	 * ddrinfo struct to add several regions dynamically.
	 */
	struct ddrinfo *info = (struct ddrinfo *)MEM_REGIONS_VIRT_ADDR;
	for (int i = 0; i < MAX_MEM_REGIONS; ++i) {
		uint64_t start = info->mem_regions[i].start;
		uint64_t end = info->mem_regions[i].start + info->mem_regions[i].size;
		if ((SECURE_REGIONS_PHYS_ADDR_START >= start) &&
		    (SECURE_REGIONS_PHYS_ADDR_END <= end)) {
			info->mem_regions[i].start = SECURE_REGIONS_PHYS_ADDR_END;
			info->mem_regions[i].size = end - SECURE_REGIONS_PHYS_ADDR_END;
		}
	}
}

int soc_debug_if_disable(void)
{
#ifdef DEBUG_INTERFACE_DISABLE

	int ret;

	// Check Clock Service Subs UCG1 CH3 & CH15 (BPAM & RISC0_TCK_UCG)
	ucg_regs_t *serv_ucg = ucg_get_registers(UCG_SUBSYS_SERV, 0);
	uint32_t bpam_div = 0;
	uint32_t risc0_tck_div = 0;
	bool bpam_enable = false;
	bool risc0_tck_enable = false;

	ret = ucg_get_state(serv_ucg, SERVICE_UCG1_CHANNEL_CLK_BPAM, &bpam_div, &bpam_enable);
	if (ret)
		return ret;

	ret = ucg_get_state(serv_ucg, SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG, &risc0_tck_div,
	                    &risc0_tck_enable);
	if (ret)
		return ret;

	// Disable Clock Service Subs UCG1 CH3 & CH15
	if (bpam_enable || risc0_tck_enable) {
		uint32_t ucg_ena_mask = BIT(SERVICE_UCG1_CHANNEL_CLK_BPAM) |
		                        BIT(SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG);

		ret = ucg_enable_bp(serv_ucg, ucg_ena_mask);
		if (ret)
			return ret;

		ret = ucg_set_divider(serv_ucg, SERVICE_UCG1_CHANNEL_CLK_BPAM, bpam_div, 1000);
		if (ret)
			return ret;

		ret = ucg_set_divider(serv_ucg, SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG, risc0_tck_div,
		                      1000);
		if (ret)
			return ret;

		ret = ucg_sync_and_disable_bp(serv_ucg, ucg_ena_mask, ucg_ena_mask);
		if (ret)
			return ret;
	}

	// Disable Debugging in Service Subs URB Register
	service_urb_regs_t *service_urb = service_get_urb_registers();
	bool is_dbg_enable;

	is_dbg_enable = FIELD_GET(SERVICE_DBG_RISC0_ENABLE, service_urb->tp_dbgen);
	if (is_dbg_enable)
		service_urb->tp_dbgen = SERVICE_DBG_RISC0_DISABLE;

	is_dbg_enable = FIELD_GET(SERVICE_DBG_ALL_ENABLE, service_urb->sdr_dbgen);
	if (is_dbg_enable)
		service_urb->sdr_dbgen = SERVICE_DBG_SDR_DISABLE;

	is_dbg_enable = FIELD_GET(SERVICE_DBG_CPU_SP_ALL_ENABLE, service_urb->sp_dbgen);
	if (is_dbg_enable)
		service_urb->sp_dbgen = SERVICE_DBG_CPU_SP_DISABLE;

	is_dbg_enable = FIELD_GET(SERVICE_DBG_CPU_S_ALL_ENABLE, service_urb->s_dbgen);
	if (is_dbg_enable)
		service_urb->s_dbgen = SERVICE_DBG_CPU_S_DISABLE;

	is_dbg_enable = FIELD_GET(SERVICE_DBG_UST_ALL_ENABLE, service_urb->ust_dbgen);
	if (is_dbg_enable)
		service_urb->ust_dbgen = SERVICE_DBG_UST_DISABLE;

	// Check Clock HS Peripheral Subs UCG1 CH4
	ucg_regs_t *hsp_ucg = ucg_get_registers(UCG_SUBSYS_HSP, 1);
	uint32_t hsp_dbg_div = 0;
	bool hsp_dbg_enable = false;

	ret = ucg_get_state(hsp_ucg, HSP_UCG1_CHANNEL_CLK_DBG, &hsp_dbg_div, &hsp_dbg_enable);
	if (ret)
		return ret;

	// Disable Clock HS Peripheral Subs UCG1 CH4
	if (hsp_dbg_enable) {
		uint32_t ucg_ena_mask = BIT(HSP_UCG1_CHANNEL_CLK_DBG);

		ret = ucg_enable_bp(hsp_ucg, ucg_ena_mask);
		if (ret)
			return ret;

		ret = ucg_set_divider(hsp_ucg, HSP_UCG1_CHANNEL_CLK_DBG, hsp_dbg_div, 1000);
		if (ret)
			return ret;

		ret = ucg_sync_and_disable_bp(hsp_ucg, ucg_ena_mask, ucg_ena_mask);
		if (ret)
			return ret;
	}

	// Disable Debugging in HS Peripheral Subs URB Register
	hsp_urb_regs_t *hsp_urb = hsp_get_urb_registers();

	if (FIELD_GET(HSP_URB_DBG_CTR_MASK, hsp_urb->dbg_ctr))
		hsp_urb->dbg_ctr = 0;

#endif // DEBUG_INTERFACE_DISABLE

	return 0;
}
