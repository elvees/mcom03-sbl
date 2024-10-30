// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <drivers/mcom03-regs.h>
#include <drivers/qlic/irq.h>
#include <drivers/qlic/qlic.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/utils-def.h>
#include <third-party/llist/llist.h>

#include "mailbox.h"

/**
 * TODO: block access to mailbox if it is already used by someone.
 * TODO: spinlock when capturing mailbox.
 */

typedef struct {
	struct llist *nodes;
} mbox_irq_t;

static mbox_irq_t mbox_irq = { 0 };

static void irq_mailbox_handler(unsigned int id)
{
	struct llist *node;

	unsigned int fifo_num = id - QLIC0_ID_MB0_RRQ_RISC0;

	assert(fifo_num < MAILBOX_FIFO_COUNT);

	mailbox_regs_t *mb0 = (mailbox_regs_t *)BASE_ADDR_SERVICE_MAILBOX0;
	unsigned int irq_read = mb0->mailbox[fifo_num].set_irq_read;

	// Iterate over list and run handler if exists
	for_each_llist(node, mbox_irq.nodes) {
		mbox_irq_handler_t handle = (mbox_irq_handler_t)node->data;
		if (handle)
			handle(fifo_num);
	}
	mb0->mailbox[fifo_num].clr_irq_read = irq_read;
}

/**
 * @brief Software reset mailbox device
 *
 * @param regs - Pointer to mailbox registers
 *
 * @return  0     - Success,
 *         -ENULL - regs is not provided (NULL pointers)
 */
static int mbox_reset(mailbox_regs_t *regs)
{
	if (!regs)
		return -ENULL;
	regs->soft_reset = 1;
	return 0;
}

mailbox_regs_t *mbox_get_regs(void)
{
	return (mailbox_regs_t *)BASE_ADDR_SERVICE_MAILBOX0;
}

int mbox_init(mailbox_regs_t *regs)
{
	int ret;

	ret = mbox_reset(regs);
	if (ret)
		return ret;

	for (uint8_t fifo_num = 0; fifo_num < MAILBOX_FIFO_COUNT; ++fifo_num) {
		ret = irq_attach_handler(fifo_num + QLIC0_ID_MB0_RRQ_RISC0, irq_mailbox_handler);
		if (ret)
			return ret;
	}

	return 0;
}

int mbox_deinit(mailbox_regs_t *regs)
{
	int ret;

	if (!regs)
		return -ENULL;

	for (uint8_t fifo_num = 0; fifo_num < MAILBOX_FIFO_COUNT; ++fifo_num) {
		ret = irq_detach_handler(fifo_num + QLIC0_ID_MB0_RRQ_RISC0);
		if (ret)
			return ret;
	}

	return mbox_reset(regs);
}

int mbox_is_empty(mbox_fifo_regs_t *mbox)
{
	if (!mbox)
		return -ENULL;
	return mbox->fifo_status.empty;
}

int mbox_attach_irq_handler(mbox_irq_handler_t mbox_irq_handler)
{
	struct llist *node;

	// Check if handler is already registered
	for_each_llist(node, mbox_irq.nodes)
		if ((mbox_irq_handler_t)node->data == mbox_irq_handler)
			return 0;

	node = malloc(sizeof(struct llist));
	if (!node)
		return -ENULL;

	node->data = (void *)mbox_irq_handler;

	add_to_list((struct llist **)&mbox_irq.nodes, node);

	return 0;
}

int mbox_detach_irq_handler(mbox_irq_handler_t mbox_irq_handler)
{
	struct llist *node;

	// Iterate over list and remove handler if exists
	for_each_llist(node, mbox_irq.nodes)
		if ((mbox_irq_handler_t)node->data == mbox_irq_handler) {
			remove_from_list((struct llist **)&mbox_irq.nodes, node);
			free(node);
			break;
		}

	return 0;
}

unsigned int mbox_write(mbox_fifo_regs_t *mbox, const char *message, unsigned int size)
{
	assert(mbox != NULL);
	assert(message != NULL);

	uint32_t retries = 0;
	uint32_t has_written = 0;
	uint32_t fifo_word = 0;
	uint32_t bytes_to_write = 0U;

	if (!size)
		return has_written;

	bytes_to_write = sizeof(fifo_word);
	for (; size > 0;
	     message += bytes_to_write, size -= bytes_to_write, has_written += bytes_to_write) {
		for (retries = 0; retries < MBOX_MAX_RETRIES; retries++) {
			if (!(mbox->fifo_status.full == FULL_L)) {
				if (bytes_to_write > size)
					bytes_to_write = size;
				fifo_word = 0U;
				memcpy((void *)&fifo_word, (void *)message, bytes_to_write);
				mbox->message = fifo_word;
				break;
			}
			time_delay(10);
		}

		if (retries == MBOX_MAX_RETRIES) {
			ERROR("mcom03: %s: Is full timeout\n", __FUNCTION__);
			return has_written;
		}
	}

	return has_written;
}

unsigned int mbox_read(mbox_fifo_regs_t *mbox, char *message, unsigned int size)
{
	assert(mbox != NULL);
	assert(message != NULL);

	uint32_t retries = 0;
	uint32_t has_read = 0;
	uint32_t fifo_word = 0;
	uint32_t bytes_to_read = 0U;

	if (!size)
		return has_read;

	bytes_to_read = sizeof(fifo_word);
	for (; size > 0;
	     message += bytes_to_read, size -= bytes_to_read, has_read += bytes_to_read) {
		for (retries = 0; retries < MBOX_MAX_RETRIES; retries++) {
			if (!(mbox->fifo_status.empty == EMPTY_L)) {
				if (bytes_to_read > size)
					bytes_to_read = size;
				fifo_word = mbox->message;
				memcpy((void *)message, (void *)&fifo_word, bytes_to_read);
				break;
			}
			time_delay(10);
		}

		if (retries == MBOX_MAX_RETRIES) {
			ERROR("mcom03: %s: Is full timeout\n", __FUNCTION__);
			return has_read;
		}
	}

	return has_read;
}
