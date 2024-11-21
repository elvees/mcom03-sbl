// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

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

int gpio_init(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int mode,
              unsigned int direction)
{
	if (!gpio)
		return -ENULL;

	// Set pin direction
	if (mode == GPIO_MODE_SW)
		GPIO_SET_BIT(gpio->swport[port].ddr, pin, direction);

	// Set pin mode
	GPIO_SET_BIT(gpio->swport[port].ctl, pin, mode);

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
