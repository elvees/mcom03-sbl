// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __FACTORY_RESET_H__
#define __FACTORY_RESET_H__

#include <drivers/gpio/gpio.h>
#include <lib/libfdt/libfdt.h>
#include <lib/fdt_helpers/fdt_helpers.h>

typedef struct {
	gpio_regs_t *gpio_regs;
	uint32_t port;
	uint32_t pin;
	uint32_t active_low;
} factory_reset_info_t;

static inline int factory_reset_init(const factory_reset_info_t *info)
{
	if (!info)
		return -1;

	return gpio_init(info->gpio_regs, info->port, info->pin, GPIO_MODE_SW, GPIO_DIR_INPUT);
}

static inline int factory_reset_is_pressed(const factory_reset_info_t *info)
{
	int state;

	if (!info)
		return -1;

	state = gpio_read_pin(info->gpio_regs, info->port, info->pin);
	return info->active_low ? (state == GPIO_PIN_RESET) : (state == GPIO_PIN_SET);
}

int factory_reset_get_info(const void *fdt, factory_reset_info_t *info);

#endif /* __FACTORY_RESET_H__ */
