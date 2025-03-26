// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdarg.h>
#include <stdio.h>

// TODO: add formatted output for %.3s (example)

#define get_num_va_args(_args, _lcount)                   \
	(((_lcount) > 1) ? va_arg(_args, long long int) : \
	                   (((_lcount) == 1) ? va_arg(_args, long int) : va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)                                          \
	(((_lcount) > 1) ? va_arg(_args, unsigned long long int) :                \
	                   (((_lcount) == 1) ? va_arg(_args, unsigned long int) : \
	                                       va_arg(_args, unsigned int)))

static void char_print(char **s, size_t n, size_t *chars_printed, const char ch)
{
	if (*chars_printed < n) {
		*(*s) = ch;
		(*s)++;
	}

	(*chars_printed)++;
}

static void string_print(char **s, size_t n, size_t *chars_printed, const char *str)
{
	for (; *str != '\0'; str++)
		char_print(s, n, chars_printed, *str);
}

static void unsigned_num_print(char **s, size_t n, size_t *chars_printed,
                               unsigned long long int unum, unsigned int radix, char padc, int padn,
                               unsigned char is_upper)
{
	// Just need enough space to store 64 bit decimal integer
	char num_buf[20];
	int i = 0;
	unsigned int rem;

	do {
		rem = unum % radix;
		if (rem < 0xa)
			num_buf[i] = '0' + rem;
		else
			num_buf[i] = (is_upper ? 'A' : 'a') + (rem - 0xa);

		i++;
		unum /= radix;
	} while (unum > 0);

	if (padn > 0)
		while (i < padn) {
			char_print(s, n, chars_printed, padc);
			padn--;
		}

	while (--i >= 0)
		char_print(s, n, chars_printed, num_buf[i]);
}

static int format_specifier_parser(char **s, size_t n, const char *fmt, va_list args)
{
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;
	char padc = '\0'; // Padding character
	int padn; // Number of characters to pad
	size_t chars_printed = 0; // Number of printed characters

	while (*fmt != '\0') {
		l_count = 0;
		padn = 0;

		if (*fmt == '%') {
			fmt++;
			// Check the format specifier
loop:
			switch (*fmt) {
			case 'i':
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					char_print(s, n, &chars_printed, '-');

					unum = (unsigned long long int)-num;
					padn--;
				} else {
					unum = (unsigned long long int)num;
				}

				unsigned_num_print(s, n, &chars_printed, unum, 10, padc, padn, 0);
				break;
			case 's':
				str = va_arg(args, char *);
				string_print(s, n, &chars_printed, str);
				break;
			case 'c':
				num = get_num_va_args(args, l_count);
				char_print(s, n, &chars_printed, num);
				break;
			case 'p':
				unum = (uintptr_t)va_arg(args, void *);
				if (unum > 0U) {
					string_print(s, n, &chars_printed, "0x");
					padn -= 2;
				}

				unsigned_num_print(s, n, &chars_printed, unum, 16, padc, padn, 0);
				break;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				unsigned_num_print(s, n, &chars_printed, unum, 16, padc, padn, 0);
				break;
			case 'X':
				unum = get_unum_va_args(args, l_count);
				unsigned_num_print(s, n, &chars_printed, unum, 16, padc, padn, 1);
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
				unsigned_num_print(s, n, &chars_printed, unum, 10, padc, padn, 0);
				break;
			case '0':
				padc = '0';
				padn = 0;
				fmt++;

				for (;;) {
					char ch = *fmt;
					if ((ch < '0') || (ch > '9'))
						goto loop;

					padn = (padn * 10) + (ch - '0');
					fmt++;
				}
			default:
				// Exit on any other format specifier
				return -1;
			}
			fmt++;
			continue;
		}

		char_print(s, n, &chars_printed, *fmt);
		fmt++;
	}

	return chars_printed;
}

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
