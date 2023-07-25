// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <asm.h>

.text
.set noreorder
.set mips32
.set noat

.global main

LEAF(_startup)
	la      $k0, __bss_start     // init bss
	la      $k1, __bss_length
	li      $v0, 0

1:	blez    $k1, 2f              // clear bss
	nop
	sw      $v0, 0($k0)
	addiu   $k0, $k0, 4
	addiu   $k1, $k1, -4
	b       1b
	nop

2:	la      $sp, _stack          // set current stack pointer
	la      $gp, _gp             // set current global pointer

	SAVE_REGS
	la      $k0, main            // call main
	jalr    $k0
	nop
	bnez    $v0, 3f              // Check if the main return error
	nop
	RESTORE_REGS

jtag:
	nop
	jr      $ra
	nop

3:	b       .                    // Infinity loop in case of error
	nop
END(_startup)

.set at
.set reorder
