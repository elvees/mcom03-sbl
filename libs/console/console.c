// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdlib.h>
#include <platform_def.h>
#include <mcom03.h>
#include <mcom03_errors.h>
#include "console.h"

struct fb_param {
	unsigned int p1;
};

struct console_handler {
	unsigned int type;
	uart_param_t huart;
	struct fb_param p_fb;
};

static struct console_handler console_handler;

int console_putchar(int c)
{
	int rc = MCOM03_SUCCESS;

#ifdef USE_UART_CONSOLE
	rc = uart_drv_putchar(&console_handler.huart, c);
#endif

#ifdef USE_FRAMEBUFFER_CONSOLE
/* TODO: make FRAMEBUFFER console or need to restruct console
             driver with console registrator instance */
#endif

	return rc;
}

/**
 * console_init
 *
 * Initialization of UART or other console input/output
 *
 * @param hcon Console handler for current configuration.
 *
 * @return rc
 *
 */
static int console_init(struct console_handler *hcon)
{
	int rc = MCOM03_SUCCESS;

#ifdef USE_UART_CONSOLE
	hcon->huart.uartNum = CONSOLE_OUTPUT_NUM;
	rc = uart_hw_init(&hcon->huart);
	if (rc != MCOM03_SUCCESS) {
		return rc;
	}
	rc = uart_drv_config_default(&hcon->huart);
	if (rc != MCOM03_SUCCESS) {
		return rc;
	}
#endif

#ifdef USE_FRAMEBUFFER_CONSOLE
/* TODO: make FRAMEBUFFER console or need to restruct console
             driver with console registrator instance */
#endif

	return rc;
}

/**
 * console_init_common
 *
 * Initialization for entrypoint
 *
 * @param console_type Console type: UART, framebuffer or other.
 *
 * @return rc
 *
 */
int console_init_common(register unsigned int console_type)
{
	int rc = MCOM03_SUCCESS;
	switch (console_type) {
	case CONSOLE_OUTPUT_UART:
		rc = console_init(&console_handler);
		break;
	case CONSOLE_OUTPUT_FB:
	case CONSOLE_OUTPUT_MIX:
	case CONSOLE_OUTPUT_NONE:
	default:
		rc = MCOM03_ERROR_NOT_SUPPORTED;
		break;
	}

	return rc;
}

int console_getchar(void)
{
	int rc = MCOM03_SUCCESS;
	int c = '\0';

#ifdef USE_UART_CONSOLE
	rc = uart_drv_getchar(&console_handler.huart, &c);
	if (rc != MCOM03_SUCCESS) {
		return 0;
	}
#endif
	return c;
}

int console_flush(void)
{
	int rc = MCOM03_SUCCESS;

#ifdef USE_UART_CONSOLE
	rc = uart_drv_flush(&console_handler.huart);
#endif

	return rc;
}

int console_deinit(void)
{
	int rc = MCOM03_SUCCESS;

#ifdef USE_UART_CONSOLE
	rc = uart_drv_deinit(&console_handler.huart);
#endif

	return rc;
}

/* TODO: ADD CRASH CONSOLE! */
