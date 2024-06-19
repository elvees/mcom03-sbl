// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <asm.h>

	.text
	.set noreorder
	.set mips32
	.set noat

	.set MC_CSR_REG,            0xbfd08000

	.global main

LEAF(_startup)
	la            $k0, __bss_start       // init bss
	la            $k1, __bss_length
	li            $v0, 0

1:	blez          $k1, 2f                // clear bss
	nop
	sw            $v0, 0($k0)
	addiu         $k0, $k0, 4
	addiu         $k1, $k1, -4
	b             1b
	nop

2:	la            $sp, _stack            // set current stack pointer
	la            $gp, _gp               // set current global pointer

	SAVE_REGS
	la            $k0, main              // call main
	jalr          $k0
	nop
	bnez          $v0, 4f                // Check if the main return error
	nop
	RESTORE_REGS

	// Allocate address for a JTAG breakpoint
	.org          0x188
	nop

3:	lui           $k0, 0x9fa0
	lui           $k1, 0x0100
	sw            $k1, 312($k0)
	li            $k0, 0xbfc00500        // Return control to SecBoot
	jalr          $k0
	nop
	b             3b                     // SecBoot will pass control to this line
	nop                                  // every time after PAYLOAD_NO_EXEC image

4:	b             .                      // Infinity loop in case of error
	nop
END(_startup)

	.set at
	.set reorder
