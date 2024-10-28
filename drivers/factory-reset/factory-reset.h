// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <libs/errors.h>

typedef struct {
	gpio_regs_t *gpio_regs;
	uint32_t port;
	uint32_t pin;
	uint32_t active_low;
} factory_reset_info_t;

static inline int factory_reset_init(const factory_reset_info_t *info)
{
	if (!info)
		return -ENULL;

	return gpio_init(info->gpio_regs, info->port, info->pin, GPIO_MODE_SW, GPIO_DIR_INPUT);
}

static inline bool factory_reset_is_pressed(const factory_reset_info_t *info)
{
	assert(info); // Null is checking in factory_reset_init()

	unsigned int state;

	gpio_read_pin(info->gpio_regs, info->port, info->pin, &state);

	return info->active_low ? (state == GPIO_PIN_RESET) : (state == GPIO_PIN_SET);
}

int factory_reset_get_info(const void *fdt, factory_reset_info_t *info);
