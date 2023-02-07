// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <mcom03.h>
#include "wdt.h"

/**
 * @brief the function calculates a coefficient to set the required timeout period
 * @param wdt_dev WDT handler contains desirable user's timeout. After calculation
 * timeout variable sets to real value of WDT timeout in milliseconds
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
		clk_number_to_zero = ((1U << WDT_NUM_TOPS) << i);
		if (clk_number <= clk_number_to_zero) {
			break;
		}
	}
	wdt_dev->timeout = (1U << (WDT_NUM_TOPS + i)) / (wdt_dev->wdt_freq / 1000UL);
	wdt_dev->pretimeout = wdt_dev->timeout;
	return i;
}

/**
 * @brief function get base address wdt registers
 * param wdt_num timer wdt number. possible values 0, 1
 * return returns the base address of the wdt timer
 */
static wdt_regs_t *__wdt_get_registers(uint32_t wdt_num)
{
	if (wdt_num == WDT0)
		return (wdt_regs_t *)BASE_ADDR_SERVICE_WDT0;
	else {
		if (wdt_num == WDT1)
			return (wdt_regs_t *)BASE_ADDR_LS1_WDT1_BASE;
	}
	return NULL;
}

/**
 * @brief Check if wdt enabled or not
 *
 * @param wdt base address of wdt block registers
 * @return 1 - if enabled, 0 - otherwise
 */
int wdt_is_enabled(wdt_dev_t *wdt_dev)
{
	return !!(wdt_dev->regs->WDT_CR & WDT_CR_WDT_EN_MASK);
}

/**
 * @brief Initializing wdt timer before starting
 *
 * @param wdt base address of wdt block registers
 * @return errors
 */
mcom_err_t wdt_init(wdt_dev_t *wdt_dev)
{
	if (wdt_dev == NULL)
		return MCOM03_ERROR_NULL;

	if (wdt_dev->rmod > 1)
		return MCOM03_ERROR_INVALID_PARAM;
	if (wdt_dev->rpl > 7)
		return MCOM03_ERROR_INVALID_PARAM;

	wdt_dev->regs = __wdt_get_registers(wdt_dev->id);
	if (wdt_dev->regs == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t wdt_cr_val = wdt_dev->regs->WDT_CR;
	uint32_t wdt_torr_val = wdt_dev->regs->WDT_TORR;

	/* Set maximum timeout if WDT is already run */
	if (wdt_cr_val & WDT_CR_WDT_EN_MASK) {
		wdt_torr_val &= ~WDT_TORR_TOP_MASK;
		wdt_torr_val |= WDT_MAX_TIMEOUT_COEF;
		wdt_dev->regs->WDT_TORR = wdt_torr_val;

		wdt_reset(wdt_dev);

		wdt_dev->status = WDT_ON;

		return MCOM03_ERROR_ALREADY_INITIALIZED;
	}

	return MCOM03_SUCCESS;
}

/**
 * @brief Launches WDT. After executing this command, the timer cannot be stopped.
 * @param wdt base address of wdt block registers
 * return errors
 */
mcom_err_t wdt_start(wdt_dev_t *wdt_dev)
{
	uint32_t err = MCOM03_SUCCESS;

	if (wdt_dev == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t wdt_torr_val = wdt_dev->regs->WDT_TORR;

	/* Fill the WDT_CR register */
	uint32_t timeout_top, timeout_top_int;

	if (wdt_dev->timeout > WDT_MAX_TIMEOUT)
		wdt_dev->timeout = WDT_MAX_TIMEOUT;

	switch (wdt_dev->timeout) {
	case WDT_MIN_TIMEOUT:
		wdt_dev->timeout = (1U << (WDT_NUM_TOPS)) / (wdt_dev->wdt_freq / 1000UL);
		timeout_top = WDT_MIN_TIMEOUT_COEF;
		break;
	case WDT_MAX_TIMEOUT:
		wdt_dev->timeout = (1U << (WDT_NUM_TOPS + WDT_MAX_TIMEOUT_COEF)) /
				   (wdt_dev->wdt_freq / 1000UL);
		timeout_top = WDT_MAX_TIMEOUT_COEF;
		break;
	default:
		timeout_top = __wdt_get_timeout_coef(wdt_dev);
		break;
	}

	timeout_top_int = timeout_top;

	wdt_torr_val = WDT_TORR_TOP_INT(timeout_top_int) | WDT_TORR_TOP(timeout_top);

	wdt_dev->regs->WDT_TORR = wdt_torr_val;

	/* Update WDT reset timer */
	wdt_reset(wdt_dev);

	/* Fill the WDT_CR register var*/
	uint32_t wdt_cr_val = wdt_dev->regs->WDT_CR;

	if (wdt_dev->rmod == WDT_IRQ_MODE) {
		wdt_cr_val |= WDT_CR_RMOD(WDT_IRQ_MODE);
		wdt_dev->pretimeout = timeout_top;
	} else {
		wdt_cr_val &= ~WDT_CR_RMOD(WDT_IRQ_MODE);
		wdt_dev->pretimeout = 0;
	}

	wdt_cr_val &= ~WDT_CR_RPL_MASK;
	wdt_cr_val |= WDT_CR_RPL(wdt_dev->rpl);

	if (!(wdt_cr_val & WDT_CR_WDT_EN_MASK)) {
		wdt_dev->status = WDT_ON;
		wdt_cr_val |= WDT_CR_WDT_EN(WDT_ON);
	}

	wdt_dev->regs->WDT_CR = wdt_cr_val;

	return err;
}

mcom_err_t wdt_stop(wdt_dev_t *wdt_dev)
{
	(void)wdt_dev;
	return MCOM03_ERROR_FORBIDDEN;
}

mcom_err_t wdt_reset_irq(wdt_dev_t *wdt_dev)
{
	if (wdt_dev == NULL)
		return MCOM03_ERROR_NULL;

	register volatile uint32_t eoi_read = wdt_dev->regs->WDT_EOI;
	(void)eoi_read;

	return MCOM03_SUCCESS;
}

/**
 * @brief reset counter to initial value and clear interrupt
 *
 * @param wdt_dev base address of wdt block registers
 * @return errors
 */
mcom_err_t wdt_reset(wdt_dev_t *wdt_dev)
{
	if (wdt_dev == NULL)
		return MCOM03_ERROR_NULL;

	wdt_dev->regs->WDT_CRR = WDT_CRR_RESET_VALUE;
	wdt_reset_irq(wdt_dev);

	return MCOM03_SUCCESS;
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
 * @param wdt_dev base address of wdt block registers
 * @param timeout new timeout value
 * @return  errors
 */
mcom_err_t wdt_set_timeout(wdt_dev_t *wdt_dev, uint32_t timeout)
{
	wdt_dev->timeout = timeout;
	return wdt_start(wdt_dev);
}

/**
 * @brief Get current timeout value
 *
 * @param wdt_dev base address of wdt block registers
 * @return current timeout value
 */
uint32_t wdt_get_timeout(wdt_dev_t *wdt_dev)
{
	return wdt_dev->timeout;
}

/**
 * @brief Get min timeout value depends on freq
 *
 * @param wdt_dev base address of wdt block registers
 * @return min timeout value
 */
uint32_t wdt_get_min_timeout(wdt_dev_t *wdt_dev)
{
	return (1U << (WDT_NUM_TOPS)) / (wdt_dev->wdt_freq / 1000UL);
}

/**
 * @brief Get max timeout value depends on freq
 *
 * @param wdt_dev base address of wdt block registers
 * @return max timeout value
 */
uint32_t wdt_get_max_timeout(wdt_dev_t *wdt_dev)
{
	return (1U << (WDT_NUM_TOPS + WDT_MAX_TIMEOUT_COEF)) / (wdt_dev->wdt_freq / 1000UL);
}
