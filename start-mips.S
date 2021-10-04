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

	/* Setup memory mapping */

	/* Set page size to 16 MiB */
	li      a0, 0x1ffe000
	mtc0    a0, C0_PAGEMASK

	mtc0    zero, C0_WIRED

	/* Map 16 MiB at 0xC0000000 (virt) to 0x80000000 (phys) */
	li      a0, 1
	mtc0    a0, C0_INDEX

	li      a1, 0xC0000000
	mtc0    a1, C0_ENTRYHI

	li      a1, 0x2000017
	mtc0    a1, C0_ENTRYLO0
	mtc0    a1, C0_ENTRYLO1
	ehb
	tlbwi
	nop

	/* Switch to TLB mapping */
	li      a0, 0x1fd08000
	li      v0, 0
	sw      v0, 0(a0)

	la      a2, main
	jr      a2
	nop
END(_start)
