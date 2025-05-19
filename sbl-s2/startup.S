// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <libs/asm.h>

	.global main

LEAF(_startup)
	li            $k1, 0x000000           // disable interrupts, IE[0] = 0, ERL[2] = 0,
	mtc0          $k1, $CP0_STATUS        // BEV[22] = 0

	mfc0          $k1, $CP0_CONFIG        // setup program cache

	li            $k0, (~K0_CACHE_MASK)
	and           $k1, $k1, $k0
	li            $k0, K0_CACHE_ENABLE
	or            $k1, $k1, $k0

	li            $k0, (~K23_CACHE_MASK)
	and           $k1, $k1, $k0
	li            $k0, K23_CACHE_DISABLE
	or            $k1, $k1, $k0

	li            $k0, (~KU_CACHE_MASK)
	and           $k1, $k1, $k0
	li            $k0, KU_CACHE_ENABLE
	or            $k1, $k1, $k0

	mtc0          $k1, $CP0_CONFIG

	// zero fill .bss/.sbss sections
	la            $k0, __bss_start
	la            $k1, __bss_length
1:	blez          $k1, 2f
	nop
	sw            $zero, 0($k0)
	addiu         $k0, $k0, 4
	addiu         $k1, $k1, -4
	b             1b
	nop

2:	INIT_STACK
	SAVE_REGS                             // save registers to SBL-S2 stack
	la            $gp, __gp               // set current global pointer

	la            $a0, __heap_start
	la            $a1, __heap_end

	la            $k0, heap_init
	jalr          $k0
	nop

	la            $k0, main               // call main
	jalr          $k0
	nop
	bnez          $v0, 3f                 // check if the main return error
	nop
	RESTORE_REGS                          // restore registers from SBL-S2 stack

3:	b             .                       // infinity loop in case of error
	nop
END(_startup)
