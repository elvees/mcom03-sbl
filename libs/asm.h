// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __ASM_H__
#define __ASM_H__

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
#define s8   30
#define fp   30
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
	.set  CONTEXT_F0,                       36
	.set  CONTEXT_F1,                       37
	.set  CONTEXT_F2,                       38
	.set  CONTEXT_F3,                       39
	.set  CONTEXT_F4,                       40
	.set  CONTEXT_F5,                       41
	.set  CONTEXT_F6,                       42
	.set  CONTEXT_F7,                       43
	.set  CONTEXT_F8,                       44
	.set  CONTEXT_F9,                       45
	.set  CONTEXT_F10,                      46
	.set  CONTEXT_F11,                      47
	.set  CONTEXT_F12,                      48
	.set  CONTEXT_F13,                      49
	.set  CONTEXT_F14,                      50
	.set  CONTEXT_F15,                      51
	.set  CONTEXT_F16,                      52
	.set  CONTEXT_F17,                      53
	.set  CONTEXT_F18,                      54
	.set  CONTEXT_F19,                      55
	.set  CONTEXT_F20,                      56
	.set  CONTEXT_F21,                      57
	.set  CONTEXT_F22,                      58
	.set  CONTEXT_F23,                      59
	.set  CONTEXT_F24,                      60
	.set  CONTEXT_F25,                      61
	.set  CONTEXT_F26,                      62
	.set  CONTEXT_F27,                      63
	.set  CONTEXT_F28,                      64
	.set  CONTEXT_F29,                      65
	.set  CONTEXT_F30,                      66
	.set  CONTEXT_F31,                      67
	.set  CONTEXT_FCSR,                     68
	.set  CONTEXT_FLAG_HAS_FPU,             69
	.set  CONTEXT_FLAG_CRITICAL_NESTING,    70
	.set  CONTEXT_FLAG_INTERRUPT_NESTING,   71

	.set CONTEXT_WORDS,                     72

	.set $C0_INDEX,                         $0
	.set $C0_RANDOM,                        $1
	.set $C0_ENTRYL0,                       $2
	.set $C0_ENTRYL1,                       $3
	.set $C0_CONTEXT,                       $4
	.set $C0_PAGEMASK,                      $5
	.set $C0_WIRED,                         $6
	.set $C0_BADVADDR,                      $8
	.set $C0_COUNT,                         $9
	.set $C0_ENTRYHI,                       $10
	.set $C0_COMPARE,                       $11
	.set $C0_STATUS,                        $12
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
	.set $C0_CAUSE,                         $13
	.set IV_ONE,                            0x800000
	.set IV_ZERO,                           0xFF7FFFFF
	.set $C0_EPC,                           $14
	.set $C0_PRID,                          $15
	.set $C0_CONFIG,                        $16
	.set K0_CACHE_MASK,                     0x00000007
	.set K0_CACHE_DISABLE,                  0x00000002
	.set K0_CACHE_ENABLE,                   0x00000003
	.set K23_CACHE_MASK,                    0x70000000
	.set K23_CACHE_DISABLE,                 0x20000000
	.set K23_CACHE_ENABLE,                  0x30000000
	.set KU_CACHE_MASK,                     0x0E000000
	.set KU_CACHE_DISABLE,                  0x04000000
	.set KU_CACHE_ENABLE,                   0x06000000
	.set $C0_LLADR,                         $17
	.set $C0_ERROREPC,                      $30

	.set FLUSH_I,                           0x1000
	.set FLUSH_D,                           0x4000
	.set TR_CRAM,                           0x2
	.set TR_CRAM_MASK,                      0xFFFFFFFD

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

	mfc0	$k0, $C0_EPC
	sw	$k0, (CONTEXT_PC * 4) ($sp)

	mfc0	$k0, $C0_STATUS
	sw	$k0, (CONTEXT_STATUS * 4) ($sp)
	.endm


	.macro RESTORE_REGS
	lw	$k0, (CONTEXT_LO * 4) ($sp)	// Load HI, LO registers
	mtlo	$k0
	lw	$k0, (CONTEXT_HI * 4) ($sp)
	mthi	$k0

	lw	$k0, (CONTEXT_PC * 4) ($sp)	// K1 = EPC
	mtc0	$k0, $C0_EPC			// put PC in EPC

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

	.macro SAVE_FP_REGS

// Already allocated space in SAVE_REGS macros

	mfc1    $k0, $f0
	sw	$k0, (CONTEXT_F0 * 4) ($sp)	// Save fp registers
	mfc1    $k0, $f1
	sw	$k0, (CONTEXT_F1 * 4) ($sp)
	mfc1    $k0, $f2
	sw	$k0, (CONTEXT_F2 * 4) ($sp)
	mfc1    $k0, $f3
	sw	$k0, (CONTEXT_F3 * 4) ($sp)
	mfc1    $k0, $f4
	sw	$k0, (CONTEXT_F4 * 4) ($sp)
	mfc1    $k0, $f5
	sw	$k0, (CONTEXT_F5 * 4) ($sp)
	mfc1    $k0, $f6
	sw	$k0, (CONTEXT_F6 * 4) ($sp)
	mfc1    $k0, $f7
	sw	$k0, (CONTEXT_F7 * 4) ($sp)
	mfc1    $k0, $f8
	sw	$k0, (CONTEXT_F8 * 4) ($sp)
	mfc1    $k0, $f9
	sw	$k0, (CONTEXT_F9 * 4) ($sp)
	mfc1    $k0, $f10
	sw	$k0, (CONTEXT_F10 * 4) ($sp)
	mfc1    $k0, $f11
	sw	$k0, (CONTEXT_F11 * 4) ($sp)
	mfc1    $k0, $f12
	sw	$k0, (CONTEXT_F12 * 4) ($sp)
	mfc1    $k0, $f13
	sw	$k0, (CONTEXT_F13 * 4) ($sp)
	mfc1    $k0, $f14
	sw	$k0, (CONTEXT_F14 * 4) ($sp)
	mfc1    $k0, $f15
	sw	$k0, (CONTEXT_F15 * 4) ($sp)
	mfc1    $k0, $f16
	sw	$k0, (CONTEXT_F16 * 4) ($sp)
	mfc1    $k0, $f17
	sw	$k0, (CONTEXT_F17 * 4) ($sp)
	mfc1    $k0, $f18
	sw	$k0, (CONTEXT_F18 * 4) ($sp)
	mfc1    $k0, $f19
	sw	$k0, (CONTEXT_F19 * 4) ($sp)
	mfc1    $k0, $f20
	sw	$k0, (CONTEXT_F20 * 4) ($sp)
	mfc1    $k0, $f21
	sw	$k0, (CONTEXT_F21 * 4) ($sp)
	mfc1    $k0, $f22
	sw	$k0, (CONTEXT_F22 * 4) ($sp)
	mfc1    $k0, $f23
	sw	$k0, (CONTEXT_F23 * 4) ($sp)
	mfc1    $k0, $f24
	sw	$k0, (CONTEXT_F24 * 4) ($sp)
	mfc1    $k0, $f25
	sw	$k0, (CONTEXT_F25 * 4) ($sp)
	mfc1    $k0, $f26
	sw	$k0, (CONTEXT_F26 * 4) ($sp)
	mfc1    $k0, $f27
	sw	$k0, (CONTEXT_F27 * 4) ($sp)
	mfc1    $k0, $f28
	sw	$k0, (CONTEXT_F28 * 4) ($sp)
	mfc1    $k0, $f29
	sw	$k0, (CONTEXT_F29 * 4) ($sp)
	mfc1    $k0, $f30
	sw	$k0, (CONTEXT_F30 * 4) ($sp)
	mfc1    $k0, $f31
	sw	$k0, (CONTEXT_F31 * 4) ($sp)
	cfc1    $k0, $31
	sw	$k0, (CONTEXT_FCSR * 4) ($sp)
	.endm

	.macro RESTORE_FP_REGS

	lw	$k0, (CONTEXT_F0 * 4) ($sp)	// Restore fp registers
	mtc1    $k0, $f0
	lw	$k0, (CONTEXT_F1 * 4) ($sp)
	mtc1    $k0, $f1
	lw	$k0, (CONTEXT_F2 * 4) ($sp)
	mtc1    $k0, $f2
	lw	$k0, (CONTEXT_F3 * 4) ($sp)
	mtc1    $k0, $f3
	lw	$k0, (CONTEXT_F4 * 4) ($sp)
	mtc1    $k0, $f4
	lw	$k0, (CONTEXT_F5 * 4) ($sp)
	mtc1    $k0, $f5
	lw	$k0, (CONTEXT_F6 * 4) ($sp)
	mtc1    $k0, $f6
	lw	$k0, (CONTEXT_F7 * 4) ($sp)
	mtc1    $k0, $f7
	lw	$k0, (CONTEXT_F8 * 4) ($sp)
	mtc1    $k0, $f8
	lw	$k0, (CONTEXT_F9 * 4) ($sp)
	mtc1    $k0, $f9
	lw	$k0, (CONTEXT_F10 * 4) ($sp)
	mtc1    $k0, $f10
	lw	$k0, (CONTEXT_F11 * 4) ($sp)
	mtc1    $k0, $f11
	lw	$k0, (CONTEXT_F12 * 4) ($sp)
	mtc1    $k0, $f12
	lw	$k0, (CONTEXT_F13 * 4) ($sp)
	mtc1    $k0, $f13
	lw	$k0, (CONTEXT_F14 * 4) ($sp)
	mtc1    $k0, $f14
	lw	$k0, (CONTEXT_F15 * 4) ($sp)
	mtc1    $k0, $f15
	lw	$k0, (CONTEXT_F16 * 4) ($sp)
	mtc1    $k0, $f16
	lw	$k0, (CONTEXT_F17 * 4) ($sp)
	mtc1    $k0, $f17
	lw	$k0, (CONTEXT_F18 * 4) ($sp)
	mtc1    $k0, $f18
	lw	$k0, (CONTEXT_F19 * 4) ($sp)
	mtc1    $k0, $f19
	lw	$k0, (CONTEXT_F20 * 4) ($sp)
	mtc1    $k0, $f20
	lw	$k0, (CONTEXT_F21 * 4) ($sp)
	mtc1    $k0, $f21
	lw	$k0, (CONTEXT_F22 * 4) ($sp)
	mtc1    $k0, $f22
	lw	$k0, (CONTEXT_F23 * 4) ($sp)
	mtc1    $k0, $f23
	lw	$k0, (CONTEXT_F24 * 4) ($sp)
	mtc1    $k0, $f24
	lw	$k0, (CONTEXT_F25 * 4) ($sp)
	mtc1    $k0, $f25
	lw	$k0, (CONTEXT_F26 * 4) ($sp)
	mtc1    $k0, $f26
	lw	$k0, (CONTEXT_F27 * 4) ($sp)
	mtc1    $k0, $f27
	lw	$k0, (CONTEXT_F28 * 4) ($sp)
	mtc1    $k0, $f28
	lw	$k0, (CONTEXT_F29 * 4) ($sp)
	mtc1    $k0, $f29
	lw	$k0, (CONTEXT_F30 * 4) ($sp)
	mtc1    $k0, $f30
	lw	$k0, (CONTEXT_F31 * 4) ($sp)
	mtc1    $k0, $f31
	lw	$k0, (CONTEXT_FCSR * 4) ($sp)
	ctc1    $k0, $31
	.endm

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

	.macro PUSH reg
	addiu $sp, $sp, -16
	sw  \reg, 12($sp)
	sw  $fp, 8($sp)
	move  $fp, $sp
	.endm

	.macro POP reg
	lw \reg, 12($sp)
	lw $fp, 8($sp)
	addiu $sp, $sp, 16
	.endm

	/*
	 * This macro sets the stack pointer to the stack memory allocated in linker script.
	 *
	 * return $v0 with saved stack pointer
	 */
	.macro INIT_STACK
	nop
	move    $v0, $sp
	la      $sp, __stack
	nop
	.endm

	/*
	 * In some cases it required to save predecessor's stack to restore it before jump back
	 * to predecessor. This macro saves the stack pointer returned by INIT_STACK to
	 * current stack.
	 */
	.macro SAVE_PRE_STACK
	nop
	PUSH    $v0
	nop
	.endm

	/*
	 * This macro restores the stack pointer returned by INIT_STACK from
	 * current stack.
	 */
	.macro RESTORE_PRE_STACK
	POP     $k0
	move    $sp, $k0
	nop
	.endm
#endif /* __ASSEMBLER__ */
#endif /* __ASM_H__ */
