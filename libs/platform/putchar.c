// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <drivers/console/console.h>

int putchar(int c)
{
	return console_putchar(c);
}
