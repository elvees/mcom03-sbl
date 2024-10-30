// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <stddef.h>
#include <mcom03.h>
#include <mcom03_errors.h>
#include <platform_helpers.h>
#include <drivers/intmgr/intmgr.h>
#include <lib/llist/llist.h>
#include "mailbox.h"

//TODO block access to mailbox if it is already used by someone
// TODO spinlock when capturing mailbox

typedef struct {
	struct llist *nodes;
} mbox_irq_t;

static mbox_irq_t mbox_irq = { 0 };

static struct llist *mbox_irq_by_idx(uint32_t idx)
{
	struct llist *irq_lut_head = mbox_irq.nodes;
	uint32_t _idx = 0;
	while (irq_lut_head != NULL) {
		if (_idx == idx) {
			return irq_lut_head;
		}
		irq_lut_head = irq_lut_head->next;
		_idx++;
	}
	return NULL;
}

static struct llist *mbox_irq_by_handle(mbox_irq_handler_t mbox_irq_handler)
{
	struct llist *irq_lut_head = mbox_irq.nodes;
	while (irq_lut_head != NULL) {
		mbox_irq_handler_t handle = (mbox_irq_handler_t)irq_lut_head->data;
		if (handle == mbox_irq_handler) {
			return irq_lut_head;
		}
		irq_lut_head = irq_lut_head->next;
	}

	return NULL;
}

static void irq_mailbox0_handler(unsigned int id)
{
	//FIXME: this only works with MB0
	unsigned int fifoNum = id - QLIC0_ID_MB0_RRQ_RISC0;

	assert(fifoNum < MAILBOX_FIFO_COUNT);

	mailbox_regs_t *mb0 = (mailbox_regs_t *)BASE_ADDR_SERVICE_MAILBOX0;
	unsigned int irq_read = mb0->mailbox[fifoNum].SET_IRQ_READ;
	/* find lowest free address */
	struct llist *irq_lut_head = mbox_irq.nodes;
	while (irq_lut_head != NULL) {
		mbox_irq_handler_t handle = (mbox_irq_handler_t)irq_lut_head->data;
		if (handle)
			handle(fifoNum);
		irq_lut_head = irq_lut_head->next;
	}
	mb0->mailbox[fifoNum].CLR_IRQ_READ = irq_read;
}

/**
 * @brief software reset mailbox device
 *
 * @param mbox fifo registers pointerp_mbox:...
 */
static int mbox_reset(mailbox_regs_t *mbox)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	mbox->SOFT_RESET = 1;
	return 0;
}

/**
 * @brief function retruns pointer to mailbox registers...
 *
 * @param mboxNum number mailbox
 * @return mailbox_regs_t* pointer to mailbox registers
 */
mailbox_regs_t *mbox_get_regs(unsigned int mboxNum)
{
	if (mboxNum == 0)
		return (mailbox_regs_t *)BASE_ADDR_SERVICE_MAILBOX0;
	if (mboxNum == 1)
		return (mailbox_regs_t *)BASE_ADDR_SDR_MAILBOX1;
	return (mailbox_regs_t *)NULL;
}

/**
 * @brief init mailbox device
 * @param mbox pointer to registers fifo
 */
int mbox_init(mailbox_regs_t *regs)
{
	int ret;

	ret = mbox_reset(regs);
	if (ret != 0) {
		return ret;
	}

	for (uint8_t fifoNum = 0; fifoNum < MAILBOX_FIFO_COUNT; ++fifoNum) {
		//FIXME: this only works with MB0
		ret = intmgr_attach_irq_handler(fifoNum + QLIC0_ID_MB0_RRQ_RISC0,
						QLIC0_TARG_SERVICE_RISC0_IP2, irq_mailbox0_handler);
		if (ret != 0) {
			return ret;
		}
	}

	return mbox_enable_irq(regs);
}

/**
 * @brief deinit mailbox device
 * @param mbox pointer to registers fifo
 *
 */
int mbox_deinit(mailbox_regs_t *regs)
{
	int ret;

	ret = mbox_disable_irq(regs);
	if (ret != 0) {
		return ret;
	}

	for (uint8_t fifoNum = 0; fifoNum < MAILBOX_FIFO_COUNT; ++fifoNum) {
		//FIXME: this only works with MB0
		ret = intmgr_detach_irq_handler(fifoNum + QLIC0_ID_MB0_RRQ_RISC0,
						QLIC0_TARG_SERVICE_RISC0_IP2);
		if (ret != 0) {
			return ret;
		}
	}

	return mbox_reset(regs);
}

/**
 * @brief the function returns status full fifo
 *
 * @param mbox pointer to registers fifo
 * @return return status fifo mailbox? if full return 1, not full return 0
 */
int mbox_is_full(mbox_fifo_regs_t *mbox)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	return mbox->FIFO_STATUS.FULL;
}

/**
 * @brief the function returns status full fifo
 *
 * @param mbox pointer to registers fifo
 * @return return status fifo mailbox? if empty return 1, not empty return 0
 */
int mbox_is_empty(mbox_fifo_regs_t *mbox)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	return mbox->FIFO_STATUS.EMPTY;
}

/**
 * @brief the function returns the number of  word in the fifo
 *
 * @param mbox pointer to registers fifo
 * @return the function returns the number of  word in the fifo
 */
int mbox_status(mbox_fifo_regs_t *mbox)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	return mbox->FIFO_STATUS.STATUS;
}

/**
 * @brief functions set irq read
 * The function sets an interrupt for external users when data for reception is prepared.
 * @param mbox pointer to registers fifo
 * @param irq interrupt bits for sets
 * @return int
 */
int mbox_set_IRQread(mbox_fifo_regs_t *mbox, int irq)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	mbox->SET_IRQ_READ = irq;
	return 0;
}

/**
 * @brief functions clears irq read
 * The function sets an interrupt.
 * @param mbox pointer to registers fifo
 * @param irq interrupt bits for clears
 * @return error
 */
int mbox_clr_IRQread(mbox_fifo_regs_t *mbox, int irq)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	mbox->CLR_IRQ_READ = irq;
	return 0;
}

/**
 * @brief functions get irq read
 * The function received mailbox irq flags.
 * @param mbox pointer to registers fifo
 * @return flag
 */
unsigned int mbox_get_IRQread(mbox_fifo_regs_t *mbox)
{
	if (mbox == NULL)
		return MCOM03_ERROR_NULL;
	return mbox->SET_IRQ_READ;
}

/**
 * @brief functions to attach irq handler
 * The function registers handler to mailbox event
 * @param mbox pointer to registers fifo
 * @param mbox_irq_handler - handler
 * @return error
 */
int mbox_attach_irq_handler(mailbox_regs_t *mbox, mbox_irq_handler_t mbox_irq_handler)
{
	(void)mbox;
	struct llist *node;
	uint32_t i;

	/* find lowest free address */
	for (i = 0; i < UINT32_MAX; ++i) {
		if (mbox_irq_by_idx(i) == NULL) {
			break;
		}
	}

	if (i == UINT32_MAX) {
		return -1;
	}

	node = malloc(sizeof(struct llist));
	if (node == NULL) {
		return -2;
	}

	node->data = (void *)mbox_irq_handler;

	add_to_list((struct llist **)&mbox_irq.nodes, node);

	return 0;
}

/**
 * @brief functions to deattach irq handler
 * @param mbox pointer to registers fifo
 * The function unregisters handler to mailbox event
 * @param mbox_irq_handler - handler
 * @return error
 */
int mbox_detach_irq_handler(mailbox_regs_t *mbox, mbox_irq_handler_t mbox_irq_handler)
{
	(void)mbox;
	struct llist *node;

	node = mbox_irq_by_handle(mbox_irq_handler);
	if (node != NULL) {
		remove_from_list((struct llist **)&mbox_irq.nodes, node);
		free(node);
	}

	return 0;
}

/**
 * @brief The function enable mailbox irq (used with mbox_disable_irq)
 * @param mbox pointer to registers fifo
 * @return error
 */
int mbox_enable_irq(mailbox_regs_t *mbox)
{
	(void)mbox;
	return intmgr_enable_irq(QLIC0_TARG_SERVICE_RISC0_IP2);
}

/**
 * @brief The function trmporary disable mailbox irq (used with mbox_enable_irq)
 * @param mbox pointer to registers fifo
 * @return error
 */
int mbox_disable_irq(mailbox_regs_t *mbox)
{
	(void)mbox;
	return intmgr_disable_irq(QLIC0_TARG_SERVICE_RISC0_IP2);
}

/**
 * @brief function to write data to mailbox
 *
 * @param mbox    - pointer to mbox fifo
 * @param message - pointer to message to send to mbox
 * @param size    - actual size of message in bytes
 * @param timeout - timeout
 * @return returns the number of successfully written bytes
 */
unsigned int mbox_write(mbox_fifo_regs_t *mbox, const char *message, unsigned int size,
			unsigned int timeout)
{
	assert(mbox != NULL);
	assert(message != NULL);

	uint32_t retries = 0;
	uint32_t has_written = 0;
	uint32_t fifoWord = 0;
	uint32_t bytesToWrite = 0U;

	if (!size)
		return has_written;

	bytesToWrite = sizeof(fifoWord);
	for (; size > 0;
	     message += bytesToWrite, size -= bytesToWrite, has_written += bytesToWrite) {
		for (retries = 0; retries < timeout; retries++) {
			if (!(mbox->FIFO_STATUS.FULL == FULL_L)) {
				if (bytesToWrite > size) {
					bytesToWrite = size;
				}
				fifoWord = 0U;
				memcpy((void *)&fifoWord, (void *)message, bytesToWrite);
				mbox->MESSAGE = fifoWord;
				break;
			}
			plat_time_delay(10);
		}

		if (retries == timeout) {
			ERROR("mcom03: %s: Is full timeout\n", __FUNCTION__);
			return has_written;
		}
	}

	return has_written;
}

/**
 * @brief function to read data from mailbox
 *
 * @param mbox    - pointer to mbox fifo
 * @param message - pointer to message received from mbox
 * @param size    - expected size of message in bytes
 * @param timeout - timeout
 * @return returns the number of successfully read bytes
 */
unsigned int mbox_read(mbox_fifo_regs_t *mbox, char *message, unsigned int size,
		       unsigned int timeout)
{
	assert(mbox != NULL);
	assert(message != NULL);

	uint32_t retries = 0;
	uint32_t has_read = 0;
	uint32_t fifoWord = 0;
	uint32_t bytesToRead = 0U;

	if (!size)
		return has_read;

	bytesToRead = sizeof(fifoWord);
	for (; size > 0; message += bytesToRead, size -= bytesToRead, has_read += bytesToRead) {
		for (retries = 0; retries < timeout; retries++) {
			if (!(mbox->FIFO_STATUS.EMPTY == EMPTY_L)) {
				if (bytesToRead > size) {
					bytesToRead = size;
				}
				fifoWord = mbox->MESSAGE;
				memcpy((void *)message, (void *)&fifoWord, bytesToRead);
				break;
			}
			plat_time_delay(10);
		}

		if (retries == timeout) {
			ERROR("mcom03: %s: Is full timeout\n", __FUNCTION__);
			return has_read;
		}
	}

	return has_read;
}
