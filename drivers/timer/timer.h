// SPDX-License-Identifier: MIT
// Copyright 2024-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <drivers/irq/irq.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#define USEC_IN_SEC  ULL(1000000)
#define MSEC_IN_SEC  ULL(1000)
#define USEC_IN_MSEC ULL(1000)

#define time_after(a, b)  ((long)((b) - (a)) < 0)
#define time_before(a, b) time_after(b, a)

#define read_poll_timeout(op, val, cond, sleep_us, timeout_us, args...)            \
	({                                                                         \
		unsigned long __timeout;                                           \
		if (timeout_us)                                                    \
			__timeout = timer_get_us() + timeout_us;                   \
		for (;;) {                                                         \
			(val) = op(args);                                          \
			if (cond)                                                  \
				break;                                             \
			if (timeout_us && time_after(timer_get_us(), __timeout)) { \
				(val) = op(args);                                  \
				break;                                             \
			}                                                          \
			if (sleep_us)                                              \
				timer_delay_us(sleep_us);                          \
		}                                                                  \
		(cond) ? 0 : -ETIMEOUT;                                            \
	})

typedef struct timer_ops_t {
	int (*get_clock)(uint32_t *);
	uint32_t (*get_ticks)(void);
	irq_handler_t irq_handler;
} timer_ops_t;

typedef struct timer_hw_t {
	uint64_t timebase_us;
	int (*init)(uint64_t);
	int (*init_irq)(irq_handler_t handler);
	timer_ops_t *ops;
} timer_hw_t;

/**
 * @brief Milliseconds to CPU ticks conversion function
 *
 * @param ms   - Milliseconds
 * @param freq - CPU frequency
 *
 * @return Ticks
 */
static inline uint64_t ms_to_tick(uint64_t ms, uint64_t freq)
{
	return ms * freq / MSEC_IN_SEC;
}

/**
 * @brief Microseconds to ticks conversion function
 *
 * @param us   - Microseconds
 * @param freq - Ticks frequency
 *
 * @return Ticks
 */
static inline uint64_t us_to_tick(uint64_t us, uint64_t freq)
{
	return us * freq / USEC_IN_SEC;
}

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
uint64_t timer_get_us(void);

/**
 * @brief Register timer hardware
 *
 * @param hw - Timer hw interface implementation
 *
 */
int timer_register_hw(timer_hw_t *hw);

/**
 * @brief Reset timer value
 *
 */
void timer_reset(void);
