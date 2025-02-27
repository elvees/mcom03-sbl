// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <assert.h>

#include <libs/utils-def.h>

#include "mips-cp0.h"

void mips_global_irq_enable(void)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status |= MIPS_CP0_SR_IE;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

void mips_global_irq_disable(void)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status &= ~MIPS_CP0_SR_IE;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

void mips_enable_irq_target(unsigned int target)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status |= target;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

void mips_disable_irq_target(unsigned int target)
{
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);
	cp0_status &= ~target;
	mips_write_cp0_register(CP0_STATUS, cp0_status);
}

int mips_is_irq_target_enabled(unsigned int target)
{
	return mips_read_cp0_register(CP0_STATUS) & target;
}

int mips_get_irq_target(void)
{
	unsigned int cp0_cause = mips_read_cp0_register(CP0_CAUSE); // get CP0 cause
	// get HW IRQ pending requests
	unsigned int pending_irq = FIELD_GET(MIPS_CP0_CR_QLIC0_TARGET_IP, cp0_cause);
	// select HW IRQ pending requests with highest priority
	unsigned int ls_bit = ((pending_irq) & (-pending_irq));
	assert(ls_bit > 0);

	return __builtin_ctz(ls_bit); // return current target number
}
