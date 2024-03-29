// SPDX-License-Identifier: MIT
// Copyright 2022 RnD Center "ELVEES", JSC

OUTPUT_ARCH("mips")
GROUP(-lgcc)

ENTRY(_start)

MEMORY
{
	xip0 (rwx) : ORIGIN = 0x00000000, LENGTH = 0x1000000
	ram (rwx)  : ORIGIN = 0x9fa00000, LENGTH = 0x8000
}

SECTIONS
{
	.text ORIGIN(xip0) : {
		*(.text.boot)
		*(.text)
		*(.text.*)
		. = ALIGN(4);
		__ddrinit_start = ABSOLUTE(.);
		KEEP(ddrinit.o (.rodata*));
		__ddrinit_end = ABSOLUTE(.);
		. = ALIGN(4);
		__dtb_start = ABSOLUTE(.);
		KEEP(dtb.o (.rodata*));
		__dtb_end = ABSOLUTE(.);
		. = ALIGN(4);
		__tfa_start = ABSOLUTE(.);
		KEEP(tfa.o (.rodata*));
		__tfa_end = ABSOLUTE(.);
		. = ALIGN(4);
#if defined(HAS_BL32)
		__bl32_start = ABSOLUTE(.);
		KEEP(bl32.o (.rodata*));
		__bl32_end = ABSOLUTE(.);
		. = ALIGN(4);
#endif
		__uboot_start = ABSOLUTE(.);
		KEEP(u-boot.o (.rodata*));
		__uboot_end = ABSOLUTE(.);
		. = ALIGN(4);
	} > xip0

	.rodata : {
		*(.rodata)
		*(.rodata.*)
		. = ALIGN(4);
	} > xip0

	__text_end = ABSOLUTE(.);

	.data : AT (__text_end) {
		__data_start = ABSOLUTE(.);
		*(.data)
		*(.data.*)
		_gp = ALIGN(16);  /* Base of small data (for MIPS) */
		. = ALIGN(4);
		*(.sdata)
		*(.sdata.*)
		. = ALIGN(4);
		__data_end = ABSOLUTE(.);
	} > ram

	.sbss (NOLOAD) : {
		__bss_start = ABSOLUTE(.);
		*(.sbss)
		*(.sbss.*)
		*(.scommon)
	} > ram

	.bss (NOLOAD) : {
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = ABSOLUTE(.);
		__bss_length = ABSOLUTE(.) - __bss_start;
	} > ram

	_stack = ORIGIN(ram) + LENGTH(ram) - 0x20;

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
