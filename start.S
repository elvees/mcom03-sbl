/*
 * Copyright 2020 RnD Center "ELVEES", JSC
 */

.section ".text.boot"

.global _start

_start:
	ldr	x1, =0xfff0

	mov	sp, x1
	bl	main
