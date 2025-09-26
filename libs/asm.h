// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include "platform-def-common.h"

#define zero 0
#define AT   1
#define v0   2
#define v1   3
#define a0   4
#define a1   5
#define a2   6
#define a3   7
#define t0   8
#define t1   9
#define t2   10
#define t3   11
#define t4   12
#define t5   13
#define t6   14
#define t7   15
#define s0   16
#define s1   17
#define s2   18
#define s3   19
#define s4   20
#define s5   21
#define s6   22
#define s7   23
#define t8   24
#define t9   25
#define k0   26
#define k1   27
#define gp   28
#define sp   29
#define fp   30 // s8
#define ra   31

#ifdef __ASSEMBLER__
#define LEAF_WEAK(a)         \
	.weak a;             \
	.type a, % function; \
a:;
#define LEAF(a)              \
	.global a;           \
	.type a, % function; \
a:;
#define END(a) ;
#define ENTRY(a)             \
	.type a, % function; \
a:;

	.set  CONTEXT_R0,                       zero
	.set  CONTEXT_R1,                       AT
	.set  CONTEXT_R2,                       v0
	.set  CONTEXT_R3,                       v1
	.set  CONTEXT_R4,                       a0
	.set  CONTEXT_R5,                       a1
	.set  CONTEXT_R6,                       a2
	.set  CONTEXT_R7,                       a3
	.set  CONTEXT_R8,                       t0
	.set  CONTEXT_R9,                       t1
	.set  CONTEXT_R10,                      t2
	.set  CONTEXT_R11,                      t3
	.set  CONTEXT_R12,                      t4
	.set  CONTEXT_R13,                      t5
	.set  CONTEXT_R14,                      t6
	.set  CONTEXT_R15,                      t7
	.set  CONTEXT_R16,                      s0
	.set  CONTEXT_R17,                      s1
	.set  CONTEXT_R18,                      s2
	.set  CONTEXT_R19,                      s3
	.set  CONTEXT_R20,                      s4
	.set  CONTEXT_R21,                      s5
	.set  CONTEXT_R22,                      s6
	.set  CONTEXT_R23,                      s7
	.set  CONTEXT_R24,                      t8
	.set  CONTEXT_R25,                      t9
	.set  CONTEXT_K0,                       k0
	.set  CONTEXT_K1,                       k1
	.set  CONTEXT_GP,                       gp
	.set  CONTEXT_SP,                       sp
	.set  CONTEXT_FP,                       fp
	.set  CONTEXT_RA,                       ra
	.set  CONTEXT_LO,                       32
	.set  CONTEXT_HI,                       33
	.set  CONTEXT_STATUS,                   34
	.set  CONTEXT_PC,                       35

	.set CONTEXT_WORDS,                     36

	.set $CP0_INDEX,                        $0
	.set $CP0_RANDOM,                       $1
	.set $CP0_ENTRYLO0,                     $2
	.set $CP0_ENTRYLO1,                     $3
	.set $CP0_CONTEXT,                      $4
	.set $CP0_PAGEMASK,                     $5
	.set $CP0_WIRED,                        $6
	.set $CP0_BADVADDR,                     $8
	.set $CP0_COUNT,                        $9
	.set $CP0_ENTRYHI,                      $10
	.set $CP0_COMPARE,                      $11
	.set $CP0_STATUS,                       $12
	.set EN_IRQ_ONE,                        0x1
	.set EN_IRQ_ZERO,                       0xFFFFFFFE
	.set ERRORLVL_MASK,                     0x6
	.set USER_MODE_MASK,                    0x10
	.set NMI_IRQ,                           0x80000
	.set IP_MASK,                           0xFC00
	.set IP_MASK_OFFSET,                    0xA
	.set EXC_MASK,                          0x7C
	.set EXC_MASK_OFFSET,                   0x2
	.set EXC_SYS,                           0x8
	.set EN_FPU_CP1,                        0x20000000
	.set $CP0_CAUSE,                        $13
	.set IV_ONE,                            0x800000
	.set IV_ZERO,                           0xFF7FFFFF
	.set $CP0_EPC,                          $14
	.set $CP0_PRID,                         $15
	.set $CP0_CONFIG,                       $16
	.set K0_CACHE_MASK,                     0x00000007
	.set K0_CACHE_DISABLE,                  0x00000002
	.set K0_CACHE_ENABLE,                   0x00000003
	.set K23_CACHE_MASK,                    0x70000000
	.set K23_CACHE_DISABLE,                 0x20000000
	.set K23_CACHE_ENABLE,                  0x30000000
	.set KU_CACHE_MASK,                     0x0E000000
	.set KU_CACHE_DISABLE,                  0x04000000
	.set KU_CACHE_ENABLE,                   0x06000000
	.set $CP0_LLADDR,                       $17
	.set $CP0_ERROREPC,                     $30

	.set FLUSH_I,                           0x1000
	.set FLUSH_D,                           0x4000
	.set TR_CRAM,                           0x2
	.set TR_CRAM_MASK,                      0xFFFFFFFD

/**
 * This macro is used to save registers into stack
 */
.macro SAVE_REGS

	addi	$sp, -CONTEXT_WORDS*4		// Allocate space for registers

	.set	push
	.set	noat
	sw	$AT, (CONTEXT_R1 * 4) ($sp)	// Save general registers
	sw	$v0, (CONTEXT_R2 * 4) ($sp)
	sw	$v1, (CONTEXT_R3 * 4) ($sp)
	sw	$a0, (CONTEXT_R4 * 4) ($sp)
	sw	$a1, (CONTEXT_R5 * 4) ($sp)
	sw	$a2, (CONTEXT_R6 * 4) ($sp)
	sw	$a3, (CONTEXT_R7 * 4) ($sp)
	sw	$t0, (CONTEXT_R8 * 4) ($sp)
	sw	$t1, (CONTEXT_R9 * 4) ($sp)
	sw	$t2, (CONTEXT_R10 * 4) ($sp)
	sw	$t3, (CONTEXT_R11 * 4) ($sp)
	sw	$t4, (CONTEXT_R12 * 4) ($sp)
	sw	$t5, (CONTEXT_R13 * 4) ($sp)
	sw	$t6, (CONTEXT_R14 * 4) ($sp)
	sw	$t7, (CONTEXT_R15 * 4) ($sp)
	sw	$s0, (CONTEXT_R16 * 4) ($sp)
	sw	$s1, (CONTEXT_R17 * 4) ($sp)
	sw	$s2, (CONTEXT_R18 * 4) ($sp)
	sw	$s3, (CONTEXT_R19 * 4) ($sp)
	sw	$s4, (CONTEXT_R20 * 4) ($sp)
	sw	$s5, (CONTEXT_R21 * 4) ($sp)
	sw	$s6, (CONTEXT_R22 * 4) ($sp)
	sw	$s7, (CONTEXT_R23 * 4) ($sp)
	sw	$t8, (CONTEXT_R24 * 4) ($sp)
	sw	$t9, (CONTEXT_R25 * 4) ($sp)
// Skip $26 - K0
// Skip $27 - K1
	sw	$gp, (CONTEXT_GP * 4) ($sp)
// Skip $29 - SP
	sw	$fp, (CONTEXT_FP * 4) ($sp)
	sw	$ra, (CONTEXT_RA * 4) ($sp)
	.set	pop

	mfhi	$k0				// Save special registers
	sw	$k0, (CONTEXT_HI * 4) ($sp)

	mflo	$k0
	sw	$k0, (CONTEXT_LO * 4) ($sp)

	mfc0	$k0, $CP0_EPC
	sw	$k0, (CONTEXT_PC * 4) ($sp)

	mfc0	$k0, $CP0_STATUS
	sw	$k0, (CONTEXT_STATUS * 4) ($sp)
.endm

/**
 * This macro is used to restore registers saved in stack
 */
.macro RESTORE_REGS
	lw	$k0, (CONTEXT_LO * 4) ($sp)	// Load HI, LO registers
	mtlo	$k0
	lw	$k0, (CONTEXT_HI * 4) ($sp)
	mthi	$k0

	lw	$k0, (CONTEXT_PC * 4) ($sp)	// K1 = EPC
	mtc0	$k0, $CP0_EPC			// put PC in EPC

	.set	push
	.set	noat
	lw	$AT, (CONTEXT_R1 * 4) ($sp)	// Load general registers
	lw	$v0, (CONTEXT_R2 * 4) ($sp)
	lw	$v1, (CONTEXT_R3 * 4) ($sp)
	lw	$a0, (CONTEXT_R4 * 4) ($sp)
	lw	$a1, (CONTEXT_R5 * 4) ($sp)
	lw	$a2, (CONTEXT_R6 * 4) ($sp)
	lw	$a3, (CONTEXT_R7 * 4) ($sp)
	lw	$t0, (CONTEXT_R8 * 4) ($sp)
	lw	$t1, (CONTEXT_R9 * 4) ($sp)
	lw	$t2, (CONTEXT_R10 * 4) ($sp)
	lw	$t3, (CONTEXT_R11 * 4) ($sp)
	lw	$t4, (CONTEXT_R12 * 4) ($sp)
	lw	$t5, (CONTEXT_R13 * 4) ($sp)
	lw	$t6, (CONTEXT_R14 * 4) ($sp)
	lw	$t7, (CONTEXT_R15 * 4) ($sp)
	lw	$s0, (CONTEXT_R16 * 4) ($sp)
	lw	$s1, (CONTEXT_R17 * 4) ($sp)
	lw	$s2, (CONTEXT_R18 * 4) ($sp)
	lw	$s3, (CONTEXT_R19 * 4) ($sp)
	lw	$s4, (CONTEXT_R20 * 4) ($sp)
	lw	$s5, (CONTEXT_R21 * 4) ($sp)
	lw	$s6, (CONTEXT_R22 * 4) ($sp)
	lw	$s7, (CONTEXT_R23 * 4) ($sp)
	lw	$t8, (CONTEXT_R24 * 4) ($sp)
	lw	$t9, (CONTEXT_R25 * 4) ($sp)
// Skip $26 - K0
// Skip $27 - K1
	lw	$gp, (CONTEXT_GP * 4) ($sp)
// Skip $29 - SP
	lw	$fp, (CONTEXT_FP * 4) ($sp)
	lw	$ra, (CONTEXT_RA * 4) ($sp)

	addi	$sp, CONTEXT_WORDS*4		// Deallocate saved context
	.set pop
.endm

/**
 * This macro is used to initialize MIPS registers
 */
.macro INIT_REGS
	.set	push
	.set	noat
	move	$AT, $zero
	move	$v0, $zero
	move	$v1, $zero
	move	$a0, $zero
	move	$a1, $zero
	move	$a2, $zero
	move	$a3, $zero
	move	$t0, $zero
	move	$t1, $zero
	move	$t2, $zero
	move	$t3, $zero
	move	$t4, $zero
	move	$t5, $zero
	move	$t6, $zero
	move	$t7, $zero
	move	$s0, $zero
	move	$s1, $zero
	move	$s2, $zero
	move	$s3, $zero
	move	$s4, $zero
	move	$s5, $zero
	move	$s6, $zero
	move	$s7, $zero
	move	$t8, $zero
	move	$t9, $zero
	.set    pop
.endm

/**
 * This macro sets the stack pointer to stack memory allocated in linker script and
 * stack check guard to pseudo random value.
 *
 * Note: Ideally, a random number should be assigned instead of the combination of a
 * timer's value and a compile-time constant. As currently the random number generator
 * has not implemented yet in the software, this is better than nothing but
 * not necessarily really secure.
 */
.macro INIT_STACK
	nop
	li      $k0, PLAT_RANDOM_CANARY_VALUE
	mfc0    $k1, $CP0_COUNT
	xor     $k0, $k0, $k1

	// Update the canary with the returned value
	la      $k1, __stack_chk_guard
	sw      $k0, 0($k1)

	// Set stack pointer
	la      $sp, __stack
	nop
.endm

#endif // __ASSEMBLER__
