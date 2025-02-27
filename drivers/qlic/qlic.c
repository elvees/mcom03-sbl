// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stddef.h>

#include <drivers/service/service.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "qlic.h"

/**
 * @brief The function returns the base address of the qlic interrupt controller register
 *
 * @return qlic base address
 */
static qlic_regs_t *qlic_get_registers(void)
{
	return (qlic_regs_t *)BASE_ADDR_SERVICE_QLIC0;
}

int qlic_init(void)
{
	int i, j;
	qlic_regs_t *qlic = qlic_get_registers();

	for (i = 0; i < QLIC_IRQ_PRIORITY_COUNT; i++)
		qlic->pri[i] = 0;
	for (i = 0; i < QLIC_PEND_REG_COUNT; i++)
		qlic->pend[i] = 0;
	for (i = 0; i < QLIC_TARG_MAX; i++) {
		for (j = 0; j < 32; j++)
			qlic->targ_ens[i].base32[j] = 0;
		qlic->targ_thd_cc[i].cc = 0;
		qlic->targ_thd_cc[i].thd = 0;
	}
	return 0;
}

int qlic_enable_int_id(unsigned int target, unsigned int id)
{
	qlic_regs_t *qlic = qlic_get_registers();

	if (id >= QLIC_INTERRUPTS_COUNT)
		return -EINVALIDPARAM;

	qlic->targ_ens[target].base32[id / 32] |= BIT(id % 32);

	return 0;
}

int qlic_disable_int_id(unsigned int target, unsigned int id)
{
	qlic_regs_t *qlic = qlic_get_registers();

	if (id >= QLIC_INTERRUPTS_COUNT)
		return -EINVALIDPARAM;

	qlic->targ_ens[target].base32[id / 32] &= ~BIT(id % 32);

	return 0;
}

int qlic_set_int_priority(unsigned int id, unsigned int priority)
{
	qlic_regs_t *qlic = qlic_get_registers();

	if (id >= QLIC_INTERRUPTS_COUNT)
		return -EINVALIDPARAM;

	qlic->pri[id] = (priority & QLIC_MAX_IRQ_PRIORITY);

	return 0;
}

int qlic_get_int_priority(unsigned int id)
{
	qlic_regs_t *qlic = qlic_get_registers();

	if (id >= QLIC_INTERRUPTS_COUNT)
		return -EINVALIDPARAM;

	return qlic->pri[id] & QLIC_MAX_IRQ_PRIORITY;
}

int qlic_set_priority_threshold(unsigned int target, unsigned int priority_threshold)
{
	qlic_regs_t *qlic = qlic_get_registers();

	qlic->targ_thd_cc[target].thd = (priority_threshold & QLIC_MAX_IRQ_PRIORITY_TRESHOLD);

	return 0;
}

int qlic_get_priority_threshold(unsigned int target)
{
	qlic_regs_t *qlic = qlic_get_registers();

	return (qlic->targ_thd_cc[target].thd & QLIC_MAX_IRQ_PRIORITY_TRESHOLD);
}

int qlic_get_irq_request(unsigned int target)
{
	qlic_regs_t *qlic = qlic_get_registers();

	return qlic->targ_thd_cc[target].cc;
}

int qlic_clear_irq_request(unsigned int target, unsigned int request)
{
	qlic_regs_t *qlic = qlic_get_registers();

	qlic->targ_thd_cc[target].cc = request;

	return 0;
}
