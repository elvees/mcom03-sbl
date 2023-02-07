// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "uart-printf.h"

#define get_num_va_args(_args, _lcount)                   \
	(((_lcount) > 1) ? va_arg(_args, long long int) : \
			   (((_lcount) == 1) ? va_arg(_args, long int) : va_arg(_args, int)))

#define get_unum_va_args(_args, _lcount)                                          \
	(((_lcount) > 1) ? va_arg(_args, unsigned long long int) :                \
			   (((_lcount) == 1) ? va_arg(_args, unsigned long int) : \
					       va_arg(_args, unsigned int)))

static int string_print(uart_param_t *uart, const char *str)
{
	int count = 0;

	for (; *str != '\0'; str++) {
		(void)uart_drv_putchar(uart, *str);
		count++;
	}

	return count;
}

static int unsigned_num_print(uart_param_t *uart, unsigned long long int unum, unsigned int radix,
			      char padc, int padn, unsigned char is_upper)
{
	/* Just need enough space to store 64 bit decimal integer */
	char num_buf[20];
	int i = 0, count = 0;
	unsigned int rem;

	do {
		rem = unum % radix;
		if (rem < 0xa)
			num_buf[i] = '0' + rem;
		else
			num_buf[i] = (is_upper ? 'A' : 'a') + (rem - 0xa);

		i++;
		unum /= radix;
	} while (unum > 0U);

	if (padn > 0) {
		while (i < padn) {
			(void)uart_drv_putchar(uart, padc);
			count++;
			padn--;
		}
	}

	while (--i >= 0) {
		(void)uart_drv_putchar(uart, num_buf[i]);
		count++;
	}

	return count;
}

/*******************************************************************
 * Reduced format print for Trusted firmware.
 * The following type specifiers are supported by this print
 * %x - hexadecimal format
 * %s - string format
 * %d or %i - signed decimal format
 * %u - unsigned decimal format
 * %p - pointer format
 *
 * The following length specifiers are supported by this print
 * %l - long int (64-bit on AArch64)
 * %ll - long long int (64-bit on AArch64)
 * %z - size_t sized integer formats (64 bit on AArch64)
 *
 * The following padding specifiers are supported by this print
 * %0NN - Left-pad the number with 0s (NN is a decimal number)
 *
 * The print exits on all other formats specifiers other than valid
 * combinations of the above specifiers.
 *******************************************************************/
int uart_vprintf(uart_param_t *uart, const char *fmt, va_list args)
{
	int l_count;
	long long int num;
	unsigned long long int unum;
	char *str;
	char padc = '\0'; /* Padding character */
	int padn; /* Number of characters to pad */
	int count = 0; /* Number of printed characters */

	while (*fmt != '\0') {
		l_count = 0;
		padn = 0;

		if (*fmt == '%') {
			fmt++;
			/* Check the format specifier */
loop:
			switch (*fmt) {
			case 'i': /* Fall through to next one */
			case 'd':
				num = get_num_va_args(args, l_count);
				if (num < 0) {
					(void)uart_drv_putchar(uart, '-');
					unum = (unsigned long long int)-num;
					padn--;
				} else
					unum = (unsigned long long int)num;

				count += unsigned_num_print(uart, unum, 10, padc, padn, 0);
				break;
			case 's':
				str = va_arg(args, char *);
				count += string_print(uart, str);
				break;
			case 'c':
				num = get_num_va_args(args, l_count);
				(void)uart_drv_putchar(uart, num);
				count++;
				break;
			case 'p':
				unum = (uintptr_t)va_arg(args, void *);
				if (unum > 0U) {
					count += string_print(uart, "0x");
					padn -= 2;
				}

				count += unsigned_num_print(uart, unum, 16, padc, padn, 0);
				break;
			case 'x':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(uart, unum, 16, padc, padn, 0);
				break;
			case 'X':
				unum = get_unum_va_args(args, l_count);
				count += unsigned_num_print(uart, unum, 16, padc, padn, 1);
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
				count += unsigned_num_print(uart, unum, 10, padc, padn, 0);
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
				/* Exit on any other format specifier */
				return -1;
			}
			fmt++;
			continue;
		}
		(void)uart_drv_putchar(uart, *fmt);
		fmt++;
		count++;
	}

	return count;
}

int uart_printf(uart_param_t *uart, const char *fmt, ...)
{
	int count;
	va_list va;

	va_start(va, fmt);
	count = uart_vprintf(uart, fmt, va);
	va_end(va);

	return count;
}
