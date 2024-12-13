// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

	.text
	.set noreorder
	.set mips32
	.set noat

#include <libs/asm.h>

	.extern irq_handler
	.extern exception_handler

LEAF(_interrupt_entry)
	mfc0          $k1, $C0_CAUSE
	li            $k0, EXC_MASK
	and           $k0, $k1, $k0
	bnez          $k0, _exception_entry
	nop

	mfc0          $k0, $C0_STATUS
	and           $k1, $k0, $k1
	li            $k0, IP_MASK
	and           $k0, $k1, $k0
	srl           $k0, $k0, IP_MASK_OFFSET
	beqz          $k0, _exception_entry
	nop
	SAVE_REGS
	la            $k0, irq_handler
	jalr          $k0
	nop
	RESTORE_REGS
exit_interrupt:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	eret
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
_exception_entry:
	SAVE_REGS
	move          $a0, $sp
	la            $k0, exception_handler
	j             $k0
	nop

	// we should not be here
	b             .
	nop
END(_interrupt_entry)
