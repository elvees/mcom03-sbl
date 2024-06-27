// SPDX-License-Identifier: MIT
// Copyright 2021-2024 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <libs/asm.h>

	.global main

LEAF(_startup)
setup_cpu:
	li            $k0, 0x000000           // disable interrupts, IE[0] = 0, ERL[2] = 0,
	mtc0          $k0, $C0_STATUS         // BEV[22] = 0

	li            $k0, 0xFF7FFFFF         // IV = 0
	mfc0          $k1, $C0_CAUSE
	and           $k1, $k1, $k0
	mtc0          $k1, $C0_CAUSE

	li            $k0, 0xbfd08000         // Switch to fixed mapping
	li            $v0, 0x1
	sw            $v0, 0($k0)

	mfc0          $k1, $C0_CONFIG         // setup program cache

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

	mtc0          $k1, $C0_CONFIG

	INIT_STACK

	la            $gp, __gp

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

	// copy .data/.sdata sections to RAM
2:	la            $a0, __data_start
	la            $a1, __text_end
	la            $a2, __data_length
	la            $k0, memcpy
	jalr          $k0
	nop

	la            $a2, main
	jalr          $a2
	nop

wait_loop:
	wait
	b             wait_loop
	nop
END(_startup)
