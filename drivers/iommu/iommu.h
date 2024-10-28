// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __IOMMU_H__
#define __IOMMU_H__

#include <stdbool.h>
#include <stdint.h>
#include <lib/utils_def.h>

#define IOMMU_ENABLE_XLAT (0xAU)

#define IOMMU_TLB_WR_RISC (0)
#define IOMMU_TLB_RD_RISC (1)
#define IOMMU_TLB_WR_QDMA (2)
#define IOMMU_TLB_RD_QDMA (3)
#define IOMMU_TLB_MAX_NUM (4U)

#define IOMMU_IRQ_TLB0_MASK (0x01)
#define IOMMU_IRQ_TLB1_MASK (0x02)
#define IOMMU_IRQ_TLB2_MASK (0x04)
#define IOMMU_IRQ_TLB3_MASK (0x08)

#define IOMMU_TLB_ERROR_SUCCESS	  (0x00)
#define IOMMU_TLB_ERROR_INVALID	  (0x01)
#define IOMMU_TLB_ERROR_WR_PROT	  (0x02)
#define IOMMU_TLB_ERROR_RD_PROT	  (0x03)
#define IOMMU_TLB_ERROR_EXEC_PROT (0x04)

#define IOMMU_TLB_CTRL_MASK	     (0xFF)
#define IOMMU_TLB_CTRL_ALL_BIT_RESET (0)

#define IOMMU_PASSTHROUGH_TLB_WRITE_CHANNEL    (1 << 0) //!< TODO: Insert description
#define IOMMU_PASSTHROUGH_TLB_READ_CHANNEL     (1 << 1) //!< TODO: Insert description
#define IOMMU_PASSTHROUGH_TLB_EXEC_CHANNEL     (1 << 2) //!< TODO: Insert description
#define IOMMU_XPCT_RETRY_TLB_WRITE_CHANNEL     (1 << 3) //!< TODO: Insert description
#define IOMMU_XPCT_RETRY_TLB_READ_CHANNEL      (1 << 4) //!< TODO: Insert description
#define IOMMU_XPCT_DUMMY_TLB_WRITE_CHANNEL     (1 << 5) //!< TODO: Insert description
#define IOMMU_XPCT_DUMMY_TLB_READ_CHANNEL      (1 << 6) //!< TODO: Insert description
#define IOMMU_XPCT_AUTODUMMY_TLB_WRITE_CHANNEL (1 << 7) //!< TODO: Insert description
#define IOMMU_XPCT_AUTODUMMY_TLB_READ_CHANNEL  (1 << 8) //!< TODO: Insert description

#define IOMMU_PTW_CFG_INVALIDATE_OFFSET	 U(0) //!< Invalidation of PTW caches
#define IOMMU_PTW_CFG_STATUS_VM_OFFSET	 U(1) //!< Enabling address translation and permissions checks
#define IOMMU_PTW_CFG_STATUS_MPRV_OFFSET U(6) //!< Enabling use PRV1
#define IOMMU_PTW_CFG_STATUS_PRV1_OFFSET \
	U(7) //!< Settings permission level: 0x0-User level, 0x1-Supervisor level
#define IOMMU_PTW_CFG_STATUS_PRV_OFFSET \
	U(9) //!< Disabling address translation and permissions checks TODO: Don't understand from documentation!!!
#define IOMMU_PTW_CFG_STATUS_IRQ_OFFSET \
	U(11) //!< Status bits from TLBx [0]-TLB0, [1]-TLB1, [2]-TLB2, [3]-TLB3,
#define IOMMU_PTW_CFG_STATUS_IRQ_MASK U(0xF)
#define IOMMU_PTW_CFG_A_CACHE_OFFSET  U(15) //!< Attributes of ARCACHE/AWCACHE bus AXI
#define IOMMU_PTW_CFG_A_PROT_OFFSET   U(19) //!< Attributes of ARPROT/AWPROT bus AXI
#define IOMMU_PTW_CFG_A_PROT_DEFAULT  U(0x2)
#define IOMMU_PTW_CFG_A_PROT_MASK     U(0x7)
#define IOMMU_PTW_CFG_PREFETCH_OFFSET U(22) //!< Loading the next PTE with a standalone request
#define IOMMU_PTW_CFG_FETCHTWO_OFFSET U(23) //!< Loading two PTEs at once

#define IOMMU_PTE_V_POS		ULL(0)
#define IOMMU_PTE_V_MASK	(0b1ULL << IOMMU_PTE_V_POS)
#define IOMMU_PTE_TYPE_POS	ULL(1)
#define IOMMU_PTE_TYPE_MASK	(0b111ULL << IOMMU_PTE_TYPE_POS)
#define IOMMU_PTE_R_POS		ULL(5)
#define IOMMU_PTE_R_MASK	(0b1ULL << IOMMU_PTE_R_POS)
#define IOMMU_PTE_D_POS		ULL(6)
#define IOMMU_PTE_D_MASK	(0b1ULL << IOMMU_PTE_D_POS)
#define IOMMU_PTE_USER_DEF_POS	ULL(7)
#define IOMMU_PTE_USER_DEF_MASK (0b111ULL << IOMMU_PTE_USER_DEF_POS)
#define IOMMU_PTE_PPN0_POS	ULL(10)
#define IOMMU_PTE_PPN0_MASK	(0b111111111ULL << IOMMU_PTE_PPN0_POS)
#define IOMMU_PTE_PPN1_POS	ULL(19)
#define IOMMU_PTE_PPN1_MASK	(0b111111111ULL << IOMMU_PTE_PPN1_POS)
#define IOMMU_PTE_PPN2_POS	ULL(28)
#define IOMMU_PTE_PPN2_MASK	(0b111111111ULL << IOMMU_PTE_PPN2_POS)
#define IOMMU_PTE_PPN3_POS	ULL(37)
#define IOMMU_PTE_PPN3_MASK	(0b111111111111ULL << IOMMU_PTE_PPN3_POS)
#define IOMMU_PTE_RSVD_POS	ULL(46)
#define IOMMU_PTE_RSVD_MASK	(0b111111111111111111ULL << IOMMU_PTE_RSVD_POS)
#define IOMMU_PTE_TYPE_REF	ULL(0b1)
#define IOMMU_PTE_TYPE_RWX	ULL(0b111)

/**
 * @brief Struct of registers IOMMU
 *
 */
typedef struct {
	volatile unsigned int
		PTW_PBA_L; //!< The lower part of the physical address of the first level of the translation table
	volatile unsigned int
		PTW_PBA_H; //!< The highest part of the physical address of the first level of the translation table
	volatile unsigned int PTW_CFG; //!< Setting register for PTW and TLB modules
	volatile unsigned int TLBXCPT_NUM; //!< TLB number selection register
	volatile unsigned int TLBXCPT_ADDR; //!< TLB address that generated the exception
	volatile unsigned int TLBXCPT_TYPE; //!< TLB event type
	volatile unsigned int
		MAPSEG_START_L; //!< The lowest part of the start address of the memory without conversion
	volatile unsigned int
		MAPSEG_START_H; //!< The highest part of the start address of the memory without conversion
	volatile unsigned int
		MAPSEG_END_L; //!< The lowest part of the end address of the memory without conversion
	volatile unsigned int
		MAPSEG_END_H; //!< The lowest part of the end address of the memory without conversion
	volatile unsigned int MAPSEG_ENABLE; //!< Enabling address check in range without conversion
	volatile unsigned int RESERVED[5]; //!< Reserved
	volatile unsigned int TLB_CTRL[4]; //!< TLB control registers
} __attribute__((packed, __aligned__(4))) iommu_t;

typedef uint64_t iommu_pte_t;

iommu_t *iommu_get_registers(void);
void iommu_init(iommu_t *dev, const void *ptw_base_addr);
void iommu_deinit(iommu_t *dev);

/* those functions shall be used with critical section */
uintptr_t iommu_register_64bit_address(iommu_t *dev, uint64_t base_address64);
int iommu_unregister_64bit_address(iommu_t *dev, uintptr_t base_address32);
int iommu_map_64bit_address(iommu_t *dev, uintptr_t desired_addr32, uint64_t base_address64);
int iommu_map_range_64bit_address(iommu_t *dev, uintptr_t base32_start, ptrdiff_t base32_size,
				  uint64_t base64_start);
void iommu_cache_invalidate(iommu_t *dev);

#endif /* __IOMMU_H__ */
