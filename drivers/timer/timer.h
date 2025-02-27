// SPDX-License-Identifier: MIT
// Copyright 2024-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/errors.h>
#include <libs/utils-def.h>

#define USEC_IN_SEC  ULL(1000000)
#define MSEC_IN_SEC  ULL(1000)
#define USEC_IN_MSEC ULL(1000)

#define time_after(a, b)  ((long)((b) - (a)) < 0)
#define time_before(a, b) time_after(b, a)

#define read_poll_timeout(op, val, cond, sleep_us, timeout_us, args...)          \
	({                                                                       \
		unsigned long timeout = timer_get_us() + timeout_us;             \
		for (;;) {                                                       \
			(val) = op(args);                                        \
			if (cond)                                                \
				break;                                           \
			if (timeout_us && time_after(timer_get_us(), timeout)) { \
				(val) = op(args);                                \
				break;                                           \
			}                                                        \
			if (sleep_us)                                            \
				timer_delay_us(sleep_us);                        \
		}                                                                \
		(cond) ? 0 : -ETIMEOUT;                                          \
	})

/**
 * @brief Do delay in milliseconds
 *
 * @param num_msec - Delay time in milliseconds
 *
 * This is not an accurate delay, it ensures at least num_msec passed when return
 */
void timer_delay_ms(uint32_t num_msec);

/**
 * @brief Do delay in microseconds
 *
 * @param num_usec - Delay time in microseconds
 *
 * Delay is not exactly accurate
 */
void timer_delay_us(uint32_t num_usec);

/**
 * @brief Get microseconds from boot start
 *
 * Is not exactly accurate
 */
unsigned long timer_get_us(void);
