// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

    .text
    .set    noreorder
    .set    mips32
    .set    noat

#include <mc_asm.h>
#include <mcom03.h>

# Exception Vector 0x000
    .org 0x000
    b           1f
    nop
# Exception Vector 0x180
    .org 0x180
    b           1f
    nop
# Exception Vector 0x200
    .org 0x200
    b           1f
    nop
# Exception Vector 0x380
    .org 0x380
    b           1f
    nop
# Exception Vector 0x400
    .org 0x400
1:  li          $k0, MC_CSR_REG
    lw          $k1, ($k0)
    nop
    ori         $k1, FLUSH_I /* + FLUSH_D */
    sw          $k1, ($k0)

    la          $k0, _interrupt_entry
    jr          $k0
    nop

    .set at
    .set reorder
