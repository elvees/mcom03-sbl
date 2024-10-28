// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <string.h>
#include <platform_def.h>
#include <platform_helpers.h>
#include <mcom03.h>
#include "iommu.h"

#define IOMMU_TABLE_COUNT	(3)
#define IOMMU_TABLE_ENTRY_COUNT (512)
#define IOMMU_TABLE_ENTRY_SIZE	(sizeof(uint64_t))

#define IOMMU_TABLE_SIZE (IOMMU_TABLE_ENTRY_COUNT * IOMMU_TABLE_ENTRY_SIZE)
#define IOMMU_TOTAL_SIZE (IOMMU_TABLE_SIZE * IOMMU_TABLE_COUNT)

#define IOMMU_1GiB ULL(0x40000000)
#define IOMMU_2MiB ULL(0x00200000)

#define IOMMU_2MiB_MASK ULL(0x1FFFFF)

#define IOMMU_Ith_GiB(x) ((x)*IOMMU_1GiB)

#define IOMMU_1GiB_SLOT_IND (PLAT_IOMMU_VIRT_BASE_START / IOMMU_1GiB)
#define IOMMU_2MiB_SLOT_MIN \
	((PLAT_IOMMU_VIRT_BASE_START - IOMMU_1GiB_SLOT_IND * IOMMU_1GiB) / IOMMU_2MiB)
#define IOMMU_2MiB_SLOT_MAX (IOMMU_2MiB_SLOT_MIN + PLAT_IOMMU_VIRT_SLOT_COUNT)

/**
 * @brief Get pointer of struct of registers IOMMU
 *
 * @return pointer of struct of registers IOMMU
 */
iommu_t *iommu_get_registers(void)
{
	return (iommu_t *)BASE_ADDR_SERVICE_IOMMU;
}

static ptrdiff_t iommu_map_get_slot_size()
{
	return (ptrdiff_t)IOMMU_2MiB;
}

static void iommu_gen_default_tables(iommu_t *dev)
{
	iommu_pte_t *pte_p;

	memset(plat_convert_pa_to_va(dev->PTW_PBA_L), 0, IOMMU_TOTAL_SIZE);

	/* Fill 512GiB table */
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(dev->PTW_PBA_L);
	uint32_t addrTab1Gb = dev->PTW_PBA_L + IOMMU_TABLE_SIZE;
	pte_p[0] = (1ULL << IOMMU_PTE_V_POS) | (IOMMU_PTE_TYPE_REF << IOMMU_PTE_TYPE_POS) |
		   ((addrTab1Gb >> 12ULL) << IOMMU_PTE_PPN0_POS);

	/* Fill 1GiB table */
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab1Gb);
	uint32_t addrTab2Mb = dev->PTW_PBA_L + 2UL * IOMMU_TABLE_SIZE;
	for (int16_t i = 0; i < 4; ++i) {
		if (i != IOMMU_1GiB_SLOT_IND) {
			pte_p[i] = (1ULL << IOMMU_PTE_V_POS) |
				   (IOMMU_PTE_TYPE_RWX << IOMMU_PTE_TYPE_POS) |
				   ((IOMMU_Ith_GiB(i) >> 12ULL) << IOMMU_PTE_PPN0_POS);
		} else {
			pte_p[i] = (1ULL << IOMMU_PTE_V_POS) |
				   (IOMMU_PTE_TYPE_REF << IOMMU_PTE_TYPE_POS) |
				   ((addrTab2Mb >> 12ULL) << IOMMU_PTE_PPN0_POS);
		}
	}

	/* Fill 2MiB table */
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab2Mb);
	for (int16_t i = 0; i < IOMMU_TABLE_ENTRY_COUNT; ++i) {
		pte_p[i] = (1ULL << IOMMU_PTE_V_POS) | (IOMMU_PTE_TYPE_RWX << IOMMU_PTE_TYPE_POS) |
			   (((IOMMU_Ith_GiB(IOMMU_1GiB_SLOT_IND) + i * IOMMU_2MiB) >> 12ULL)
			    << IOMMU_PTE_PPN0_POS);
	}
}

static int16_t iommu_find_free_slot(iommu_t *dev)
{
	iommu_pte_t *pte_p;
	uint32_t addrTab2Mb = dev->PTW_PBA_L + 2UL * IOMMU_TABLE_SIZE;

	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab2Mb);
	for (int16_t i = IOMMU_2MiB_SLOT_MIN; i < IOMMU_2MiB_SLOT_MAX; ++i) {
		if (((pte_p[i] & IOMMU_PTE_USER_DEF_MASK) >> IOMMU_PTE_USER_DEF_POS) == 0ULL) {
			return i;
		}
	}

	return -1;
}

/**
 * @brief Initialize IOMMU
 *
 * @param dev                pointer of struct of registers IOMMU
 * @param ptw_base_addr_lo   low part physical address for translation table
 */
void iommu_init(iommu_t *dev, const void *ptw_base_addr)
{
	unsigned int reg_val = 0UL;

	/* Set address for xlat table */
	dev->PTW_PBA_L = plat_convert_va_to_pa(ptw_base_addr);
	dev->PTW_PBA_H = 0x00UL; /* Always Null */

	//TODO: check align
	iommu_gen_default_tables(dev);

	/* Enable translation */
	reg_val = dev->PTW_CFG;
	reg_val &= ~(IOMMU_PTW_CFG_A_PROT_MASK << IOMMU_PTW_CFG_A_PROT_OFFSET);
	reg_val |= (IOMMU_ENABLE_XLAT << IOMMU_PTW_CFG_STATUS_VM_OFFSET);
	dev->PTW_CFG = reg_val;

	/* Disable skipping translation for all channel TLB */
	for (unsigned int i = 0UL; i < IOMMU_TLB_MAX_NUM; i++) {
		dev->TLB_CTRL[i] = IOMMU_TLB_CTRL_ALL_BIT_RESET;
	}

	/* Check disabling range without conversion */
	if (dev->MAPSEG_ENABLE != 0UL) {
		dev->MAPSEG_ENABLE = 0UL;
	}

	/* Enable ARPROT/AWPROT bus AXI */
	reg_val = dev->PTW_CFG;
	reg_val |= ((IOMMU_PTW_CFG_A_PROT_DEFAULT) << IOMMU_PTW_CFG_A_PROT_OFFSET);
	dev->PTW_CFG = reg_val;
}

/**
 * @brief Deinitialize IOMMU (Set the default value in the registers)
 *
 * @param dev                pointer of struct of registers IOMMU
 * @param ptw_base_addr_lo   low part physical address for translation table
 */
void iommu_deinit(iommu_t *dev)
{
	unsigned int reg_val = 0UL;

	/* Disable translation */
	reg_val = dev->PTW_CFG;
	reg_val &= ~(IOMMU_PTW_CFG_A_PROT_MASK << IOMMU_PTW_CFG_A_PROT_OFFSET);
	reg_val &= ~(0xAUL << IOMMU_PTW_CFG_STATUS_VM_OFFSET);
	dev->PTW_CFG = reg_val;

	/* Disable skipping translation for all channel TLB */
	for (unsigned int i = 0UL; i < IOMMU_TLB_MAX_NUM; i++) {
		dev->TLB_CTRL[i] =
			(IOMMU_PASSTHROUGH_TLB_WRITE_CHANNEL | IOMMU_PASSTHROUGH_TLB_READ_CHANNEL |
			 IOMMU_PASSTHROUGH_TLB_EXEC_CHANNEL);
	}

	/* Reset address for xlat table */
	dev->PTW_PBA_L = 0x00UL;
	dev->PTW_PBA_H = 0x00UL; /* Always Null */

	/* Check disabling range without conversion */
	if (dev->MAPSEG_ENABLE != 0UL) {
		dev->MAPSEG_ENABLE = 0UL;
	}

	/* Enable ARPROT/AWPROT bus AXI */
	reg_val = dev->PTW_CFG;
	reg_val |= ((IOMMU_PTW_CFG_A_PROT_DEFAULT) << IOMMU_PTW_CFG_A_PROT_OFFSET);

	/* Invalidate cache */
	reg_val |= (0x1UL << IOMMU_PTW_CFG_INVALIDATE_OFFSET);
	dev->PTW_CFG = reg_val;
}

uintptr_t iommu_register_64bit_address(iommu_t *dev, uint64_t base_address64)
{
	iommu_pte_t *pte_p;

	int16_t slot = iommu_find_free_slot(dev);
	if (slot < 0)
		return (uintptr_t)NULL;

	/* Setup translation window for base_address64 */
	uint32_t addrTab2Mb = dev->PTW_PBA_L + 2UL * IOMMU_TABLE_SIZE;
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab2Mb);
	pte_p[slot] = 0ULL;
	pte_p[slot] = (1ULL << IOMMU_PTE_V_POS) | (IOMMU_PTE_TYPE_RWX << IOMMU_PTE_TYPE_POS) |
		      ((base_address64 >> 12ULL) << IOMMU_PTE_PPN0_POS);
	pte_p[slot] |= (1ULL << IOMMU_PTE_USER_DEF_POS);

	/* Invalidate cache */
	unsigned int reg_val = dev->PTW_CFG;
	reg_val |= (0x1UL << IOMMU_PTW_CFG_INVALIDATE_OFFSET);
	dev->PTW_CFG = reg_val;

	return (uintptr_t)(PLAT_IOMMU_VIRT_BASE_START + IOMMU_2MiB * slot);
}

int iommu_unregister_64bit_address(iommu_t *dev, uintptr_t base_address32)
{
	iommu_pte_t *pte_p;

	if ((base_address32 == 0ULL) || ((base_address32 & IOMMU_2MiB_MASK) != 0ULL)) {
		return -1;
	}

	int16_t slot = (base_address32 - PLAT_IOMMU_VIRT_BASE_START) / IOMMU_2MiB;

	if ((slot < IOMMU_2MiB_SLOT_MIN) || (slot >= IOMMU_2MiB_SLOT_MAX)) {
		return -1;
	}

	uint32_t addrTab2Mb = dev->PTW_PBA_L + 2UL * IOMMU_TABLE_SIZE;
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab2Mb);

	pte_p[slot] = 0ULL;
	pte_p[slot] = (1ULL << IOMMU_PTE_V_POS) | (IOMMU_PTE_TYPE_RWX << IOMMU_PTE_TYPE_POS) |
		      (((IOMMU_Ith_GiB(IOMMU_1GiB_SLOT_IND) + slot * IOMMU_2MiB) >> 12ULL)
		       << IOMMU_PTE_PPN0_POS);

	/* Invalidate cache */
	unsigned int reg_val = dev->PTW_CFG;
	reg_val |= (0x1UL << IOMMU_PTW_CFG_INVALIDATE_OFFSET);
	dev->PTW_CFG = reg_val;

	return 0;
}

int iommu_map_64bit_address(iommu_t *dev, uintptr_t desired_addr32, uint64_t base_address64)
{
	iommu_pte_t *pte_p;
	int16_t slot;

	/* Get pte */
	uint32_t addrTab2Mb = dev->PTW_PBA_L + 2UL * IOMMU_TABLE_SIZE;
	pte_p = (iommu_pte_t *)plat_convert_pa_to_va(addrTab2Mb);

	if ((desired_addr32 == 0ULL) || ((desired_addr32 & IOMMU_2MiB_MASK) != 0ULL)) {
		return -1;
	}

	slot = (desired_addr32 - PLAT_IOMMU_VIRT_BASE_START) / IOMMU_2MiB;
	if ((slot < IOMMU_2MiB_SLOT_MIN) || (slot >= IOMMU_2MiB_SLOT_MAX)) {
		return -1;
	}

	if (((pte_p[slot] & IOMMU_PTE_USER_DEF_MASK) >> IOMMU_PTE_USER_DEF_POS) != 0ULL) {
		return -1;
	}

	/* Setup translation window for base_address64 */
	pte_p[slot] = 0ULL;
	pte_p[slot] = (1ULL << IOMMU_PTE_V_POS) | (IOMMU_PTE_TYPE_RWX << IOMMU_PTE_TYPE_POS) |
		      ((base_address64 >> 12ULL) << IOMMU_PTE_PPN0_POS);
	pte_p[slot] |= (1ULL << IOMMU_PTE_USER_DEF_POS);

	/* Invalidate cache */
	unsigned int reg_val = dev->PTW_CFG;
	reg_val |= (0x1UL << IOMMU_PTW_CFG_INVALIDATE_OFFSET);
	dev->PTW_CFG = reg_val;

	return 0;
}

int iommu_map_range_64bit_address(iommu_t *dev, uintptr_t base32_start, ptrdiff_t base32_size,
				  uint64_t base64_start)
{
	uintptr_t p32;
	uint64_t p64;
	int ret;

	ptrdiff_t size = iommu_map_get_slot_size();

	for (p32 = base32_start, p64 = base64_start; p32 < (base32_start + base32_size);
	     p32 += size, p64 += size) {
		ret = iommu_map_64bit_address(dev, p32, p64);
		if (ret)
			return ret;
	}

	return 0;
}

void iommu_cache_invalidate(iommu_t *dev)
{
	/* Invalidate cache */
	unsigned int reg_val = dev->PTW_CFG;
	reg_val |= (0x1UL << IOMMU_PTW_CFG_INVALIDATE_OFFSET);
	dev->PTW_CFG = reg_val;
}
