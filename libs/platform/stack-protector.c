// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <sys/types.h>

#include <libs/log.h>
#include <libs/platform-def-common.h>

/*
 * Canary value used by the compiler runtime checks to detect stack corruption.
 *
 * Force the canary to be in .stack to allow predictable memory layout relatively
 * to the stacks.
 */
register_t __stack_chk_guard
	__attribute__((section(".stack"))) = (register_t)(PLAT_RANDOM_CANARY_VALUE);

/*
 * Function called when the stack's canary check fails, which means the stack
 * was corrupted. It must not return.
 */
void __dead2 __stack_chk_fail(void)
{
	ERROR("Stack corruption detected\n");
	while (1)
		; // Unreachable
}
