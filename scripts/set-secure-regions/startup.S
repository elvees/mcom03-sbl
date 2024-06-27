// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <drivers/mcom03-regs.h>
#include <libs/asm.h>

	.global main

LEAF(_startup)
	INIT_REGS

	// zero fill .bss/.sbss sections
	la              $k0, __bss_start
	la              $k1, __bss_length
1:	blez            $k1, 2f
	nop
	sw              $zero, 0($k0)
	addiu           $k0, $k0, 4
	addiu           $k1, $k1, -4
	b               1b
	nop

2:	INIT_STACK

	la              $gp, __gp            // set current global pointer

	la              $k0, main            // call main
	jalr            $k0
	nop
	bnez            $v0, 3f              // Check if the main return error
	nop

	li              $k0, MC_CSR_REG      // invalidate cache
	lw              $k1, ($k0)
	ori             $k1, FLUSH_I + FLUSH_D
	sw              $k1, ($k0)

	// Allocate address for a JTAG breakpoint
jtag:
	.align 4
	nop
	jr              $ra
	nop

3:	b               .                    // Infinity loop in case of error
	nop
END(_startup)
