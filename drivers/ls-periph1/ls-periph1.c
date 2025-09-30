// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <drivers/pll/pll.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>

#include "ls-periph1.h"

// LSP1 Subsystem PLL output frequency is 614.250 MHz, assuming that XTI = 27 MHz
static struct ucg_channel lsp1_ucg_channels[] = {
	{ 0, LSP1_UCG_CLK_SYS, 7 }, // 87.750 MHz
	{ 0, LSP1_UCG_CLK_GPIO1, 585 }, //  1.050 MHz
	{ 0, LSP1_UCG_CLK_UART0, 23 }, // ~26.71 MHz
	{ 0, LSP1_UCG_CLK_TIMERS, 7 }, // 87.750 MHz
};

static int lsp1_cfg_clks(void)
{
	int ret;

	ucg_regs_t *ucg = ucg_get_registers(UCG_SUBSYS_LSP1, 0);
	lsp1_urb_regs_t *urb = lsp1_get_urb_registers();

	ret = ucg_enable_bp(ucg, LSP1_UCG_ALL_CH_MASK);
	if (ret)
		return ret;

	/**
	 * Setup PLL to 614.250 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 90;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 3;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)&urb->pll_cfg, &pll_cfg, 1000);
	if (ret)
		return ret;

	// Set dividers
	for (int i = 0; i < ARRAY_SIZE(lsp1_ucg_channels); i++) {
		ret = ucg_set_divider(ucg, lsp1_ucg_channels[i].chan_id, lsp1_ucg_channels[i].div,
		                      1000);
		if (ret)
			return ret;
	}

	return ucg_sync_and_disable_bp(ucg, LSP1_UCG_ALL_CH_MASK, LSP1_UCG_SYNC_MASK);
}

lsp1_urb_regs_t *lsp1_get_urb_registers(void)
{
	return (lsp1_urb_regs_t *)BASE_ADDR_LSP1_URB_BASE;
}

int lsp1_enable(void)
{
	/**
	 * Initialize and configure the LS Peripheral 1 clocking system.
	 * Release reset signal of LS Peripheral 1.
	 */
	service_enable_lsp1();

	return lsp1_cfg_clks();
}

int lsp1_pad_cfg(unsigned int port, unsigned int pin, unsigned int value)
{
	lsp1_urb_regs_t *urb = lsp1_get_urb_registers();

	switch (port) {
	case GPIO_PORTA:
		mmio_write_32((uintptr_t)&urb->gpio1_port_a_pad_ctr[pin], value);
		break;
	case GPIO_PORTB:
		mmio_write_32((uintptr_t)&urb->gpio1_port_b_pad_ctr[pin], value);
		break;
	case GPIO_PORTC:
		mmio_write_32((uintptr_t)&urb->gpio1_port_c_pad_ctr[pin], value);
		break;
	case GPIO_PORTD:
		mmio_write_32((uintptr_t)&urb->gpio1_port_d_pad_ctr[pin], value);
		break;
	default:
		return -EINVALIDPARAM;
	}

	return 0;
}
