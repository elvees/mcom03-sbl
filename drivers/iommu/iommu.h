// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stddef.h>
#include <stdint.h>

#include <libs/utils-def.h>

// Struct of registers IOMMU
typedef struct {
	// The lower part of the physical address of the first level of the translation table
	volatile unsigned int ptw_pba_l;
	// The highest part of the physical address of the first level of the translation table
	volatile unsigned int ptw_pba_h;
	// Setting register for PTW and TLB modules
	volatile unsigned int ptw_cfg;
	// TLB number selection register
	volatile unsigned int tlbxcpt_num;
	// TLB address that generated the exception
	volatile unsigned int tlbxcpt_addr;
	// TLB event type
	volatile unsigned int tlbxcpt_type;
	// The lowest part of the start address of the memory without conversion
	volatile unsigned int mapseg_start_l;
	// The highest part of the start address of the memory without conversion
	volatile unsigned int mapseg_start_h;
	// The lowest part of the end address of the memory without conversion
	volatile unsigned int mapseg_end_l;
	// The lowest part of the end address of the memory without conversion
	volatile unsigned int mapseg_end_h;
	// Enabling address check in range without conversion
	volatile unsigned int mapseg_enable;
	// Reserved
	volatile unsigned int reserved[5];
	// TLB control registers
	volatile unsigned int tlb_ctrl[4];
} __attribute__((packed, __aligned__(4))) iommu_regs_t;

typedef uint64_t iommu_pte_t;

/**
 * @brief Gets pointer of IOMMU registers struct
 *
 * @return Pointer of IOMMU registers struct
 */
iommu_regs_t *iommu_get_registers(void);

/**
 * @brief Initializes IOMMU
 *
 * @param dev           - Pointer of IOMMU registers struct
 * @param ptw_base_addr - Low part physical address for translation table
 */
void iommu_init(iommu_regs_t *dev, const void *ptw_base_addr);

/**
 * @brief Deinitializes IOMMU (Set the default value in the registers)
 *
 * @param dev - Pointer of IOMMU registers struct
 */
void iommu_reset(iommu_regs_t *dev);

// Those functions shall be used with critical section
int iommu_map_32to64(iommu_regs_t *dev, uintptr_t addr32_aligned, uint64_t addr64_aligned);
int iommu_map_32to64_range(iommu_regs_t *dev, uintptr_t addr32_aligned,
                           ptrdiff_t addr32_aligned_size, uint64_t addr64_aligned);
void iommu_cache_invalidate(iommu_regs_t *dev);
