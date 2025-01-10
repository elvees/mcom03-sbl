// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <assert.h>

#include "utils-def.h"

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
#define CP0_LLADDR   17 // phisical address of the last LL command
#define CP0_ERROREPC 30 // return address from exception error

// EXCEPTION TABLE
#define EXC_INT  0 // Int  Interruption
#define EXC_MOD  1 // Mod  TLB-exception of modification
#define EXC_TLBL 2 // TLBL TLB-exception (loading or command call)
#define EXC_TLBS 3 // TLBS TLB-exception (saving)
#define EXC_ADEL 4 // AdEL Interruption due to an addressing error (loading or command call)
#define EXC_ADES 5 // AdES Interruption due to an addressing error (saving)
#define EXC_SYS  8 // Sys  System exception
#define EXC_BP   9 // Bp   Breakpoint exception
#define EXC_RI   10 // RI   Reserved command exception
#define EXC_CPU  11 // CpU  Coprocessort unavailability exception
#define EXC_OV   12 // Ov   Integer overflow exception
#define EXC_TR   13 // Tr   Trap exception

#define MIPS_CP0_CR_QLIC0_TARGET_IP GENMASK(14, 10)

#define MIPS_CP0_SR_IE                BIT(0)
#define MIPS_CP0_SR_IM2_QLIC0_TARG0   BIT(10)
#define MIPS_CP0_SR_IM3_QLIC0_TARG1   BIT(11)
#define MIPS_CP0_SR_IM4_QLIC0_TARG2   BIT(12)
#define MIPS_CP0_SR_IM5_QLIC0_TARG3   BIT(13)
#define MIPS_CP0_SR_IM6_COUNT_COMPARE BIT(14)

/**
 * @brief Enables all MIPS global interrupts
 */
static inline void mips_global_irq_enable(void)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status |= MIPS_CP0_SR_IE;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

/**
 * @brief Disables all MIPS global interrupts
 */
static inline void mips_global_irq_disable(void)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status &= ~MIPS_CP0_SR_IE;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

/**
 * @brief The function enables a specific external MIPS interrupt
 */
static inline void mips_enable_irq_target(unsigned int target)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status |= target;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

/**
 * @brief Disables a specific external MIPS interrupt.
 */
static inline void mips_disable_irq_target(unsigned int target)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status &= ~target;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

/**
 * @brief Gets info if a specific external MIPS interrupt is enabled
 */
static inline int mips_is_irq_target_enabled(unsigned int target)
{
	return mips_read_cp0_register(CP0_STATUS) & target;
}

/**
 * @brief Gets current IRQ target. Function is called in ISR
 *
 * @return Current IRQ target
 */
static inline int mips_get_irq_target(void)
{
	unsigned int cp0_cause = mips_read_cp0_register(CP0_CAUSE); // get CP0 cause
	// get HW IRQ pending requests
	unsigned int pending_irq = FIELD_GET(MIPS_CP0_CR_QLIC0_TARGET_IP, cp0_cause);
	// select HW IRQ pending requests with highest priority
	unsigned int ls_bit = ((pending_irq) & (-pending_irq));
	assert(ls_bit > 0);

	return __builtin_ctz(ls_bit); // return current target number
}
