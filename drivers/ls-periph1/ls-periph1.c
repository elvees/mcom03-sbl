// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <drivers/pll/pll.h>
#include <drivers/service/service.h>
#include <drivers/timer/timer.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>
#include <libs/platform-def-common.h>
#include <libs/utils-def.h>

#include "ls-periph1.h"

#define TIMERS_ID 7

// LSP1 Subsystem PLL output frequency is 614.250 MHz, assuming that XTI = 27 MHz
static struct ucg_channel lsp1_ucg_channels[] = {
	{ 0, LSP1_UCG_CLK_SYS, 7 }, // 87.750 MHz
	{ 0, LSP1_UCG_CLK_GPIO1, 585 }, //  1.050 MHz
	{ 0, LSP1_UCG_CLK_UART0, 23 }, // ~26.71 MHz
	{ 0, LSP1_UCG_CLK_TIMERS, 7 }, // 87.750 MHz
};

static void lsp1_timers_irq_handler(unsigned int id)
{
	mmio_read_32(LSP1_TIMERS_N_EOI(id - IRQ_ID_TIMERS0_INT));
}

static int lsp1_timers_get_clock(uint32_t *freq)
{
	int ret;
	uint32_t freq_div = 0;

	ucg_regs_t *ucg = ucg_get_registers(UCG_SUBSYS_LSP1, 0);
	lsp1_urb_regs_t *urb = lsp1_get_urb_registers();

	pll_cfg_t pll_cfg;
	pll_cfg.inp_freq = MCOM03_XTI_CLK_HZ;
	pll_cfg.out_freq = 0;
	ret = pll_get_freq((pll_cfg_reg_t *)&urb->pll_cfg, &pll_cfg);
	if (ret)
		return ret;

	ret = ucg_get_divider(ucg, LSP1_UCG_CLK_TIMERS, &freq_div);
	if (ret)
		return ret;

	*freq = pll_cfg.out_freq / freq_div;

	return 0;
}

static int lsp1_timers_init(uint64_t timebase_us)
{
	int ret;
	uint32_t freq, ticks;
	uint32_t val;

	ret = lsp1_enable();
	if (ret)
		return ret;

	ret = lsp1_timers_get_clock(&freq);
	if (ret)
		return ret;

	val = mmio_read_32(LSP1_TIMERS_CTRL(TIMERS_ID));
	val &= ~LSP1_TIMERS_CTRL_EN;
	val |= FIELD_PREP(LSP1_TIMERS_CTRL_EN, LSP1_TIMERS_CTRL_DISABLE);
	mmio_write_32(LSP1_TIMERS_CTRL(TIMERS_ID), val);

	ticks = (uint32_t)us_to_tick(timebase_us, freq);
	mmio_write_32(LSP1_TIMERS_LOAD_COUNT(TIMERS_ID), ~ticks);

	val = mmio_read_32(LSP1_TIMERS_CTRL(TIMERS_ID));
	val &= ~(LSP1_TIMERS_CTRL_EN | LSP1_TIMERS_CTRL_MODE | LSP1_TIMERS_CTRL_IRQ_MASK);
	val |= FIELD_PREP(LSP1_TIMERS_CTRL_EN, LSP1_TIMERS_CTRL_ENABLE) |
	       FIELD_PREP(LSP1_TIMERS_CTRL_MODE, LSP1_TIMERS_CTRL_USER_DEF) |
	       FIELD_PREP(LSP1_TIMERS_CTRL_IRQ_MASK, LSP1_TIMERS_CTRL_IRQ_DISABLE);
	mmio_write_32(LSP1_TIMERS_CTRL(TIMERS_ID), val);

	mmio_write_32(LSP1_TIMERS_LOAD_COUNT(TIMERS_ID), ~0);

	return 0;
}

static int lsp1_timers_init_irq(irq_handler_t handler)
{
	int ret;
	uint32_t val;

	ret = irq_attach_handler(TIMERS_ID + IRQ_ID_TIMERS0_INT, handler);
	if (ret)
		return ret;

	mmio_read_32(LSP1_TIMERS_N_EOI(TIMERS_ID));

	val = mmio_read_32(LSP1_TIMERS_CTRL(TIMERS_ID));
	val &= ~LSP1_TIMERS_CTRL_IRQ_MASK;
	val |= FIELD_PREP(LSP1_TIMERS_CTRL_IRQ_MASK, LSP1_TIMERS_CTRL_IRQ_ENABLE);
	mmio_write_32(LSP1_TIMERS_CTRL(TIMERS_ID), val);

	return 0;
}

static uint32_t lsp1_timers_get_ticks(void)
{
	return ~mmio_read_32(LSP1_TIMERS_CURRENT_VALUE(TIMERS_ID));
}

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

int lsp1_timer_register(bool do_hw_init, uint64_t timebase_us, bool do_irq_init)
{
	timer_ops_t ops;
	timer_hw_t hw;

	// Initialize Timer interface with LSP1 TIMERS ops
	ops.get_clock = lsp1_timers_get_clock;
	ops.get_ticks = lsp1_timers_get_ticks;

	// Initialize LSP1 TIMERS if required
	if (do_hw_init) {
		hw.init = lsp1_timers_init;
		hw.timebase_us = timebase_us;
	} else {
		hw.init = NULL;
		hw.timebase_us = 0;
	}

	// Initialize LSP1 TIMERS IRQ if required
	if (do_irq_init) {
		ops.irq_handler = lsp1_timers_irq_handler;
		hw.init_irq = lsp1_timers_init_irq;
	} else {
		ops.irq_handler = NULL;
		hw.init_irq = NULL;
	}

	hw.ops = &ops;

	return timer_register_hw(&hw);
}
