// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/utils-def.h>

// Read CP0 coprocessor register.
#define mips_read_cp0_register(reg)                                            \
	({                                                                     \
		unsigned __value;                                              \
		__asm__ volatile("mfc0   %0, $%1" : "=r"(__value) : "K"(reg)); \
		__value;                                                       \
	})

// Write CP0 coprocessor register.
#define mips_write_cp0_register(reg, value)                               \
	do {                                                              \
		__asm__ volatile("mtc0   %z0, $%1 \n nop \n  nop \n  nop" \
		                 :                                        \
		                 : "r"((unsigned int)(value)), "K"(reg)); \
	} while (0)

// Coprocessor 0 registers.
#define CP0_INDEX    0 // access index to TLB
#define CP0_RANDOM   1 // TBL index for Write Random command
#define CP0_ENTRYLO0 2 // strings for even TLB pages
#define CP0_ENTRYLO1 3 // strings for odd TLB pages
#define CP0_CONTEXT  4 // pointer to PTE table
#define CP0_PAGEMASK 5 // TLB page size mask
#define CP0_WIRED    6 // border of linked TLB strings
#define CP0_BADVADDR 8 // last exception virtual address
#define CP0_COUNT    9 // timer
#define CP0_ENTRYHI  10 // virtual address compliance information
#define CP0_COMPARE  11 // limit value for interrupt via timer
#define CP0_STATUS   12 // processor functional modes
#define CP0_CAUSE    13 // reason of the last exception
#define CP0_EPC      14 // return address from exception
#define CP0_PRID     15 // processor id
#define CP0_CONFIG   16 // processor abilities info
#define CP0_LLADDR   17 // physical address of the last LL command
#define CP0_ERROREPC 30 // return address from exception error

#define MIPS_CP0_CR_QLIC0_TARGS     GENMASK(13, 10)
#define MIPS_CP0_CR_CNT_CMP_TARG_IP BIT(14)

#define MIPS_CP0_SR_IE              BIT(0)
#define MIPS_CP0_SR_IM2_QLIC0_TARG0 BIT(10)
#define MIPS_CP0_SR_IM3_QLIC0_TARG1 BIT(11)
#define MIPS_CP0_SR_IM4_QLIC0_TARG2 BIT(12)
#define MIPS_CP0_SR_IM5_QLIC0_TARG3 BIT(13)
#define MIPS_CP0_SR_IM6_CNT_CMP     BIT(14)

// MIPS interrupt ID
#define MIPS_ID_CP0_CNT_CMP 0

/**
 * @brief Enables all MIPS global interrupts
 */
void mips_global_irq_enable(void);

/**
 * @brief Disables all MIPS global interrupts
 */
void mips_global_irq_disable(void);

/**
 * @brief The function enables a specific external MIPS interrupt
 */
void mips_enable_irq_target(unsigned int target);

/**
 * @brief Disables a specific external MIPS interrupt.
 */
void mips_disable_irq_target(unsigned int target);

/**
 * @brief Gets info if a specific external MIPS interrupt is enabled
 */
int mips_is_irq_target_enabled(unsigned int target);
