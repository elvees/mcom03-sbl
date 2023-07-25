// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

OUTPUT_ARCH("mips")
TARGET("elf32-littlemips")
GROUP(-lgcc)

ENTRY(_startup)

MEMORY
{
	spram (rwx)  : ORIGIN = 0xa0000000, LENGTH = 0xF000
}

SECTIONS
{
	.text ORIGIN(spram) : {
		*(.text)
		*(.text.*)
		. = ALIGN(4);
	} > spram

	.data : {
		*(.rodata)
		*(.rodata.*)
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		_gp = ALIGN(16);  /* Base of small data (for MIPS) */
		*(.lit8)
		*(.lit4)
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(4);
	} > spram

	.sbss (NOLOAD) : {
		__bss_start = ABSOLUTE(.);
		*(.sbss)
		*(.sbss.*)
		*(.scommon)
	} > spram

	.bss (NOLOAD) : {
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = ABSOLUTE(.);
		__bss_length = ABSOLUTE(.) - __bss_start;
	} > spram

	_stack = ORIGIN(spram) + LENGTH(spram) - 0x20;

	/DISCARD/ : {
		*(.reginfo)
		*(.MIPS.abiflags)
		*(.comment)
		*(.note*)
		*(.eh_frame*)
		*(.debug*)
		*(.dynsym)
		*(.dynstr*)
		*(.dynamic*)
		*(.plt*)
		*(.interp*)
		*(.gnu*)
	}
}
