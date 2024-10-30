// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <libs/mc.h>

#include "irq.h"
#include "qlic.h"

static irq_handler_t irq_handlers[QLIC_INTERRUPTS_COUNT] = { 0 };

static int32_t irq_reset_handlers(void)
{
	for (int i = 0; i < QLIC_INTERRUPTS_COUNT; ++i)
		irq_handlers[i] = NULL;
	return 0;
}

int32_t irq_init(void)
{
	return qlic_init();
}

int32_t irq_deinit(void)
{
	return irq_reset_handlers();
}

int32_t irq_attach_handler(unsigned int id, irq_handler_t handler)
{
	int32_t ret;

	ret = qlic_set_int_priority(id, QLIC_MAX_IRQ_PRIORITY);
	if (ret)
		return ret;

	ret = qlic_set_priority_threshold(QLIC0_TARG_SERVICE_RISC0_IP2, 0);
	if (ret)
		return ret;

	ret = qlic_enable_int_id(QLIC0_TARG_SERVICE_RISC0_IP2, id);
	if (ret)
		return ret;

	irq_handlers[id] = handler;
	return 0;
}

int32_t irq_detach_handler(unsigned int id)
{
	int32_t ret;

	ret = qlic_disable_int_id(QLIC0_TARG_SERVICE_RISC0_IP2, id);
	if (ret)
		return ret;

	irq_handlers[id] = NULL;
	return 0;
}

void irq_handler(void)
{
	unsigned int target = mips_get_irq_target();

	// get IRQ request ID for this target (related to IP base)
	unsigned int id = qlic_get_irq_request(target);
	assert(id < QLIC_INTERRUPTS_COUNT);

	irq_handler_t handler = irq_handlers[id];
	if (handler)
		handler(id);

	qlic_clear_irq_request(target, id);
}
