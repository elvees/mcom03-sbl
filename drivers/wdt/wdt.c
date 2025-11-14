// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>

#include <drivers/service/service.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "wdt.h"

#define __wdt_get_timeout_ms(freq, coef) (BIT(WDT_TORR_NUM_TOPS + (coef)) / ((freq) / MSEC_IN_SEC))

static wdt_dev_t wdt;

/**
 * @brief The function calculates a coefficient to set the required timeout period
 *
 * @param wdt - Pointer to WDT instance structure
 * @param timeout_ms - Desired timeout in ms.
 *
 * @return Coefficient that must be written to the WDT_TORR::{TOP,TOP_INT} register
 */
static uint32_t __wdt_get_timeout_coef(wdt_dev_t *wdt, uint32_t timeout_ms)
{
	int i = 0;

	// number of ticks before counter zeroing
	uint32_t clk_number_to_zero;
	// number of ticks to obtain the required delay
	uint32_t clk_number = timeout_ms * (wdt->freq / MSEC_IN_SEC);
	for (i = WDT_TORR_TOP_MIN_TIMEOUT_COEF; i < WDT_TORR_TOP_MAX_TIMEOUT_COEF; i++) {
		clk_number_to_zero = BIT(WDT_TORR_NUM_TOPS + i);
		if (clk_number <= clk_number_to_zero)
			break;
	}

	return i;
}

wdt_dev_t *wdt_get_instance(void)
{
	return &wdt;
}

int wdt_config_default(wdt_dev_t *wdt)
{
	if (!wdt)
		return -ENULL;

	wdt->rmod = WDT_CR_RMOD_RST_MODE;
	wdt->rpl = WDT_CR_RPL_RST_PULSE_LEN_2;

	return 0;
}

int wdt_set_timeout_ms(wdt_dev_t *wdt, uint32_t timeout_ms)
{
	uint32_t torr_val;

	if (!wdt || !wdt->regs)
		return -ENULL;

	// Fill the WDT_TORR register
	torr_val = wdt->regs->torr;
	torr_val &= ~WDT_TORR_TOP_MASK;
	torr_val |= FIELD_PREP(WDT_TORR_TOP_MASK, __wdt_get_timeout_coef(wdt, timeout_ms));
	wdt->regs->torr = torr_val;

	// Reset WDT counter
	return wdt_reset(wdt);
}

uint32_t wdt_get_timeout_ms(wdt_dev_t *wdt)
{
	if (!wdt || !wdt->regs)
		return 0;

	return __wdt_get_timeout_ms(wdt->freq, FIELD_GET(WDT_TORR_TOP_MASK, wdt->regs->torr));
}

uint32_t wdt_get_min_timeout_ms(wdt_dev_t *wdt)
{
	if (!wdt)
		return 0;

	return __wdt_get_timeout_ms(wdt->freq, WDT_TORR_TOP_MIN_TIMEOUT_COEF);
}

uint32_t wdt_get_max_timeout_ms(wdt_dev_t *wdt)
{
	if (!wdt)
		return 0;

	return __wdt_get_timeout_ms(wdt->freq, WDT_TORR_TOP_MAX_TIMEOUT_COEF);
}

int wdt_is_enabled(wdt_dev_t *wdt)
{
	if (!wdt || !wdt->regs)
		return 0;

	return !!(wdt->regs->cr & WDT_CR_WDT_EN_MASK);
}

int wdt_start(wdt_dev_t *wdt)
{
	uint32_t cr_val, torr_val;

	if (!wdt || !wdt->regs)
		return -ENULL;

	cr_val = wdt->regs->cr;
	cr_val &= ~(WDT_CR_RMOD_MASK | WDT_CR_RPL_MASK);
	cr_val |= FIELD_PREP(WDT_CR_RMOD_MASK, wdt->rmod) | FIELD_PREP(WDT_CR_RPL_MASK, wdt->rpl);
	wdt->regs->cr = cr_val;

	if (wdt_is_enabled(wdt)) {
		// Set max timeout
		torr_val = wdt->regs->torr;
		torr_val &= ~WDT_TORR_TOP_MASK;
		torr_val |= FIELD_PREP(WDT_TORR_TOP_MASK, WDT_TORR_TOP_MAX_TIMEOUT_COEF);
		wdt->regs->torr = torr_val;

		// Reset WDT counter
		wdt_reset(wdt);

		return -EALREADYINITIALIZED;
	}

	// Fill the WDT_TORR register
	torr_val = wdt->regs->torr;
	torr_val &= ~WDT_TORR_TOP_INT_MASK;
	torr_val |= FIELD_PREP(WDT_TORR_TOP_INT_MASK, WDT_TORR_TOP_MAX_TIMEOUT_COEF);
	wdt->regs->torr = torr_val;

	// Enable WDT
	cr_val = wdt->regs->cr;
	cr_val |= FIELD_PREP(WDT_CR_WDT_EN_MASK, WDT_CR_WDT_EN_ON);
	wdt->regs->cr = cr_val;

	return 0;
}

int wdt_reset_irq(wdt_dev_t *wdt)
{
	if (!wdt || !wdt->regs)
		return -ENULL;

	register volatile uint32_t eoi_read = wdt->regs->eoi;
	(void)eoi_read;

	return 0;
}

int wdt_reset(wdt_dev_t *wdt)
{
	if (!wdt || !wdt->regs)
		return -ENULL;

	wdt->regs->crr = WDT_CRR_RESET_VALUE;
	wdt_reset_irq(wdt);

	return 0;
}

int wdt0_hw_enable(wdt_dev_t *wdt)
{
	int ret;

	if (!wdt)
		return -ENULL;

	ret = service_get_apb_clock(&wdt->freq);
	if (ret)
		return ret;

	wdt->regs = (wdt_regs_t *)BASE_ADDR_SERVICE_WDT0;

	return 0;
}
