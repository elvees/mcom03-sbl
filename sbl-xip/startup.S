// SPDX-License-Identifier: MIT
// Copyright 2021 RnD Center "ELVEES", JSC

#include "machine/asm.h"
#include "machine/regdef.h"
#include "mips/m32c0.h"

	.set noreorder

	.set K0_CACHE_MASK,         0x00000007
	.set K0_CACHE_DISABLE,      0x00000002
	.set K0_CACHE_ENABLE,       0x00000003
	.set K23_CACHE_MASK,        0x70000000
	.set K23_CACHE_DISABLE,     0x20000000
	.set K23_CACHE_ENABLE,      0x30000000
	.set KU_CACHE_MASK,         0x0E000000
	.set KU_CACHE_DISABLE,      0x04000000
	.set KU_CACHE_ENABLE,       0x06000000

LEAF(_start)
setup_cpu:
	li              a0, 0x000000           // disable interrupts, IE[0] = 0, ERL[2] = 0,
	mtc0            a0, C0_STATUS          // BEV[22] = 0

	li              a0, 0xFF7FFFFF         // IV = 0
	mfc0            a1, C0_CAUSE
	and             a1, a1, a0
	mtc0            a1, C0_CAUSE

	li              a0, 0xbfd08000         // Switch to fixed mapping
	li              v0, 0x1
	sw              v0, 0(a0)

	mfc0            a1, C0_CONFIG          // setup program cache
	li              a0, (~K0_CACHE_MASK)
	and             a1, a1, a0
	li              a0, K0_CACHE_ENABLE
	or              a1, a1, a0
	li              a0, (~K23_CACHE_MASK)
	and             a1, a1, a0
	li              a0, K23_CACHE_DISABLE
	or              a1, a1, a0
	li              a0, (~KU_CACHE_MASK)
	and             a1, a1, a0
	li              a0, KU_CACHE_ENABLE
	or              a1, a1, a0
	mtc0            a1, C0_CONFIG

	la              sp, _stack
	la              gp, _gp

/* TODO: Need .data section copy */

/* Zero fill .bss/.sbss sections */
	la              a0, __bss_start
	la              a1, __bss_length
	li              v0, 0
bss_zero_loop:
	blez            a1, bss_zero_done
	nop
	sw              v0, 0(a0)
	addiu           a0, a0, 4
	addiu           a1, a1, -4
	b               bss_zero_loop
	nop
bss_zero_done:

/* Copy .data/.sdata sections to RAM */
	la              a0, __data_start
	la              a1, __data_end
	la              a2, __text_end
data_copy_loop:
	sltu            v0, a0, a1
	beqz            v0, data_copy_done
	nop
	lw              v0, 0(a2)
	sw              v0, 0(a0)
	addiu           a0, a0, 4
	addiu           a2, a2, 4
	b               data_copy_loop
	nop
data_copy_done:
	la              a2, main
	jr              a2
	nop
END(_start)
