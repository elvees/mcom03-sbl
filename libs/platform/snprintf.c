// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>

/*
 * TODO: add formatted output for %.3s (example)
 */

#define get_num_va_args(_args, _lcount)                   \
	(((_lcount) > 1) ? va_arg(_args, long long int) : \
			   (((_lcount) == 1) ? va_arg(_args, long int) : va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)                                          \
	(((_lcount) > 1) ? va_arg(_args, unsigned long long int) :                \
			   (((_lcount) == 1) ? va_arg(_args, unsigned long int) : \
					       va_arg(_args, unsigned int)))

static void string_print(char **s, size_t n, size_t *chars_printed, const char *str)
{
	while (*str != '\0') {
		if (*chars_printed < n) {
			*(*s) = *str;
			(*s)++;
		}

		(*chars_printed)++;
		str++;
	}
}

static void unsigned_dec_print(char **s, size_t n, size_t *chars_printed,
			       unsigned long long int unum, unsigned int radix, char padc, int padn)
{
	/* Enough for a 32-bit unsigned decimal integer (4294967295). */
	char num_buf[10];
	int i = 0;
	unsigned int rem;

	do {
		rem = unum % radix;
		if (rem < 0xa)
			num_buf[i] = '0' + rem;
		else
			num_buf[i] = 'a' + (rem - 0xa);
		i++;
		unum /= radix;
	} while (unum > 0U);

	if (padn > 0) {
		while (i < padn) {
			if (*chars_printed < n) {
				*(*s) = padc;
				(*s)++;
			}
			(*chars_printed)++;
			padn--;
		}
	}

	while (--i >= 0) {
		if (*chars_printed < n) {
			*(*s) = num_buf[i];
			(*s)++;
		}
		(*chars_printed)++;
	}
}

/*******************************************************************
 * Reduced snprintf to be used for Trusted firmware.
 * The following type specifiers are supported:
 *
 * %d or %i - signed decimal format
 * %s - string format
 * %u - unsigned decimal format
 *
 * The function panics on all other formats specifiers.
 *
 * It returns the number of characters that would be written if the
 * buffer was big enough. If it returns a value lower than n, the
 * whole string has been written.
 *******************************************************************/
int snprintf(char *s, size_t n, const char *fmt, ...)
{
	va_list args;
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;
	char padc = '\0'; /* Padding character */
	int padn; /* Number of characters to pad */
	size_t chars_printed = 0U;

	if (n == 0U) {
		/* There isn't space for anything. */
	} else if (n == 1U) {
		/* Buffer is too small to actually write anything else. */
		*s = '\0';
		n = 0U;
	} else {
		/* Reserve space for the terminator character. */
		n--;
	}

	va_start(args, fmt);
	while (*fmt != '\0') {
		l_count = 0;
		padn = 0;
		if (*fmt == '%') {
			fmt++;
			/* Check the format specifier. */
loop:
			switch (*fmt) {
			case 'i':
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					if (chars_printed < n) {
						*s = '-';
						s++;
					}
					chars_printed++;

					unum = (unsigned long long int)-num;
					padn--;
				} else {
					unum = (unsigned long long int)num;
				}

				unsigned_dec_print(&s, n, &chars_printed, unum, 10, padc, padn);
				break;
			case 's':
				str = va_arg(args, char *);
				string_print(&s, n, &chars_printed, str);
				break;
			case 'p':
				unum = (uintptr_t)va_arg(args, void *);
				if (unum > 0U) {
					string_print(&s, n, &chars_printed, "0x");
					padn -= 2;
				}

				unsigned_dec_print(&s, n, &chars_printed, unum, 16, padc, padn);
				break;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				unsigned_dec_print(&s, n, &chars_printed, unum, 16, padc, padn);
				break;
			case 'z':
				if (sizeof(size_t) == 8U)
					l_count = 2;
				fmt++;
				goto loop;
			case 'l':
				l_count++;
				fmt++;
				goto loop;
			case 'u':
				unum = get_unum_va_args(args, l_count);
				unsigned_dec_print(&s, n, &chars_printed, unum, 10, padc, padn);
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;

				for (;;) {
					char ch = *fmt;
					if ((ch < '0') || (ch > '9')) {
						goto loop;
					}
					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
			default:
				/* Panic on any other format specifier. */
				assert(0); /* Unreachable */
			}
			fmt++;
			continue;
		}

		if (chars_printed < n) {
			*s = *fmt;
			s++;
		}

		fmt++;
		chars_printed++;
	}

	va_end(args);

	if (n > 0U)
		*s = '\0';

	return (int)chars_printed;
}
