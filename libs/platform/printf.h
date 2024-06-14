// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __UART_PPRINTF__
#define __UART_PPRINTF__

#include "drivers/uart.h"

int uart_printf(uart_param_t *uart, const char *fmt, ...);

#endif /* __UART_PPRINTF__ */
