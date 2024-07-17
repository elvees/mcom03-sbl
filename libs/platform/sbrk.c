// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <libs/platform-def-common.h>
#include <libs/utils-def.h>

static uintptr_t heap_start;
static uintptr_t heap_end;
static const char *heap_ptr;

void heap_init(uintptr_t start, uintptr_t end)
{
	heap_start = start;
	heap_end = end;

	heap_ptr = (const char *)heap_start;
}

void *sbrk(ptrdiff_t size)
{
	const char *old_heap_ptr = NULL;

	if (heap_start == heap_end) {
		// No Heap provided
		errno = ENOMEM;
		return (void *)-1;
	}

	old_heap_ptr = heap_ptr;

	size = ALIGN_UP(size, PLAT_HEAP_POINTER_ALIGN);

	if ((uintptr_t)(heap_ptr + size) > heap_end) {
		// top of heap is bigger than heap_max
		errno = ENOMEM;
		return (void *)-1;
	}

	// success: update heap_ptr and return previous value
	heap_ptr += size;

	return (void *)old_heap_ptr;
}
