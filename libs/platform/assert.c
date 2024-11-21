// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stdio.h>
#include <sys/cdefs.h>

void __dead2 __wrap___assert_func(const char *file, int line, const char *func,
                                  const char *assertion)
{
	printf("Assertion failed: %s, file %s%s%s, line %d\n", assertion, file,
	       func ? ", function: " : "", func ? func : "", line);
	while (1)
		; // NOTREACHED
}
