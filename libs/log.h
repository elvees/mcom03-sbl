// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>
#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_NONE
#endif

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

#define LOG_LEVEL_NONE    0
#define LOG_LEVEL_ERROR   10
#define LOG_LEVEL_NOTICE  20
#define LOG_LEVEL_WARNING 30
#define LOG_LEVEL_INFO    40
#define LOG_LEVEL_VERBOSE 50

/*
 * Define Log Markers corresponding to each log level which will
 * be embedded in the format string and is expected by printf() to determine
 * the log level.
 */
#define LOG_MARKER_ERROR   "ERROR: "
#define LOG_MARKER_NOTICE  "NOTICE: "
#define LOG_MARKER_WARNING "WARN: "
#define LOG_MARKER_INFO    "INFO: "
#define LOG_MARKER_VERBOSE "VERB: "

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

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#define ERROR(fmt, ...) printf(LOG_MARKER_ERROR fmt, ##__VA_ARGS__)
#else
#define ERROR(fmt, ...) no_printf(LOG_MARKER_ERROR fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL >= LOG_LEVEL_NOTICE
#define NOTICE(fmt, ...) printf(LOG_MARKER_NOTICE fmt, ##__VA_ARGS__)
#else
#define NOTICE(fmt, ...) no_printf(LOG_MARKER_NOTICE fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#define WARN(fmt, ...) printf(LOG_MARKER_WARNING fmt, ##__VA_ARGS__)
#else
#define WARN(fmt, ...) no_printf(LOG_MARKER_WARNING fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#define INFO(fmt, ...) printf(LOG_MARKER_INFO fmt, ##__VA_ARGS__)
#else
#define INFO(fmt, ...) no_printf(LOG_MARKER_INFO fmt, ##__VA_ARGS__)
#endif

#if LOG_LEVEL >= LOG_LEVEL_VERBOSE
#define VERBOSE(fmt, ...) printf(LOG_MARKER_VERBOSE fmt, ##__VA_ARGS__)
#else
#define VERBOSE(fmt, ...) no_printf(LOG_MARKER_VERBOSE fmt, ##__VA_ARGS__)
#endif
