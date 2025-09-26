// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/mips-cp0/mips-cp0.h>
#include <libs/errors.h>

#include "irq.h"
#include "qlic.h"

#define IRQ_COUNT QLIC_INTERRUPTS_COUNT

static irq_handler_t irq_handlers[IRQ_COUNT] = { 0 };

static int32_t irq_qlic_handlers_is_empty(void)
{
	for (int id = 0; id < IRQ_COUNT; ++id) {
		if (id == IRQ_ID_CP0_CNT_CMP)
			continue;
		if (irq_handlers[id])
			return 0;
	}

	return 1;
}

int32_t irq_init(void)
{
	int32_t ret;

	ret = qlic_init();
	if (ret)
		return ret;

	mips_global_irq_enable();

	return 0;
}

int32_t irq_attach_handler(unsigned int id, irq_handler_t handler)
{
	int32_t ret;

	if (id >= IRQ_COUNT)
		return -EINVALIDPARAM;

	if (irq_handlers[id])
		return -EALREADYINITIALIZED;

	if (id == IRQ_ID_CP0_CNT_CMP) {
		mips_enable_irq_target(MIPS_CP0_SR_IM6_CNT_CMP);
	} else {
		if (irq_qlic_handlers_is_empty())
			mips_enable_irq_target(MIPS_CP0_SR_IM2_QLIC0_TARG0);

		ret = qlic_set_int_priority(id, QLIC_MAX_IRQ_PRIORITY);
		if (ret)
			return ret;

		ret = qlic_set_priority_threshold(QLIC0_TARG_SERVICE_RISC0_IP2, 0);
		if (ret)
			return ret;

		ret = qlic_enable_int_id(QLIC0_TARG_SERVICE_RISC0_IP2, id);
		if (ret)
			return ret;
	}

	irq_handlers[id] = handler;

	return 0;
}

int32_t irq_detach_handler(unsigned int id)
{
	int32_t ret;

	if (id >= IRQ_COUNT)
		return -EINVALIDPARAM;

	if (!irq_handlers[id])
		return -ENULL;

	irq_handlers[id] = NULL;

	if (id == IRQ_ID_CP0_CNT_CMP) {
		mips_disable_irq_target(MIPS_CP0_SR_IM6_CNT_CMP);
	} else {
		ret = qlic_disable_int_id(QLIC0_TARG_SERVICE_RISC0_IP2, id);
		if (ret)
			return ret;

		if (irq_qlic_handlers_is_empty())
			mips_disable_irq_target(MIPS_CP0_SR_IM2_QLIC0_TARG0);
	}

	return 0;
}

void irq_handler(void)
{
	irq_handler_t handler;

	unsigned int cp0_cause = mips_read_cp0_register(CP0_CAUSE);
	unsigned int cp0_status = mips_read_cp0_register(CP0_STATUS);

	if (cp0_cause & cp0_status & MIPS_CP0_CR_CNT_CMP_TARG_IP) {
		// CP0_COMPARE has to be set by IRQ handler to clear IRQ request
		handler = irq_handlers[IRQ_ID_CP0_CNT_CMP];
		if (handler)
			handler(IRQ_ID_CP0_CNT_CMP);
	}

	if (cp0_cause & cp0_status & MIPS_CP0_CR_QLIC0_TARGS) {
		// Get HW IRQ pending requests
		unsigned int pending_irq = FIELD_GET(MIPS_CP0_CR_QLIC0_TARGS, cp0_cause);

		// Select HW IRQ pending request with highest priority
		unsigned int lsb = (pending_irq) & (-pending_irq);
		assert(lsb != 0);

		// Get current QLIC0 target number
		unsigned int target = __builtin_ctz(lsb);

		// Get IRQ request ID for current QLIC0 target
		unsigned int id = qlic_get_irq_request(target);
		assert(id != QLIC_ID0_INVALID);

		handler = irq_handlers[id];
		if (handler)
			handler(id);

		qlic_clear_irq_request(target, id);
	}
}
