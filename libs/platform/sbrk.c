// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <lib/utils_def.h>
#include <platform_def.h>

#define HEAP_START ((const char *)heap)
#define HEAP_MAX   ((const char *)(HEAP_START + sizeof(heap)))

static unsigned char heap[PLATFORM_HEAP_SIZE]
	__attribute__((aligned(PLATFORM_HEAP_POINTER_ALIGN), section(".heap"))) = { 0 };

void *sbrk(ptrdiff_t size)
{
	/* heap_ptr is initialized to HEAP_START */
	static const char *heap_ptr = HEAP_START;
	const char *old_heap_ptr = NULL;

	if (HEAP_START == HEAP_MAX) {
		/* No Heap provided */
		errno = ENOMEM;
		return (void *)-1;
	}

	old_heap_ptr = heap_ptr;
	size = ALIGN_UP(size, PLATFORM_HEAP_POINTER_ALIGN);

	if ((heap_ptr + size) > HEAP_MAX) {
		/* top of heap is bigger than HEAP_MAX */
		errno = ENOMEM;
		return (void *)-1;
	}

	/* success: update heap_ptr and return previous value */
	heap_ptr += size;

	return (void *)old_heap_ptr;
}
