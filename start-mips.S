/*
 * Copyright 2021 RnD Center "ELVEES", JSC
 */

#include "mips/asm.h"
#include "mips/m64c0.h"
#include "mips/regdef.h"

.set    noreorder

LEAF(_start)
setup_cpu:
	la      sp, _stack
	la      gp, _gp

/* TODO: Need .data section copy */

/* Zero fill .bss/.sbss sections */
	la      a0, __bss_start
	la      a1, __bss_length
	li      v0, 0
bss_zero_loop:
	blez    a1, bss_zero_done
	nop
	sw      v0, 0(a0)
	addiu   a0, a0, 4
	addiu   a1, a1, -4
	b       bss_zero_loop
	nop
bss_zero_done:

/* Copy .data/.sdata sections to RAM */
	la      a0, __data_start
	la      a1, __data_end
	la	a2, __text_end
data_copy_loop:
	sltu	v0, a0, a1
	beqz	v0, data_copy_done
	nop
	lw	v0, 0(a2)
	sw	v0, 0(a0)
	addiu   a0, a0, 4
	addiu   a2, a2, 4
	b	data_copy_loop
	nop
data_copy_done:

	la      a2, main
	jr      a2
	nop
END(_start)
