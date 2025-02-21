// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include <drivers/mcom03-regs.h>
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

static int gpio_enable_lsp(gpio_regs_t *gpio)
{
	static int lsp0_is_enabled = 0;
	static int lsp1_is_enabled = 0;

	if (!gpio)
		return -ENULL;

	/**
	 * Initialize and configure the LS Peripheral [0,1] clocking system.
	 * Release reset signal of LS Peripheral [0,1].
	 */
	switch ((uintptr_t)gpio) {
	case BASE_ADDR_LSP0_GPIO0_BASE:
		if (!lsp0_is_enabled) {
			service_enable_lsp0();
			lsp0_is_enabled = !lsp0_is_enabled;
		}
		break;
	case BASE_ADDR_LSP1_GPIO1_BASE:
		if (!lsp1_is_enabled) {
			service_enable_lsp1();
			lsp1_is_enabled = !lsp1_is_enabled;
		}
		break;
	default:
		return -EINVALIDPARAM;
	}

	return 0;
}

static int gpio_cfg_clks(gpio_regs_t *gpio)
{
	int ret;

	static int lsp0_is_cfg = 0;
	static int lsp1_is_cfg = 0;

	ucg_regs_t *ucg;

	switch ((uintptr_t)gpio) {
	case BASE_ADDR_LSP0_GPIO0_BASE:
		if (!lsp0_is_cfg) {
			// Initialize the UCG register for clocking GPIO0
			ucg = ucg_get_registers(UCG_SUBSYS_LSP0, 0);

			ret = ucg_enable_bp(ucg, LSP0_UCG2_ALL_CH_MASK);
			if (ret)
				return ret;
			ret = ucg_set_divider(ucg, LSP0_UCG2_CLK_GPIO0, 1, 1000);
			if (ret)
				return ret;
			ret = ucg_sync_and_disable_bp(ucg, LSP0_UCG2_ALL_CH_MASK,
			                              LSP0_UCG2_SYNC_MASK);
			if (ret)
				return ret;

			lsp0_is_cfg = !lsp0_is_cfg;
		}
		break;
	case BASE_ADDR_LSP1_GPIO1_BASE:
		if (!lsp1_is_cfg) {
			// Initialize the UCG register for clocking GPIO1
			ucg = ucg_get_registers(UCG_SUBSYS_LSP1, 0);

			ret = ucg_enable_bp(ucg, LSP1_UCG_ALL_CH_MASK);
			if (ret)
				return ret;
			ret = ucg_set_divider(ucg, LSP1_UCG_CLK_GPIO1, 1, 1000);
			if (ret)
				return ret;
			ret = ucg_sync_and_disable_bp(ucg, LSP1_UCG_ALL_CH_MASK,
			                              LSP1_UCG_SYNC_MASK);
			if (ret)
				return ret;

			lsp1_is_cfg = !lsp1_is_cfg;
		}
		break;
	default:
		return -EINVALIDPARAM;
	}

	return 0;
}

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

	ret = gpio_enable_lsp(gpio);
	if (ret)
		return ret;

	ret = gpio_cfg_clks(gpio);
	if (ret)
		return ret;

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
