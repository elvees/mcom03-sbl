// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stdint.h>

#include <drivers/ddr/ddr.h>
#include <drivers/hs-periph/hs-periph.h>
#include <drivers/iommu/iommu.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/service/service.h>
#include <drivers/timer/timer.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "helpers.h"

#define SECURE_REGIONS_PHYS_ADDR_START 0x880000000ULL
#define SECURE_REGIONS_PHYS_ADDR_SIZE  0x10000000ULL
#define SECURE_REGIONS_PHYS_ADDR_MASK  (~(SECURE_REGIONS_PHYS_ADDR_SIZE - 1))
#define SECURE_REGIONS_PHYS_ADDR_END \
	(SECURE_REGIONS_PHYS_ADDR_START + SECURE_REGIONS_PHYS_ADDR_SIZE)

static int _set_ppolicy(uintptr_t reg, uint32_t new_policy, uint32_t timeout_us)
{
	uint32_t val;

	mmio_write_32(reg, new_policy);

	return read_poll_timeout(mmio_read_32, val, (val & PP_MASK) == new_policy, 1000, timeout_us,
	                         reg + 0x4);
}

int set_ppolicy(uintptr_t reg, uint32_t new_policy, uint32_t bp0_mask, uint32_t bp1_mask,
                uint32_t timeout_us, bool is_delay_required)
{
	ucg_regs_t *interconnect_ucg0 = ucg_get_registers(UCG_SUBSYS_TOP, 0);
	ucg_regs_t *interconnect_ucg1 = ucg_get_registers(UCG_SUBSYS_TOP, 1);
	int ret;

	if ((mmio_read_32(reg) & PP_MASK) == new_policy)
		return 0;

	if (bp0_mask)
		ucg_enable_bp(interconnect_ucg0, bp0_mask);

	if (bp1_mask)
		ucg_enable_bp(interconnect_ucg1, bp1_mask);

	ret = _set_ppolicy(reg, new_policy, timeout_us);
	if (ret && new_policy == PP_ON)
		_set_ppolicy(reg, PP_OFF, timeout_us);

	/* For subsusytem with power management OFFREQN and OFFACKN pads usually connected to
	 * ENABLE and POWER_GOOD of external power regulator respectively.
	 * SoC will set high level to ENABLE of regulator when PP_ON is written to PPOLICY register.
	 * When power regulator set POWER_GOOD then PP_ON will be read from PSTATUS register.
	 * Add delay for case when power regulator set POWER_GOOD too early.
	 */
	if (is_delay_required)
		timer_delay_us(20000);

	if (bp0_mask)
		ucg_sync_and_disable_bp(interconnect_ucg0, bp0_mask, bp0_mask);

	if (bp1_mask)
		ucg_sync_and_disable_bp(interconnect_ucg1, bp1_mask, bp1_mask);

	return ret;
}

int setup_ddr_regions(void)
{
	int ret;

	/**
	 * Note that ddrinit also maps the following memory range:
	 * +----------------------------+-------------------------+
	 * | 64Bit phys addrs           |   32bit virt addrs      |
	 * +----------------------------+-------------------------+
	 * | 0x890400000 - 0x8905FFFFF  | 0xC0000000 - 0xC01FFFFF |
	 * +----------------------------+-------------------------+
	 */
	iommu_regs_t *iommu_regs = iommu_get_registers();
	ret = iommu_map_range(iommu_regs, DDR_MAP_TO_DDRHIGH_VIRT, DDR_MAP_TO_DDRHIGH_SIZE,
	                      DDR_MAP_TO_DDRHIGH_PHYS);
	if (ret)
		return ret;

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
	struct ddrinfo *info = (struct ddrinfo *)DDR_MEM_REGIONS_VIRT_ADDR;
	for (int i = 0; i < DDR_MAX_MEM_REGIONS; ++i) {
		uint64_t start = info->mem_regions[i].start;
		uint64_t end = info->mem_regions[i].start + info->mem_regions[i].size;
		if ((SECURE_REGIONS_PHYS_ADDR_START >= start) &&
		    (SECURE_REGIONS_PHYS_ADDR_END <= end)) {
			info->mem_regions[i].start = SECURE_REGIONS_PHYS_ADDR_END;
			info->mem_regions[i].size = end - SECURE_REGIONS_PHYS_ADDR_END;
		}
	}

	return 0;
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
