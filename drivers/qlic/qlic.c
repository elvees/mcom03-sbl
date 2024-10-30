// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <assert.h>
#include <mc.h>
#include <mcom03.h>
#include <mcom03_errors.h>
#include "qlic.h"

/**
 * @brief function returns the base address of the qlic interrupt controller registers
 *
 * @param QlicNum - qlic number
 *
 * @return qlic base address
 */
static qlic_register_t *qlic_get_registers(unsigned int QlicNum)
{
	if (QlicNum > QLIC1)
		return (qlic_register_t *)NULL;
	if (QlicNum == QLIC0) {
		return (qlic_register_t *)BASE_ADDR_SERVICE_QLIC0;
	}
	return (qlic_register_t *)BASE_ADDR_SDR_QLIC;
}

/**
 * @brief function returns the base IP offset
 *
 * @param QlicNum - qlic number
 *
 * @return qlic base IP offset
 */
static unsigned int qlic_get_target_base(unsigned int QlicNum)
{
	assert(QlicNum <= QLIC1);
	return (QlicNum == QLIC0) ? QLIC_TARG_IP2_OFFSET : QLIC_TARG_IP0_OFFSET;
}

/**
 * @brief The function resets all qlic registers
 *
 * @param qlic - pointer to registered qlic context
 *
 * @return errors
 */
static int qlic_reset_registers(qlic_t *qlic)
{
	int i, j;
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	for (i = 0; i < QLIC_IRQ_PRIORITY_COUNT; i++) {
		qlic->regs->PRI[i] = 0;
	}
	for (i = 0; i < QLIC_PEND_REG_COUNT; i++) {
		qlic->regs->PEND[i] = 0;
	}
	for (i = 0; i < QLIC_TARG_MAX; i++) {
		for (j = 0; j < 32; j++) {
			qlic->regs->TARG_ENS[i].Base32[j] = 0;
		}
		qlic->regs->TARG_THD_CC[i].CC = 0;
		qlic->regs->TARG_THD_CC[i].THD = 0;
	}
	return 0;
}

/**
 * @brief The qlic instance initialization
 */
int qlic_init(qlic_t *qlic, unsigned int QlicNum)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	qlic->regs = qlic_get_registers(QlicNum);
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->QlicNum = QlicNum;
	qlic_reset_registers(qlic);
	return 0;
}

/**
 * @brief The function enables a specific external MIPS interrupt
 */
void qlic_enable_target_output(qlic_t *qlic, unsigned int target)
{
	assert(qlic->QlicNum <= QLIC1);
	unsigned int co_status = mips_read_c0_register(C0_STATUS);
	unsigned int offset = qlic_get_target_base(qlic->QlicNum);
	co_status |= (1 << (offset + target));
	mips_write_c0_register(C0_STATUS, co_status);
}

/**
 * @brief Disables a specific external MIPS interrupt.
 */
void qlic_disable_target_output(qlic_t *qlic, unsigned int target)
{
	assert(qlic->QlicNum <= QLIC1);
	unsigned int co_status = mips_read_c0_register(C0_STATUS);
	unsigned int offset = qlic_get_target_base(qlic->QlicNum);
	co_status &= ~(1 << (offset + target));
	mips_write_c0_register(C0_STATUS, co_status);
}

/**
 * @brief get info is specific external MIPS interrupt enabled
 */
int qlic_is_target_output_enabled(qlic_t *qlic, unsigned int target)
{
	assert(qlic != NULL);
	unsigned int offset = qlic_get_target_base(qlic->QlicNum);
	return mips_read_c0_register(C0_STATUS) & (1 << (offset + target));
}

/**
 * @brief enables all MIPS global interrupts
 */
void qlic_global_irq_enable(qlic_t *qlic)
{
	(void)qlic;
	unsigned int co_status = mips_read_c0_register(C0_STATUS);
	co_status |= 1;
	mips_write_c0_register(C0_STATUS, co_status);
}

/**
 * @brief disable all MIPS global interrupts
 */
void qlic_global_irq_disable(qlic_t *qlic)
{
	(void)qlic;
	unsigned int co_status = mips_read_c0_register(C0_STATUS);
	co_status &= ~1;
	mips_write_c0_register(C0_STATUS, co_status);
}

/**
 * @brief get info is MIPS global interrupts enabled or not
 */
int qlic_global_is_irq_enabled(qlic_t *qlic)
{
	(void)qlic;
	return mips_read_c0_register(C0_STATUS) & 0x1;
}

/**
 * @brief Enables the interrupt source number ID
 */
int qlic_enable_IntID(qlic_t *qlic, unsigned int target, unsigned int ID)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->regs->TARG_ENS[target].Base32[ID / 32] |= (1 << (ID % 32));
	return 0;
}

/**
 * @brief Disables the interrupt source number ID
 */
int qlic_disable_IntID(qlic_t *qlic, unsigned int target, unsigned int ID)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->regs->TARG_ENS[target].Base32[ID / 32] &= ~(1 << (ID % 32));
	return 0;
}

/**
 * @brief Returns for which target output the interrupt with ID is allowed
 */
int qlic_is_enable_intID(qlic_t *qlic, unsigned int ID)
{
	int int_enable = 0;
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	for (unsigned int i = 0; i < QLIC_TARG_MAX; i++)
		int_enable |=
			(qlic->regs->TARG_ENS[i].Base32[ID / 32] & (1 << (ID % 32))) ? (1 << i) : 0;
	return int_enable;
}

/**
 * @brief Sets the priority of the specified ID
 */
int qlic_set_int_priority(qlic_t *qlic, unsigned int ID, unsigned int priority)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->regs->PRI[ID] = (priority & QLIC_MAX_IRQ_PRIORITY);
	return 0;
}

// Returns the priority of the specified ID
/**
 * @brief Gets the priority of the specified ID
 */
int qlic_get_int_priority(qlic_t *qlic, unsigned int ID)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	return qlic->regs->PRI[ID] & QLIC_MAX_IRQ_PRIORITY;
}

// Sets the Priority mask register for the core run on
// The reset value masks ALL interrupts!
/**
 * @brief function set priority threshold
 */
int qlic_set_priority_threshold(qlic_t *qlic, unsigned int target, unsigned int priority_threshold)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->regs->TARG_THD_CC[target].THD = (priority_threshold & QLIC_MAX_IRQ_PRIORITY_TRESHOLD);
	return 0;
}

/**
 * @brief function get priority threshold
 */
int qlic_get_priority_threshold(qlic_t *qlic, unsigned int target)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	return (qlic->regs->TARG_THD_CC[target].THD & QLIC_MAX_IRQ_PRIORITY_TRESHOLD);
}

/**
 * @brief FIXME: Add function name
 */
int qlic_get_int_pending(qlic_t *qlic, unsigned int ID)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (ID >= QLIC_INTERRUPTS_COUNT)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	return ((qlic->regs->PEND[ID / 32] & (1 << (ID % 32))) != 0);
}

/**
 * @brief Get interrupt ID.Function is called in ISR
 */
int qlic_get_irq_request(qlic_t *qlic, unsigned int target)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	return qlic->regs->TARG_THD_CC[target].CC;
}

/**
 * @brief Clear interrupt request ID.Function is called in ISR
 */
int qlic_set_irq_terminate(qlic_t *qlic, unsigned int target, unsigned int request)
{
	if (qlic == NULL)
		return MCOM03_ERROR_INVALID_PARAM;
	if (qlic->regs == NULL)
		return MCOM03_ERROR_NULL;
	qlic->regs->TARG_THD_CC[target].CC = request;
	return 0;
}

/**
 * @brief Get current IRQ target
 */
int qlic_get_irq_target(qlic_t *qlic)
{
	unsigned int co_cause = mips_read_c0_register(C0_CAUSE); // get C0 cause
	unsigned int ip_base = qlic_get_target_base(qlic->QlicNum); // get RISC IRQ IP base
	unsigned int pending_irq = (co_cause & 0xFFFF) >> ip_base; // get HW IRQ pending requests
	unsigned int ls_bit =
		((pending_irq) &
		 (-pending_irq)); // select HW IRQ pending requests with highest priority
	assert(ls_bit > 0);

	return __builtin_ctz(ls_bit); // return current target number
}
