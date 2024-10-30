// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <platform_def.h>

OUTPUT_FORMAT(PLATFORM_LINKER_FORMAT)
OUTPUT_ARCH(PLATFORM_LINKER_ARCH)
ENTRY(_entrypoint)

#ifdef DEBUG
GROUP(-lc -lgcc -lg)
#else
GROUP(-lc -lgcc)
#endif

MEMORY
{
  vcpu_cram (rwx)           : ORIGIN = 0x9fa00000,                 LENGTH = 32K
  vcpu_cram_uncached (rwx)  : ORIGIN = 0xbfa00000,                 LENGTH = 32K
  spram0 (rwx)              : ORIGIN = 0xa0000000,                 LENGTH = 64K
  ddr_iommu (rw)            : ORIGIN = PLAT_IOMMU_BASE,            LENGTH = PLAT_IOMMU_SIZE
  ddr_mips (rwx)            : ORIGIN = PLAT_RAM_BASE,              LENGTH = PLAT_RAM_SIZE
}

SECTIONS
{
  . = PLAT_IOMMU_BASE;
  ASSERT(. == ALIGN(PAGE_SIZE), "IOMMU_BASE address is not aligned on a page boundary.")

  .iommu.table PLAT_IOMMU_BASE (NOLOAD) :
  {
    __iommu_table_start = .;
    KEEP(* (.iommu.table .iommu.table.*));
    __iommu_table_end = .;
  } > ddr_iommu

  . = PLAT_MIPS_BASE;
  ASSERT(. == ALIGN(PAGE_SIZE), "MIPS_BASE address is not aligned on a page boundary.")

  .entrypoint.text PLAT_MIPS_BASE :
  {
    KEEP(*entrypoint.* (.text .text.*));
  } > ddr_mips

  .vectors.text :
  {
    . = ALIGN(4);
    KEEP(*vectors.*(.text .text.*));
    . = ALIGN(4);
  } > vcpu_cram_uncached AT > ddr_mips

  PROVIDE(__l_vectors_start = LOADADDR(.vectors.text));
  PROVIDE(__l_vectors_size = SIZEOF(.vectors.text));
  PROVIDE(__v_vectors_start = ADDR(.vectors.text));

  .text :
  {
    *(.text .text.*);
  } > ddr_mips

  .rdata :
  {
    *(.rdata .rdata.*)
    *(.rodata .rodata.*)
  } > ddr_mips

  .data :
  {
    . = ALIGN(4);
    *(.data .data.*)
    . = ALIGN(4);
    __gp = ALIGN(16);  /* Base of small data (for MIPS) */
    *(.lit8)
    *(.lit4)
    *(.sdata .sdata.*)
  } > ddr_mips

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    __BSS_START__ = .;
    *(.sbss .sbss.*)
    *(.scommon .scommon.*)
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4);
    __BSS_END__ = .;
  } > ddr_mips
  __BSS_SIZE__ = __BSS_END__ - __BSS_START__;

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    KEEP(* (.stack .stack.*));
    . = . + PLATFORM_STACK_SIZE;
    . = ALIGN(8);
    __stack = . - 0x20;
  } > ddr_mips

  .heap (NOLOAD) :
  {
    KEEP (*(.heap));
  } > ddr_mips

  end = .;

  ASSERT(end < (PLAT_RAM_BASE + PLAT_RAM_SIZE), "MIPS image has exceeded RAM limit.")

#ifdef DEBUG
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end ) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3 */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF Extension.  */
  .debug_macro    0 : { *(.debug_macro) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  .gptab.sdata : { *(.gptab.data) *(.gptab.sdata) }
  .gptab.sbss : { *(.gptab.bss) *(.gptab.sbss) }
  .mdebug.abi32 0 : { KEEP(*(.mdebug.abi32)) }
  .mdebug.abiN32 0 : { KEEP(*(.mdebug.abiN32)) }
  .mdebug.abi64 0 : { KEEP(*(.mdebug.abi64)) }
  .mdebug.abiO64 0 : { KEEP(*(.mdebug.abiO64)) }
  .mdebug.eabi32 0 : { KEEP(*(.mdebug.eabi32)) }
  .mdebug.eabi64 0 : { KEEP(*(.mdebug.eabi64)) }
#else
  /DISCARD/ :
  {
    *(.reginfo)
    *(.MIPS.abiflags)
    *(.comment)
    *(.note*)
    *(.eh_frame*)
    *(.dynsym)
    *(.dynstr*)
    *(.dynamic*)
    *(.plt*)
    *(.interp*)
    *(.gnu*)
    *(.debug)
    *(.line)
    *(.debug_srcinfo)
    *(.debug_sfnames)
    *(.debug_aranges)
    *(.debug_pubnames)
    *(.debug_info .gnu.linkonce.wi.*)
    *(.debug_abbrev)
    *(.debug_line .debug_line.* .debug_line_end)
    *(.debug_frame)
    *(.debug_str)
    *(.debug_loc)
    *(.debug_macinfo)
    *(.debug_weaknames)
    *(.debug_funcnames)
    *(.debug_typenames)
    *(.debug_varnames)
    *(.debug_pubtypes)
    *(.debug_ranges)
    *(.debug_macro)
    *(.gnu.attributes)
    *(.gptab.data)
    *(.gptab.sdata)
    *(.gptab.bss)
    *(.gptab.sbss)
    *(.mdebug.abi32)
    *(.mdebug.abiN32)
    *(.mdebug.abi64)
    *(.mdebug.abiO64)
    *(.mdebug.eabi32)
    *(.mdebug.eabi64)
  }
#endif
}
