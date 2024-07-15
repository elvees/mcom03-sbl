// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <drivers/uart/uart.h>

/*TODO: add crash_console_* !*/

int console_init_common(register unsigned int console_type);
int console_putchar(int c);
int console_getchar(void);
int console_flush(void);
int console_deinit(void);

#endif /* __CONSOLE_H__ */
