// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

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
	spram (rwx)  : ORIGIN = 0x80000000, LENGTH = 64K - 4096 // use cached address for fast access
}

SECTIONS
{
	.text ORIGIN(spram) : {
		KEEP(*startup.*(.text .text.*));
		*(.text .text.*)
	} > spram

	.rdata : {
		*(.rdata .rdata.*)
		*(.rodata .rodata.*)
	} > spram

	.data : {
		. = ALIGN(4);
		*(.data .data.*)
		. = ALIGN(4);
		__gp = ALIGN(16);  // Base of small data (for MIPS)
		*(.lit8)
		*(.lit4)
		*(.sdata .sdata.*)
	} > spram

	.bss (NOLOAD) : {
		. = ALIGN(4);
		__bss_start = ABSOLUTE(.);
		*(.sbss .sbss.*)
		*(.scommon)
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = ABSOLUTE(.);
	} > spram
	__bss_length = __bss_end - __bss_start;

	.stack (NOLOAD) : {
		. = ALIGN(8);
		KEEP(* (.stack .stack.*));
		__stack = ORIGIN(spram) + LENGTH(spram) - 0x20;
	} > spram

#ifndef NDEBUG
	/* DWARF debug sections.
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
