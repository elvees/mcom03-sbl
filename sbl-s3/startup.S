// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

    .text
    .set    noreorder
    .set    mips32
    .set    noat

#include <mc_asm.h>
#include <mcom03.h>
#include <platform_def.h>

    .global _entrypoint

LEAF(_entrypoint)
    li          $k1, 0x000000           # disable interrupts, IE[0] = 0, ERL[2] = 0, BEV[22] = 0
    mtc0        $k1, $C0_STATUS

    INIT_REGS

    la          $28, __gp

    la          $k0, plat_set_my_stack
    jalr        $k0
    nop

    la          $a0, __v_vectors_start  # Copy vectors from DDR to (uncached) CRAM.
    la          $a1, __l_vectors_start
    la          $a2, __l_vectors_size
    la          $k0, memcpy
    jalr        $k0
    nop

    li          $k0, IV_ZERO            # IV = 0
    mfc0        $k1, $C0_CAUSE
    and         $k1, $k1, $k0
    mtc0        $k1, $C0_CAUSE

    li          $k1, MC_CSR_REG         # interrupt vector to 0xbfa00000 (BEV = 0)
    lw          $k0, ($k1)
    nop
    ori         $k0, TR_CRAM
    sw          $k0, ($k1)

    mfc0        $k1, $C0_CONFIG         # setup program cache

    li          $k0, (~K0_CACHE_MASK)
    and         $k1, $k1, $k0
    li          $k0, K0_CACHE_ENABLE
    or          $k1, $k1, $k0

    li          $k0, (~K23_CACHE_MASK)
    and         $k1, $k1, $k0
    li          $k0, K23_CACHE_DISABLE
    or          $k1, $k1, $k0

    li          $k0, (~KU_CACHE_MASK)
    and         $k1, $k1, $k0
    li          $k0, KU_CACHE_ENABLE
    or          $k1, $k1, $k0

    mtc0        $k1, $C0_CONFIG

    la          $a0, __BSS_START__      # set dest
    move        $a1, $zero              # set number
    la          $a2, __BSS_SIZE__       # set size
    la          $k0, memset             # zero BSS
    jalr        $k0
    nop

    /* Ensure the console is initialized */
    li          $a0, CONSOLE_OUTPUT_UART
    la          $k0, console_init_common
    jalr        $k0
    nop

    /* Check if the console is initialized */
    bnez        $v0, 1f
    nop

    la          $k0, main
    jalr        $k0
    nop

 1: b           .
    nop
END(_entrypoint)

    .set at
    .set reorder
