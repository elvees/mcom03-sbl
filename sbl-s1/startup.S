// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <drivers/mips-cp0/mips-cp0.h>
#include <libs/asm.h>

	.global main

LEAF(_startup)
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

	SAVE_REGS
	la            $gp, __gp              // set current global pointer

	la            $k0, main              // call main
	jalr          $k0
	nop
	bnez          $v0, 4f                // Check if the main return error
	nop
	RESTORE_REGS

	li            $k0, MC_CSR_REG        // invalidate cache
	lw            $k1, ($k0)
	ori           $k1, FLUSH_I + FLUSH_D
	sw            $k1, ($k0)

	// Allocate address for a JTAG breakpoint
jtag:
	.align 4
	nop

3:	lui           $k0, 0x9fa0
	lui           $k1, 0x0100
	sw            $k1, 312($k0)
	li            $k0, 0xbfc00500        // Return control to SecBoot
	jalr          $k0
	nop
	b             3b                     // SecBoot will pass control to this line
	nop                                  // every time after PAYLOAD_NO_EXEC image

4:	nop
	wait
	b             4b                     // Infinity loop in case of error
	nop
END(_startup)
