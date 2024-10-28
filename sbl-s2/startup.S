// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

  .text
  .set    noreorder
  .set    mips32
  .set    noat

#include <mc_asm.h>
#include <mcom03.h>
#include <platform_def.h>

  .global       main

LEAF(_entrypoint)
  li            $k1, 0x000000           // disable interrupts, IE[0] = 0, ERL[2] = 0, BEV[22] = 0
  mtc0          $k1, $C0_STATUS

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

  la            $k0, __BSS_START__        // BSS init
  la            $k1, __BSS_END__
  beq           $k0, $k1, 2f              // jump to label 2 if bss size == 0
  nop

1:sw            $zero, ($k0)              // BSS zero
  addiu         $k0, 4
  bne           $k0, $k1, 1b              // jump to label 1 if bss size != 0
  nop

2:la            $gp, __gp                 // init global pointer

  la            $k0, plat_set_my_stack    // set current stack pointer to SBL-TL one
  jalr          $k0
  nop

  SAVE_REGS                               // save registers to SBL-TL stack
  li            $a0, CONSOLE_OUTPUT_UART  // Ensure the console is initialized
  la            $k0, console_init_common
  jalr          $k0
  nop
  bnez          $v0, 4f                   // Check if the console is initialized
  nop

  la            $k0, main                 // call main
  jalr          $k0
  nop
  bnez          $v0, 4f                   // Check if the main return error
  nop
  RESTORE_REGS                            // restore registers from SBL-TL stack

3:lui           $k0, 0x9fa0
  lui           $k1, 0x0100
  sw            $k1, 312($k0)
  li            $k0, 0xbfc00500           // Return control to SecBoot
  jalr          $k0
  nop
  b             3b                        // SecBoot will pass control to this line every time after PAYLOAD_NO_EXEC image
  nop

4:b             .                         // infinity loop in case of error
  nop
END(_entrypoint)

  .set at
  .set reorder
