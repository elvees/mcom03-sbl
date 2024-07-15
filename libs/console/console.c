// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stdio.h>

#include <libs/errors.h>

#include "console.h"

static console_t console_handler;

int console_register(console_t *console)
{
	if (!console)
		return -ENULL;

	console_handler.hw = console->hw;
	console_handler.ops = console->ops;

	return 0;
}

int console_putchar(int c)
{
	int rc = 0;

	if (console_handler.ops && console_handler.ops->putchar)
		rc = console_handler.ops->putchar(console_handler.hw, c);

	return rc;
}

int console_init(void)
{
	int rc = 0;

	if (console_handler.ops && console_handler.ops->init)
		rc = console_handler.ops->init(console_handler.hw);

	return rc;
}

int console_getchar(void)
{
	int rc = 0;
	int c = '\0';

	if (console_handler.ops && console_handler.ops->getchar)
		rc = console_handler.ops->getchar(console_handler.hw, &c);

	return (rc) ? EOF : c;
}

int console_flush(void)
{
	int rc = 0;

	if (console_handler.ops && console_handler.ops->flush)
		rc = console_handler.ops->flush(console_handler.hw);

	return rc;
}

int console_deinit(void)
{
	int rc = 0;

	if (console_handler.ops && console_handler.ops->deinit)
		rc = console_handler.ops->deinit(console_handler.hw);

	return rc;
}

// TODO: ADD CRASH CONSOLE!
