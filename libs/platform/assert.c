// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdio.h>
#include <platform_helpers.h>
#include "assert.h"

/*
 * Only print the output if PLAT_LOG_LEVEL_ASSERT is higher or equal to
 * LOG_LEVEL_INFO, which is the default value for builds with DEBUG=1.
 */

#ifdef ENABLE_ASSERTIONS

#if PLAT_LOG_LEVEL_ASSERT >= LOG_LEVEL_VERBOSE
void __dead2 __assert(const char *file, int line, const char *assertion)
{
	printf("ASSERT: %s:%d:%s\r\n", file, line, assertion);
	(void)console_flush();
	infinity_loop();
}
#elif PLAT_LOG_LEVEL_ASSERT >= LOG_LEVEL_INFO
void __dead2 __assert(const char *file, int line)
{
	printf("ASSERT: %s:%dr\r\n", file, line);
	(void)console_flush();
	infinity_loop();
}
#else
void __dead2 __assert(void)
{
	(void)console_flush();
	infinity_loop();
}
#endif //PLAT_LOG_LEVEL_ASSERT

#else
void __dead2 __assert(void)
{
	(void)console_flush();
	infinity_loop();
}
#endif /* ENABLE_ASSERTIONS */
