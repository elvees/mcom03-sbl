/*
 * Copyright 2020 RnD Center "ELVEES", JSC
 */

SECTIONS
{
	. = 0x50000000;
	.text : {
		*(.text.boot*)
		*(.text*)
	}

	. = ALIGN(8);

	.rodata : {
		__ddrinit_start = .;
		ddrinit.o (.rodata*);
		__ddrinit_end = .;

		. = ALIGN(8);

		__tfa_start = .;
		tfa.o (.rodata*);
		__tfa_end = .;

		. = ALIGN(8);

		__uboot_start = .;
		u-boot.o (.rodata*);
		__uboot_end = .;
	}

	_end = .;

	/DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) *(.debug*) }
	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.plt*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
}
