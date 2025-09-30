// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include <drivers/ls-periph0/ls-periph0.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "gpio.h"

#define GPIO_SET_BIT(reg, pin, val)         \
	({                                  \
		if (val)                    \
			reg |= BIT(pin);    \
		else                        \
			reg &= ~(BIT(pin)); \
	})

static int gpio_set_pin_mode(gpio_regs_t *gpio, unsigned int port, unsigned int pin,
                             unsigned int mode)
{
	if (!gpio)
		return -ENULL;

	// Set pin mode
	GPIO_SET_BIT(gpio->swport[port].ctl, pin, mode);

	return 0;
}

static int gpio_set_pin_dir(gpio_regs_t *gpio, unsigned int port, unsigned int pin,
                            unsigned int dir)
{
	if (!gpio)
		return -ENULL;

	// Set pin direction
	GPIO_SET_BIT(gpio->swport[port].ddr, pin, dir);

	return 0;
}

int gpio_init(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int mode,
              unsigned int dir)
{
	int ret;

	if (!gpio)
		return -ENULL;

	/**
	 * Initialize and configure the LS Peripheral [0,1] clocking system.
	 * Release reset signal of LS Peripheral [0,1].
	 */
	switch ((uintptr_t)gpio) {
	case BASE_ADDR_LSP0_GPIO0_BASE:
		lsp0_enable();
		break;
	case BASE_ADDR_LSP1_GPIO1_BASE:
		lsp1_enable();
		break;
	default:
		return -EINVALIDPARAM;
	}

	ret = gpio_set_pin_mode(gpio, port, pin, mode);
	if (ret)
		return ret;

	ret = gpio_set_pin_dir(gpio, port, pin, dir);
	if (ret)
		return ret;

	return 0;
}

int gpio_read_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int *pin_state)
{
	if (!gpio || !pin_state)
		return -ENULL;

	*pin_state = GPIO_PIN_RESET;

	if (gpio->ext_port[port] & BIT(pin))
		*pin_state = GPIO_PIN_SET;

	return 0;
}

int gpio_write_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int pin_state)
{
	if (!gpio)
		return -ENULL;

	GPIO_SET_BIT(gpio->swport[port].dr, pin, pin_state);

	return 0;
}
