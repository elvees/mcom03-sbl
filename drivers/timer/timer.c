// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/service/service.h>
#include <libs/mc.h>
#include <libs/utils-def.h>

static void __timer_delay_ticks(uint32_t ticks)
{
	if (!ticks)
		return;

	uint32_t start = mips_read_c0_register(C0_COUNT);
	while (mips_read_c0_register(C0_COUNT) - start < ticks)
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
