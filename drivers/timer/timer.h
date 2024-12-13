// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#include <stdint.h>

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
