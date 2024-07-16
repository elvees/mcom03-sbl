// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __LOG_H__
#define __LOG_H__

#include <lib/utils_def.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <drivers/console/console.h>
#include "platform_def.h"

/*
 * The log output macros print output to the console. These macros produce
 * compiled log output only if the LOG_LEVEL defined in the makefile (or the
 * make command line) is greater or equal than the level required for that
 * type of log output.
 *
 * The format expected is the same as for printf(). For example:
 * INFO("Info %s.\n", "message")    -> INFO:    Info message.
 * WARN("Warning %s.\n", "message") -> WARNING: Warning message.
 */

#define LOG_LEVEL_NONE	  U(0)
#define LOG_LEVEL_ERROR	  U(10)
#define LOG_LEVEL_NOTICE  U(20)
#define LOG_LEVEL_WARNING U(30)
#define LOG_LEVEL_INFO	  U(40)
#define LOG_LEVEL_VERBOSE U(50)

/*
 * Define Log Markers corresponding to each log level which will
 * be embedded in the format string and is expected by printf() to determine
 * the log level.
 */
#define LOG_MARKER_ERROR   "\033[41mERROR\033[00m: "
#define LOG_MARKER_NOTICE  "\033[42mNOTICE\033[00m: "
#define LOG_MARKER_WARNING "\033[43mWARN\033[00m: "
#define LOG_MARKER_INFO	   "\033[42mINFO\033[00m: "
#define LOG_MARKER_VERBOSE "\033[42mVERB\033[00m: "

/*
 * If the log output is too low then this macro is used in place of printf()
 * below. The intent is to get the compiler to evaluate the function call for
 * type checking and format specifier correctness but let it optimize it out.
 */
#define no_printf(fmt, ...)                           \
	do {                                          \
		if (false) {                          \
			printf((fmt), ##__VA_ARGS__); \
		}                                     \
	} while (false)

#if PLAT_LOG_LEVEL >= LOG_LEVEL_ERROR
#define ERROR(fmt, ...) printf(LOG_MARKER_ERROR fmt, ##__VA_ARGS__)
#else
#define ERROR(fmt, ...) no_printf(LOG_MARKER_ERROR fmt, ##__VA_ARGS__)
#endif

#if PLAT_LOG_LEVEL >= LOG_LEVEL_NOTICE
#define NOTICE(fmt, ...) printf(LOG_MARKER_NOTICE fmt, ##__VA_ARGS__)
#else
#define NOTICE(fmt, ...) no_printf(LOG_MARKER_NOTICE fmt, ##__VA_ARGS__)
#endif

#if PLAT_LOG_LEVEL >= LOG_LEVEL_WARNING
#define WARN(fmt, ...) printf(LOG_MARKER_WARNING fmt, ##__VA_ARGS__)
#else
#define WARN(fmt, ...) no_printf(LOG_MARKER_WARNING fmt, ##__VA_ARGS__)
#endif

#if PLAT_LOG_LEVEL >= LOG_LEVEL_INFO
#define INFO(fmt, ...) printf(LOG_MARKER_INFO fmt, ##__VA_ARGS__)
#else
#define INFO(fmt, ...) no_printf(LOG_MARKER_INFO fmt, ##__VA_ARGS__)
#endif

#if PLAT_LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define VERBOSE(fmt, ...) printf(LOG_MARKER_VERBOSE fmt, ##__VA_ARGS__)
#else
#define VERBOSE(fmt, ...) no_printf(LOG_MARKER_VERBOSE fmt, ##__VA_ARGS__)
#endif

#endif /* __LOG_H__ */
