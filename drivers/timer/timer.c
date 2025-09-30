// SPDX-License-Identifier: MIT
// Copyright 2024-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "timer.h"

typedef struct timer_ticks_t {
	volatile uint32_t h;
	volatile uint32_t l;
} timer_ticks_t;

typedef struct timer_t {
	bool irq_on;
	bool registered;
	timer_ops_t ops;
} timer_t;

static timer_ticks_t timer_ticks __attribute__((__section__(".timer")));
static timer_t timer = { 0 };

static void irq_timer_handler(unsigned int id)
{
	if (!timer.registered)
		return;

	timer_ticks.h++;

	if (timer.ops.irq_handler)
		timer.ops.irq_handler(id);
}

static uint64_t __timer_get_ticks(void)
{
	if (!timer.registered)
		return 0;

	uint32_t now = timer.ops.get_ticks();

	if (!timer.irq_on && now < timer_ticks.l)
		timer_ticks.h++;

	timer_ticks.l = now;

	return ((uint64_t)timer_ticks.h) << 32 | (uint64_t)timer_ticks.l;
}

static void __timer_delay_us(uint64_t us)
{
	int ret;
	uint32_t freq;
	uint64_t ticks, start;

	if (!us || !timer.registered)
		return;

	ret = timer.ops.get_clock(&freq);
	if (!ret) {
		ticks = us_to_tick(us, freq);
		start = __timer_get_ticks();

		while (__timer_get_ticks() - start < ticks)
			__asm__ volatile("nop");
	}
}

int timer_register_hw(timer_hw_t *hw)
{
	int ret;
	uint32_t freq;
	uint64_t ticks;
	bool irq_on = false;

	if (!hw || !hw->ops || !hw->ops->get_ticks || !hw->ops->get_clock)
		return -ENULL;

	if (hw->init_irq) {
		ret = hw->init_irq(irq_timer_handler);
		if (ret)
			return ret;

		irq_on = true;
	}

	if (hw->init) {
		ret = hw->init(hw->timebase_us);
		if (ret)
			return ret;

		ret = hw->ops->get_clock(&freq);
		if (ret)
			return ret;

		ticks = us_to_tick(hw->timebase_us, freq);

		timer_ticks.h = (uint32_t)(ticks >> 32);
		timer_ticks.l = (uint32_t)ticks;
	}

	timer.ops = *hw->ops;
	timer.irq_on = irq_on;
	timer.registered = true;

	return 0;
}

void timer_reset(void)
{
	timer_ticks.h = 0;
	timer_ticks.l = 0;
}

void timer_delay_ms(uint32_t num_msec)
{
	__timer_delay_us(num_msec * USEC_IN_MSEC);
}

void timer_delay_us(uint32_t num_usec)
{
	__timer_delay_us(num_usec);
}

uint64_t timer_get_us(void)
{
	uint32_t freq;
	int ret;

	if (!timer.registered)
		return 0;

	ret = timer.ops.get_clock(&freq);
	if (ret)
		return 0;

	return __timer_get_ticks() / (freq / USEC_IN_SEC);
}
