// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

// Read C0 coprocessor register.
#define mips_read_c0_register(reg)                                             \
	({                                                                     \
		unsigned __value;                                              \
		__asm__ volatile("mfc0   %0, $%1" : "=r"(__value) : "K"(reg)); \
		__value;                                                       \
	})

// Write C0 coprocessor register.
#define mips_write_c0_register(reg, value)                                \
	do {                                                              \
		__asm__ volatile("mtc0   %z0, $%1 \n nop \n  nop \n  nop" \
		                 :                                        \
		                 : "r"((unsigned int)(value)), "K"(reg)); \
	} while (0)

// Coprocessor 0 registers.
#define C0_INDEX    0 // access index to TLB
#define C0_RANDOM   1 // TBL index for Write Random command
#define C0_ENTRYLO0 2 // strings for even TLB pages
#define C0_ENTRYLO1 3 // strings for odd TLB pages
#define C0_CONTEXT  4 // pointer to PTE table
#define C0_PAGEMASK 5 // TLB page size mask
#define C0_WIRED    6 // border of linked TLB strings
#define C0_BADVADDR 8 // last exception virtual address
#define C0_COUNT    9 // timer
#define C0_ENTRYHI  10 // virtual address compliance information
#define C0_COMPARE  11 // limit value for interrupt via timer
#define C0_STATUS   12 // processor functional modes
#define C0_CAUSE    13 // reason of the last exception
#define C0_EPC      14 // return address from exception
#define C0_PRID     15 // processor id
#define C0_CONFIG   16 // processor abilities info
#define C0_LLADDR   17 // phisical address of the last LL command
#define C0_ERROREPC 30 // return address from exception error

#define mips_get_core_id() 0

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
