// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdarg.h>
#include <stdio.h>

#include "format-parser.h"

/**
 * Reduced format print.
 * The following type specifiers are supported:
 *
 * %d or %i - signed decimal format
 * %s - string format
 * %c - character format
 * %p - pointer format
 * %x - hexadecimal format (lower case)
 * %X - hexadecimal format (upper case)
 * %u - unsigned decimal format
 *
 * The following length specifiers are supported
 * %l - long int
 * %ll - long long int
 * %z - size_t sized integer formats
 *
 * The following padding specifiers are supported by this print
 * %0NN - Left-pad the number with 0s (NN is a decimal number)
 *
 * The function exits on all other formats specifiers.
 */
int printf(const char *fmt, ...)
{
	int chars_printed; // Number of printed characters
	va_list args;

	va_start(args, fmt);
	chars_printed = format_specifier_parser(NULL, 0, fmt, args);
	va_end(args);

	return chars_printed;
}
