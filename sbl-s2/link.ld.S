// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include "platform-def.h"

OUTPUT_FORMAT("elf32-littlemips")
OUTPUT_ARCH("mips:isa32")
ENTRY(_startup)

#ifndef NDEBUG
GROUP(-lc -lgcc -lg)
#else
GROUP(-lc -lgcc)
#endif

MEMORY
{

	spram0 (rwx)            : ORIGIN = 0x80000000,          LENGTH = 64K
	spram0_uncached (rwx)   : ORIGIN = 0xa0000000,          LENGTH = 64K
	ddr_low_trusted0 (rwx)  : ORIGIN = 0x40000000,          LENGTH = 512M
	ddr_low_sdr0 (rwx)      : ORIGIN = 0x60000000,          LENGTH = 512M
	ddr_low_trusted1 (rwx)  : ORIGIN = 0xc0000000,          LENGTH = 512M
	ddr_low_sdr1 (rwx)      : ORIGIN = 0xe0000000,          LENGTH = 512M
	iommu (rwx)             : ORIGIN = PLAT_IOMMU_BASE,     LENGTH = PLAT_IOMMU_SIZE
	fw_slot (rw)            : ORIGIN = PLAT_FW_SLOT_OFFSET, LENGTH = PLAT_FW_SLOT_SIZE
	ram (rwx)               : ORIGIN = PLAT_SBL_S2_BASE,    LENGTH = PLAT_SBL_S2_SIZE
}

SECTIONS
{
	. = PLAT_FW_SLOT_OFFSET;
	ASSERT(. == ALIGN(4), "fw slot address is not aligned on a word boundary.")

	.fw_slot PLAT_FW_SLOT_OFFSET (NOLOAD) : {
		KEEP(*(.fw_slot));
	} > fw_slot

	end_fw_slot = .;

	ASSERT(end_fw_slot == (PLAT_FW_SLOT_OFFSET + PLAT_FW_SLOT_SIZE), "fw slot has exceeded RAM limit.")

	. = ORIGIN(ram);
	ASSERT(. == ALIGN(4), "ORIGIN(ram) address is not aligned on a word boundary.")

	.text ORIGIN(ram) : {
		KEEP(*startup.*(.text .text.*));
		*(.text .text.*)
	} > ram

	.rdata : {
		*(.rdata .rdata.*)
		*(.rodata .rodata.*)
	} > ram

	.data : {
		. = ALIGN(4);
		*(.data .data.*)
		. = ALIGN(4);
		__gp = ALIGN(16);  // Base of small data (for MIPS)
		*(.lit8)
		*(.lit4)
		*(.sdata .sdata.*)
	} > ram

	.bss (NOLOAD) : {
		. = ALIGN(4);
		__bss_start = ABSOLUTE(.);
		*(.sbss .sbss.*)
		*(.scommon .scommon.*)
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = ABSOLUTE(.);
	} > ram
	__bss_length = __bss_end - __bss_start;

	.heap (NOLOAD) : {
		. = ALIGN(PLAT_HEAP_POINTER_ALIGN);
		__heap_start = ABSOLUTE(.);
		. = . + PLAT_HEAP_SIZE;
		__heap_end = ABSOLUTE(.);
	} > ram

	.stack (NOLOAD) : {
		. = ALIGN(8);
		KEEP(* (.stack .stack.*));
		. = . + PLAT_STACK_SIZE;
		. = ALIGN(8);
		__stack = . - 0x20;
	} > ram

	__spram0_start = ORIGIN(spram0);
	__spram0_end = ORIGIN(spram0) + LENGTH(spram0);
	__spram0_uncached_start = ORIGIN(spram0_uncached);
	__spram0_uncached_end = ORIGIN(spram0_uncached) + LENGTH(spram0_uncached);
	__ddr_low_trusted0_start = ORIGIN(ddr_low_trusted0);
	__ddr_low_trusted0_end = ORIGIN(ddr_low_trusted0) + LENGTH(ddr_low_trusted0);
	__ddr_low_sdr0_start = ORIGIN(ddr_low_sdr0);
	__ddr_low_sdr0_end = ORIGIN(ddr_low_sdr0) + LENGTH(ddr_low_sdr0);
	__ddr_low_trusted1_start = ORIGIN(ddr_low_trusted1);
	__ddr_low_trusted1_end = ORIGIN(ddr_low_trusted1) + LENGTH(ddr_low_trusted1);
	__ddr_low_sdr1_start = ORIGIN(ddr_low_sdr1);
	__ddr_low_sdr1_end = ORIGIN(ddr_low_sdr1) + LENGTH(ddr_low_sdr1);
	__iommu_start = ORIGIN(iommu);
	__iommu_end = ORIGIN(iommu) + LENGTH(iommu);
	__fw_slot_start = ORIGIN(fw_slot);
	__fw_slot_end = ORIGIN(fw_slot) + LENGTH(fw_slot);
	__ram_start = ORIGIN(ram);
	__ram_end = ORIGIN(ram) + LENGTH(ram);

#ifndef NDEBUG
	/**
	 * DWARF debug sections.
	 * Symbols in the DWARF debugging sections are relative to the beginning
	 * of the section so we begin them at 0.
	 */

	// DWARF 1
	.debug           0 : { *(.debug) }
	.line            0 : { *(.line) }

	// GNU DWARF 1 extensions
	.debug_srcinfo   0 : { *(.debug_srcinfo) }
	.debug_sfnames   0 : { *(.debug_sfnames) }
	// DWARF 1.1 and DWARF 2
	.debug_aranges   0 : { *(.debug_aranges) }
	.debug_pubnames  0 : { *(.debug_pubnames) }

	// DWARF 2
	.debug_info      0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev    0 : { *(.debug_abbrev) }
	.debug_line      0 : { *(.debug_line .debug_line.* .debug_line_end ) }
	.debug_frame     0 : { *(.debug_frame) }
	.debug_str       0 : { *(.debug_str) }
	.debug_loc       0 : { *(.debug_loc) }
	.debug_macinfo   0 : { *(.debug_macinfo) }

	// SGI/MIPS DWARF 2 extensions
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }

	// DWARF 3
	.debug_pubtypes  0 : { *(.debug_pubtypes) }
	.debug_ranges    0 : { *(.debug_ranges) }

	// DWARF Extension.
	.debug_macro     0 : { *(.debug_macro) }
	.gnu.attributes  0 : { KEEP (*(.gnu.attributes)) }
	.gptab.sdata       : { *(.gptab.data) *(.gptab.sdata) }
	.gptab.sbss        : { *(.gptab.bss) *(.gptab.sbss) }
	.mdebug.abi32    0 : { KEEP(*(.mdebug.abi32)) }
	.mdebug.abiN32   0 : { KEEP(*(.mdebug.abiN32)) }
	.mdebug.abi64    0 : { KEEP(*(.mdebug.abi64)) }
	.mdebug.abiO64   0 : { KEEP(*(.mdebug.abiO64)) }
	.mdebug.eabi32   0 : { KEEP(*(.mdebug.eabi32)) }
	.mdebug.eabi64   0 : { KEEP(*(.mdebug.eabi64)) }
	/DISCARD/ : { *(.note.GNU-stack) *(.MIPS.abiflags) }
#else
	/DISCARD/ : {
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
