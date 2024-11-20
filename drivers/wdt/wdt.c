// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>

#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "wdt.h"

static wdt_dev_t wdt_dev;

/**
 * @brief The function gets pointer of the wdt device structure instance
 *
 * @return pointer of the wdt device structure instance
 */
wdt_dev_t *wdt_get_instance(void)
{
	return &wdt_dev;
}

/**
 * @brief The function configures wdt
 *
 * @param wdt_dev pointer to wdt device structure
 * @param timeout timeout value in milliseconds
 *
 * @return  0     - Success,
 *         -ENULL - wdt_dev is not provided (NULL pointer)
 */
int wdt_set_config(wdt_dev_t *wdt_dev, uint32_t timeout)
{
	int ret;
	uint32_t apb_freq;

	if (!wdt_dev)
		return -ENULL;

	ret = service_get_apb_clock(&apb_freq);
	if (ret)
		return ret;

	wdt_dev->rmod = WDT_RST_MODE;
	wdt_dev->rpl = WDT_RST_PULSE_LEN_2;
	wdt_dev->timeout = timeout;
	wdt_dev->wdt_freq = apb_freq;

	return 0;
}

/**
 * @brief The function calculates a coefficient to set the required timeout period
 *
 * @param wdt_dev WDT handler contains desirable user's timeout. After calculation
 *                timeout variable sets to real value of WDT timeout in milliseconds
 *
 * @return coefficient that must be written to the TOP and TOP_INT fields of the WDT_TOPR register
 */
static uint32_t __wdt_get_timeout_coef(wdt_dev_t *wdt_dev)
{
	int i = 0;
	uint32_t clk_number_to_zero; // number of pulses signal clock before counter zeroing
	uint32_t clk_number =
		wdt_dev->timeout *
		((wdt_dev->wdt_freq) / 1000UL); // number of claps to obtain the required delay
	for (i = WDT_MIN_TIMEOUT_COEF; i < WDT_MAX_TIMEOUT_COEF; i++) {
		clk_number_to_zero = BIT(WDT_NUM_TOPS + i);
		if (clk_number <= clk_number_to_zero)
			break;
	}
	wdt_dev->timeout = BIT(WDT_NUM_TOPS + i) / (wdt_dev->wdt_freq / 1000UL);
	wdt_dev->pretimeout = wdt_dev->timeout;
	return i;
}

/**
 * @brief Check if wdt is enabled or not
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return 1 - if enabled, 0 - otherwise
 */
int wdt_is_enabled(wdt_dev_t *wdt_dev)
{
	return !!(wdt_dev->regs->WDT_CR & WDT_CR_WDT_EN_MASK);
}

/**
 * @brief Initializing wdt timer before starting
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return  0                   - Success,
 *         -ENULL               - wdt_dev is not provided (NULL pointer)
 *         -EINVALIDPARAM       - rmod field in wdt_dev is bigger than 1 or
 *                                rpl field in wdt_dev is bigger than 7
 *         -EALREADYINITIALIZED - WDT is already enabled
 */
int wdt_init(wdt_dev_t *wdt_dev)
{
	if (!wdt_dev)
		return -ENULL;

	if (wdt_dev->rmod > 1)
		return -EINVALIDPARAM;
	if (wdt_dev->rpl > 7)
		return -EINVALIDPARAM;

	wdt_dev->regs = (wdt_regs_t *)BASE_ADDR_SERVICE_WDT0;

	/* Set maximum timeout if WDT is already run */
	if (wdt_is_enabled(wdt_dev)) {
		uint32_t wdt_torr_val = wdt_dev->regs->WDT_TORR;
		wdt_torr_val &= ~WDT_TORR_TOP_MASK;
		wdt_torr_val |= WDT_MAX_TIMEOUT_COEF;
		wdt_dev->regs->WDT_TORR = wdt_torr_val;

		wdt_reset(wdt_dev);

		wdt_dev->status = WDT_ON;

		return -EALREADYINITIALIZED;
	}

	return 0;
}

/**
 * @brief Launches WDT. After executing this command, the timer cannot be stopped.
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return  0     - Success,
 *         -ENULL - wdt_dev is not provided (NULL pointer)
 */
int wdt_start(wdt_dev_t *wdt_dev)
{
	int err = 0;

	if (!wdt_dev)
		return -ENULL;

	uint32_t wdt_torr_val = wdt_dev->regs->WDT_TORR;

	/* Fill the WDT_CR register */
	uint32_t timeout_top, timeout_top_int;

	if (wdt_dev->timeout > WDT_MAX_TIMEOUT)
		wdt_dev->timeout = WDT_MAX_TIMEOUT;

	switch (wdt_dev->timeout) {
	case WDT_MIN_TIMEOUT:
		wdt_dev->timeout = BIT(WDT_NUM_TOPS) / (wdt_dev->wdt_freq / 1000UL);
		timeout_top = WDT_MIN_TIMEOUT_COEF;
		break;
	case WDT_MAX_TIMEOUT:
		wdt_dev->timeout =
			BIT(WDT_NUM_TOPS + WDT_MAX_TIMEOUT_COEF) / (wdt_dev->wdt_freq / 1000UL);
		timeout_top = WDT_MAX_TIMEOUT_COEF;
		break;
	default:
		timeout_top = __wdt_get_timeout_coef(wdt_dev);
		break;
	}

	timeout_top_int = timeout_top;

	wdt_torr_val = FIELD_PREP(WDT_TORR_TOP_INT_MASK, timeout_top_int) |
	               FIELD_PREP(WDT_TORR_TOP_MASK, timeout_top);

	wdt_dev->regs->WDT_TORR = wdt_torr_val;

	/* Update WDT reset timer */
	wdt_reset(wdt_dev);

	/* Fill the WDT_CR register var*/
	uint32_t wdt_cr_val = wdt_dev->regs->WDT_CR;

	if (wdt_dev->rmod == WDT_IRQ_MODE) {
		wdt_cr_val |= FIELD_PREP(WDT_CR_RMOD_MASK, WDT_IRQ_MODE);
		wdt_dev->pretimeout = timeout_top;
	} else {
		wdt_cr_val &= ~FIELD_PREP(WDT_CR_RMOD_MASK, WDT_IRQ_MODE);
		wdt_dev->pretimeout = 0;
	}

	wdt_cr_val &= ~WDT_CR_RPL_MASK;
	wdt_cr_val |= FIELD_PREP(WDT_CR_RPL_MASK, wdt_dev->rpl);

	if (!(wdt_cr_val & WDT_CR_WDT_EN_MASK)) {
		wdt_dev->status = WDT_ON;
		wdt_cr_val |= FIELD_PREP(WDT_CR_WDT_EN_MASK, WDT_ON);
	}

	wdt_dev->regs->WDT_CR = wdt_cr_val;

	return err;
}

int wdt_stop(wdt_dev_t *wdt_dev)
{
	(void)wdt_dev;
	return -EFORBIDDEN;
}

int wdt_reset_irq(wdt_dev_t *wdt_dev)
{
	if (!wdt_dev)
		return -ENULL;

	register volatile uint32_t eoi_read = wdt_dev->regs->WDT_EOI;
	(void)eoi_read;

	return 0;
}

/**
 * @brief Reset counter to initial value and clear interrupt
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return  0     - Success,
 *         -ENULL - wdt_dev is not provided (NULL pointer)
 */
int wdt_reset(wdt_dev_t *wdt_dev)
{
	if (!wdt_dev)
		return -ENULL;

	wdt_dev->regs->WDT_CRR = WDT_CRR_RESET_VALUE;
	wdt_reset_irq(wdt_dev);

	return 0;
}

uint32_t wdt_get_timeleft(wdt_dev_t *wdt_dev)
{
	uint32_t millis;
	uint32_t val;

	val = wdt_dev->regs->WDT_CCVR;
	millis = val / ((wdt_dev->wdt_freq) / 1000UL);

	if (wdt_dev->rmod == WDT_IRQ_MODE) {
		val = wdt_dev->regs->WDT_STAT;
		if (!val)
			millis += wdt_dev->pretimeout;
	}

	return millis;
}

/**
 * @brief Set current timeout value
 *
 * @param wdt_dev pointer to wdt device structure
 * @param timeout new timeout value in milliseconds
 *
 * @return  0     - Success,
 *         -ENULL - wdt_dev is not provided (NULL pointer)
 */
int wdt_set_timeout(wdt_dev_t *wdt_dev, uint32_t timeout)
{
	if (!wdt_dev)
		return -ENULL;

	wdt_dev->timeout = timeout;
	return wdt_start(wdt_dev);
}

/**
 * @brief Get current timeout value
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return current timeout value in milliseconds
 */
uint32_t wdt_get_timeout(wdt_dev_t *wdt_dev)
{
	return wdt_dev->timeout;
}

/**
 * @brief Get min timeout value depends on freq
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return min timeout value in milliseconds
 */
uint32_t wdt_get_min_timeout(wdt_dev_t *wdt_dev)
{
	return BIT(WDT_NUM_TOPS) / (wdt_dev->wdt_freq / 1000UL);
}

/**
 * @brief Get max timeout value depends on freq
 *
 * @param wdt_dev pointer to wdt device structure
 *
 * @return max timeout value in milliseconds
 */
uint32_t wdt_get_max_timeout(wdt_dev_t *wdt_dev)
{
	return BIT(WDT_NUM_TOPS + WDT_MAX_TIMEOUT_COEF) / (wdt_dev->wdt_freq / 1000UL);
}
