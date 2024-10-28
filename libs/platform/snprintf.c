// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdarg.h>
#include <stdio.h>

#include "format-parser.h"

// TODO: add formatted output for %.3s (example)

/**
 * Reduced snprintf.
 * The following type specifiers are supported:
 *
 * %d or %i - signed decimal format
 * %s - string format
 * %c - character foramt
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
 *
 * It returns the number of characters that would be written if the
 * buffer was big enough. If it returns a value lower than n, the
 * whole string has been written.
 */
int snprintf(char *s, size_t n, const char *fmt, ...)
{
	int chars_printed; // Number of printed characters
	va_list args;

	switch (n) {
	case 0: // There isn't space for anything
		break;
	case 1: // Buffer is too small to actually write anything else
		*s = '\0';
		n = 0;
		break;
	default: // Reserve space for the terminator character
		n--;
	}

	va_start(args, fmt);
	chars_printed = format_specifier_parser(&s, n, fmt, args);
	va_end(args);

	// Place a NULL character at the end of the buffer
	if (n > 0)
		*s = '\0';

	return chars_printed;
}
