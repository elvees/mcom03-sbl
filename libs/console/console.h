// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

// TODO: add crash_console_* !

typedef struct console_ops {
	int (*init)(void *);
	int (*putchar)(void *, char);
	int (*getchar)(void *, int *);
	int (*flush)(void *);
	int (*deinit)(void *);
} console_ops_t;

typedef struct console_t {
	void *hw;
	console_ops_t *ops;
} console_t;

int console_register(console_t *console);

int console_init(void);
int console_putchar(int c);
int console_getchar(void);
int console_flush(void);
int console_deinit(void);
