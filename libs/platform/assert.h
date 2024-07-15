// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <sys/cdefs.h>

#include <platform_def.h>
#include <log.h>

#ifndef PLAT_LOG_LEVEL_ASSERT
#define PLAT_LOG_LEVEL_ASSERT PLAT_LOG_LEVEL
#endif

#ifdef ENABLE_ASSERTIONS
#if PLAT_LOG_LEVEL_ASSERT >= LOG_LEVEL_VERBOSE
void __dead2 __assert(const char *file, int line, const char *assertion);
#define assert(e) ((e) ? (void)0 : __assert(__FILE__, __LINE__, #e))
#elif PLAT_LOG_LEVEL_ASSERT >= LOG_LEVEL_INFO
void __dead2 __assert(const char *file, int line);
#define assert(e) ((e) ? (void)0 : __assert(__FILE__, __LINE__))
#else
void __dead2 __assert(void);
#define assert(e) ((e) ? (void)0 : __assert())
#endif //PLAT_LOG_LEVEL_ASSERT
#else
void __dead2 __assert(void);
#define assert(e) ((e) ? (void)0 : __assert())
#endif /* ENABLE_ASSERTIONS */

#endif /* __ASSERT_H__ */
