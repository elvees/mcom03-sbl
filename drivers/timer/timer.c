// SPDX-License-Identifier: MIT
// Copyright 2024-2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mips-cp0/mips-cp0.h>
#include <drivers/service/service.h>

#include "timer.h"

unsigned long timebase_h;
unsigned long timebase_l;

static void __timer_delay_ticks(uint32_t ticks)
{
	if (!ticks)
		return;

	uint32_t start = mips_read_cp0_register(CP0_COUNT);
	while (mips_read_cp0_register(CP0_COUNT) - start < ticks)
		__asm__ volatile("nop");
}

static void __timer_delay_us(uint32_t us)
{
	int ret;
	uint32_t core_frequency;

	ret = service_get_core_clock(&core_frequency);
	if (!ret) {
		uint32_t ticks = (uint64_t)us * core_frequency / USEC_IN_SEC;
		__timer_delay_ticks(ticks);
	}
}

void timer_delay_ms(uint32_t num_msec)
{
	__timer_delay_us(num_msec * USEC_IN_MSEC);
}

void timer_delay_us(uint32_t num_usec)
{
	__timer_delay_us(num_usec);
}

uint64_t timer_get_ticks(void)
{
	unsigned long now = mips_read_cp0_register(CP0_COUNT);

	if (now < timebase_l)
		timebase_h++;

	timebase_l = now;

	return ((uint64_t)timebase_h << 32) | timebase_l;
}

unsigned long timer_get_us(void)
{
	uint64_t tick = timer_get_ticks();
	uint32_t core_frequency;
	int ret;

	ret = service_get_core_clock(&core_frequency);
	if (ret)
		return 0;

	return tick / (core_frequency / USEC_IN_SEC);
}
