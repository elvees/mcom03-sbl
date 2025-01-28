// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#include <stdint.h>

#define time_after(a, b)  ((long)((b) - (a)) < 0)
#define time_before(a, b) time_after(b, a)

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
