// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <drivers/mips-cp0/mips-cp0.h>
#include <libs/asm.h>

#include "platform-def.h"

	.global _startup

LEAF(_startup)
	li            $k1, 0x000000           // disable interrupts, IE[0] = 0, ERL[2] = 0,
	mtc0          $k1, $CP0_STATUS        // BEV[22] = 0

	INIT_REGS

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

	la            $gp, __gp

	la            $a0, __v_vectors_start  // copy vectors from RAM to (uncached) CRAM.
	la            $a1, __l_vectors_start
	la            $a2, __l_vectors_size
	la            $k0, memcpy
	jalr          $k0
	nop

	li            $k0, IV_ZERO            // IV = 0
	mfc0          $k1, $CP0_CAUSE
	and           $k1, $k1, $k0
	mtc0          $k1, $CP0_CAUSE

	li            $k1, MC_CSR_REG         // interrupt vector to 0xbfa00000 (BEV = 0)
	lw            $k0, ($k1)
	nop
	ori           $k0, TR_CRAM
	sw            $k0, ($k1)

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

	la            $a0, __heap_start
	la            $a1, __heap_end

	la            $k0, heap_init
	jalr          $k0
	nop

	la            $k0, main
	jalr          $k0
	nop

	b             .
	nop
END(_startup)
