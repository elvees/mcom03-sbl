// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <drivers/mcom03-regs.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/platform-def-common.h>
#include <libs/utils-def.h>

#include "iommu.h"

#define IOMMU_ENABLE_XLAT UL(0xA)

#define IOMMU_TLB_WR_RISC 0
#define IOMMU_TLB_RD_RISC 1
#define IOMMU_TLB_WR_QDMA 2
#define IOMMU_TLB_RD_QDMA 3
#define IOMMU_TLB_MAX_NUM 4

#define IOMMU_TLB_CTRL_MASK          GENMASK(8, 0)
#define IOMMU_TLB_CTRL_ALL_BIT_RESET 0

#define IOMMU_PASSTHROUGH_TLB_WRITE_CHANNEL    BIT(0)
#define IOMMU_PASSTHROUGH_TLB_READ_CHANNEL     BIT(1)
#define IOMMU_PASSTHROUGH_TLB_EXEC_CHANNEL     BIT(2)
#define IOMMU_XPCT_RETRY_TLB_WRITE_CHANNEL     BIT(3)
#define IOMMU_XPCT_RETRY_TLB_READ_CHANNEL      BIT(4)
#define IOMMU_XPCT_DUMMY_TLB_WRITE_CHANNEL     BIT(5)
#define IOMMU_XPCT_DUMMY_TLB_READ_CHANNEL      BIT(6)
#define IOMMU_XPCT_AUTODUMMY_TLB_WRITE_CHANNEL BIT(7)
#define IOMMU_XPCT_AUTODUMMY_TLB_READ_CHANNEL  BIT(8)

// Invalidation of PTW caches
#define IOMMU_PTW_CFG_INVALIDATE BIT(0)
// Enabling address translation and permissions checks
#define IOMMU_PTW_CFG_STATUS_VM BIT(1)
// Enabling use PRV1
#define IOMMU_PTW_CFG_STATUS_MPRV BIT(6)
// Settings permission level: 0x0-User level, 0x1-Supervisor level
#define IOMMU_PTW_CFG_STATUS_PRV1 BIT(7)
#define IOMMU_PTW_CFG_STATUS_PRV  BIT(9)
// Status bits from TLBx [0]-TLB0, [1]-TLB1, [2]-TLB2, [3]-TLB3
#define IOMMU_PTW_CFG_STATUS_IRQ GENMASK(14, 11)
// Attributes of ARCACHE/AWCACHE bus AXI
#define IOMMU_PTW_CFG_A_CACHE BIT(15)
// Attributes of ARPROT/AWPROT bus AXI
#define IOMMU_PTW_CFG_A_PROT_DEFAULT BIT(20)
#define IOMMU_PTW_CFG_A_PROT         GENMASK(21, 19)
// Loading the next PTE with a standalone request
#define IOMMU_PTW_CFG_PREFETCH BIT(22)
// Loading two PTEs at once
#define IOMMU_PTW_CFG_FETCHTWO BIT(23)

#define IOMMU_PTE_V        BIT64(0)
#define IOMMU_PTE_TYPE_REF BIT64(0)
#define IOMMU_PTE_TYPE_RWX GENMASK64(2, 0)
#define IOMMU_PTE_TYPE     GENMASK64(4, 1)
#define IOMMU_PTE_R        BIT64(5)
#define IOMMU_PTE_D        BIT64(6)
#define IOMMU_PTE_USER_DEF GENMASK64(9, 7)
#define IOMMU_PTE_PPN0     GENMASK64(18, 10)
#define IOMMU_PTE_PPN1     GENMASK64(27, 19)
#define IOMMU_PTE_PPN2     GENMASK64(36, 28)
#define IOMMU_PTE_PPN3     GENMASK64(45, 37)
#define IOMMU_PTE_PPNS     GENMASK64(63, 10)
#define IOMMU_PTE_RSVD     GENMASK64(63, 46)

#define IOMMU_TABLE_COUNT       3
#define IOMMU_TABLE_ENTRY_COUNT 512
#define IOMMU_TABLE_ENTRY_SIZE  (sizeof(uint64_t))

#define IOMMU_TABLE_SIZE (IOMMU_TABLE_ENTRY_COUNT * IOMMU_TABLE_ENTRY_SIZE)
#define IOMMU_TOTAL_SIZE (IOMMU_TABLE_SIZE * IOMMU_TABLE_COUNT)

#define IOMMU_4_KIB             BIT64(12)
#define IOMMU_4_KIB_OFFSET_MASK (IOMMU_4_KIB - 1ULL)
#define IOMMU_4_KIB_ADDR_MASK   (~IOMMU_4_KIB_OFFSET_MASK)

#define IOMMU_2_MIB             BIT64(21)
#define IOMMU_2_MIB_OFFSET_MASK (IOMMU_2_MIB - 1ULL)
#define IOMMU_2_MIB_ADDR_MASK   (~IOMMU_2_MIB_OFFSET_MASK)

#define IOMMU_1_GIB             BIT64(30)
#define IOMMU_1_GIB_OFFSET_MASK (IOMMU_1_GIB - 1ULL)
#define IOMMU_1_GIB_ADDR_MASK   (~IOMMU_1_GIB_OFFSET_MASK)

#define IOMMU_GIB(x) (IOMMU_1_GIB * (x))
#define SHR_12(x)    ((x) >> 12)

#define IOMMU_1_GIB_SLOT_IND (PLAT_IOMMU_VIRT_BASE_START / IOMMU_1_GIB)
#define IOMMU_2_MIB_SLOT_MIN \
	((PLAT_IOMMU_VIRT_BASE_START - IOMMU_1_GIB_SLOT_IND * IOMMU_1_GIB) / IOMMU_2_MIB)
#define IOMMU_2_MIB_SLOT_MAX (IOMMU_2_MIB_SLOT_MIN + PLAT_IOMMU_VIRT_SLOT_COUNT)

iommu_regs_t *iommu_get_registers(void)
{
	return (iommu_regs_t *)BASE_ADDR_SERVICE_IOMMU;
}

static void iommu_gen_default_tables(iommu_regs_t *dev)
{
	iommu_pte_t *pte_p;

	memset(convert_pa_to_va(dev->ptw_pba_l), 0, IOMMU_TOTAL_SIZE);

	// Fill 512GiB table
	pte_p = (iommu_pte_t *)convert_pa_to_va(dev->ptw_pba_l);
	uint32_t table_1gb = dev->ptw_pba_l + IOMMU_TABLE_SIZE;
	pte_p[0] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_REF) |
	           FIELD_PREP(IOMMU_PTE_PPNS, SHR_12(table_1gb));

	// Fill 1GiB table
	pte_p = (iommu_pte_t *)convert_pa_to_va(table_1gb);
	uint32_t table_2mb = dev->ptw_pba_l + 2 * IOMMU_TABLE_SIZE;
	for (int16_t i = 0; i < 4; ++i)
		if (i != IOMMU_1_GIB_SLOT_IND)
			pte_p[i] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_RWX) |
			           FIELD_PREP(IOMMU_PTE_PPNS, SHR_12(IOMMU_GIB(i)));
		else
			pte_p[i] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_REF) |
			           FIELD_PREP(IOMMU_PTE_PPNS, SHR_12(table_2mb));

	// Fill 2MiB table
	pte_p = (iommu_pte_t *)convert_pa_to_va(table_2mb);
	for (int16_t i = 0; i < IOMMU_TABLE_ENTRY_COUNT; ++i)
		pte_p[i] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_RWX) |
		           FIELD_PREP(IOMMU_PTE_PPNS,
		                      SHR_12(IOMMU_GIB(IOMMU_1_GIB_SLOT_IND) + i * IOMMU_2_MIB));
}

static int16_t iommu_find_free_slot(iommu_regs_t *dev)
{
	iommu_pte_t *pte_p;
	uint32_t table_2mb = dev->ptw_pba_l + 2 * IOMMU_TABLE_SIZE;

	pte_p = (iommu_pte_t *)convert_pa_to_va(table_2mb);
	for (int16_t i = IOMMU_2_MIB_SLOT_MIN; i < IOMMU_2_MIB_SLOT_MAX; ++i)
		if (!FIELD_GET(IOMMU_PTE_USER_DEF, pte_p[i]))
			return i;

	return -EDATASIZE;
}

static uintptr_t __iommu_map_64(iommu_regs_t *dev, uint64_t base_address64)
{
	iommu_pte_t *pte_p;

	int16_t slot = iommu_find_free_slot(dev);
	if (slot < 0)
		return (uintptr_t)NULL;

	// Setup translation window for base_address64
	uint32_t table_2mb = dev->ptw_pba_l + 2 * IOMMU_TABLE_SIZE;
	pte_p = (iommu_pte_t *)convert_pa_to_va(table_2mb);
	pte_p[slot] = 0;
	pte_p[slot] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_RWX) |
	              FIELD_PREP(IOMMU_PTE_PPNS, SHR_12(base_address64));
	pte_p[slot] |= FIELD_PREP(IOMMU_PTE_USER_DEF, 1);

	// Invalidate cache
	iommu_cache_invalidate(dev);

	return (uintptr_t)(PLAT_IOMMU_VIRT_BASE_START + IOMMU_2_MIB * slot);
}

static int __iommu_unmap_64(iommu_regs_t *dev, uintptr_t base_address32)
{
	iommu_pte_t *pte_p;

	if (!base_address32 || (base_address32 & IOMMU_2_MIB_OFFSET_MASK))
		return -EINVALIDPARAM;

	int16_t slot = (base_address32 - PLAT_IOMMU_VIRT_BASE_START) / IOMMU_2_MIB;

	if ((slot < IOMMU_2_MIB_SLOT_MIN) || (slot >= IOMMU_2_MIB_SLOT_MAX))
		return -EDATASIZE;

	uint32_t table_2mb = dev->ptw_pba_l + 2 * IOMMU_TABLE_SIZE;
	pte_p = (iommu_pte_t *)convert_pa_to_va(table_2mb);

	pte_p[slot] = 0;
	pte_p[slot] = IOMMU_PTE_V | FIELD_PREP(IOMMU_PTE_TYPE, IOMMU_PTE_TYPE_RWX) |
	              FIELD_PREP(IOMMU_PTE_PPNS,
	                         SHR_12(IOMMU_GIB(IOMMU_1_GIB_SLOT_IND) + slot * IOMMU_2_MIB));

	// Invalidate cache
	iommu_cache_invalidate(dev);

	return 0;
}

void iommu_init(iommu_regs_t *dev, const void *ptw_base_addr)
{
	unsigned int reg_val = 0;

	// Set address for xlat table
	dev->ptw_pba_l = convert_va_to_pa(ptw_base_addr);
	dev->ptw_pba_h = 0x00; // Always Null

	// TODO: check align
	iommu_gen_default_tables(dev);

	// Enable translation
	reg_val = dev->ptw_cfg;
	reg_val &= ~IOMMU_PTW_CFG_A_PROT;
	reg_val |= FIELD_PREP(GENMASK(5, 1), IOMMU_ENABLE_XLAT);
	dev->ptw_cfg = reg_val;

	// Disable skipping translation for all channel TLB
	for (int i = 0; i < IOMMU_TLB_MAX_NUM; i++)
		dev->tlb_ctrl[i] = IOMMU_TLB_CTRL_ALL_BIT_RESET;

	// Check disabling range without conversion
	if (dev->mapseg_enable)
		dev->mapseg_enable = 0;

	// Enable ARPROT/AWPROT bus AXI
	reg_val = dev->ptw_cfg;
	reg_val |= IOMMU_PTW_CFG_A_PROT_DEFAULT;
	dev->ptw_cfg = reg_val;
}

void iommu_reset(iommu_regs_t *dev)
{
	unsigned int reg_val = 0;

	// Disable translation
	reg_val = dev->ptw_cfg;
	reg_val &= ~IOMMU_PTW_CFG_A_PROT;
	reg_val &= ~FIELD_PREP(GENMASK(5, 1), IOMMU_ENABLE_XLAT);
	dev->ptw_cfg = reg_val;

	// Disable skipping translation for all channel TLB
	for (int i = 0; i < IOMMU_TLB_MAX_NUM; i++)
		dev->tlb_ctrl[i] =
			(IOMMU_PASSTHROUGH_TLB_WRITE_CHANNEL | IOMMU_PASSTHROUGH_TLB_READ_CHANNEL |
		         IOMMU_PASSTHROUGH_TLB_EXEC_CHANNEL);

	// Reset address for xlat table
	dev->ptw_pba_l = 0x00;
	dev->ptw_pba_h = 0x00; // Always Null

	// Check disabling range without conversion
	if (dev->mapseg_enable)
		dev->mapseg_enable = 0;

	// Enable ARPROT/AWPROT bus AXI
	reg_val = dev->ptw_cfg;
	reg_val |= IOMMU_PTW_CFG_A_PROT_DEFAULT;
	dev->ptw_cfg = reg_val;

	// Invalidate cache
	iommu_cache_invalidate(dev);
}

uintptr_t iommu_map(iommu_regs_t *dev, uint64_t addr64)
{
	uintptr_t iobase = (uintptr_t)NULL;
	uintptr_t offset = (uintptr_t)NULL;

	iobase = __iommu_map_64(dev, (addr64 & IOMMU_4_KIB_ADDR_MASK));
	if (!iobase)
		panic_handler("iommu failed to register 64bit addr\r\n");

	offset = addr64 & IOMMU_4_KIB_OFFSET_MASK;

	return (uintptr_t)(iobase + offset);
}

void iommu_unmap(iommu_regs_t *dev, uintptr_t addr32)
{
	uintptr_t iobase = (uintptr_t)(addr32 & IOMMU_2_MIB_ADDR_MASK);

	if (__iommu_unmap_64(dev, iobase))
		panic_handler("iommu failed to unregister 64bit addr\r\n");
}

void iommu_cache_invalidate(iommu_regs_t *dev)
{
	// Invalidate cache
	unsigned int reg_val = dev->ptw_cfg;
	reg_val |= IOMMU_PTW_CFG_INVALIDATE;
	dev->ptw_cfg = reg_val;
}
