// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <string.h>
#include <sys/cdefs.h>
#include <platform_helpers.h>
#include <lib/queue/queue.h>
#include <drivers/mailbox/mailbox.h>
#include "tl_ddr_subs.h"
#include "tl_init.h"
#include "tl_services_api.h"
#include "tl_wdt.h"

#define CRITICAL_SECTION_ENTER plat_disable_irq_global
#define CRITICAL_SECTION_EXIT  plat_enable_irq_global

typedef struct {
	uint32_t linkId;
	TL_MBOX_SERVICES_req_t req;
} TL_MBOX_SERVICES_msg_t;

static queue_t mbox_msgs = { 0 };

static void tl_service_push_queue(queue_t *queue, struct llist *queue_node)
{
	queue_push(queue, queue_node);
}

static struct llist *tl_service_pop_queue(queue_t *queue)
{
	return queue_pop(queue);
}

static void tl_service_irq_handler(unsigned int fifoNum)
{
	mailbox_regs_t *regs = mbox_get_regs(MAILBOX0);
	if (regs != NULL) {
		int empty = mbox_is_empty(&regs->mailbox[fifoNum]);
		if (!empty) {
			TL_MBOX_SERVICES_msg_t *msg =
				plat_allocate_memory(sizeof(TL_MBOX_SERVICES_msg_t));
			if (msg == NULL) {
				plat_panic_handler("No free memory\r\n");
			}

			unsigned int count = mbox_read(&regs->mailbox[fifoNum],
						       (char *)&msg->req.hdr, sizeof(msg->req.hdr),
						       100);
			if ((count == sizeof(msg->req.hdr)) &&
			    (msg->req.hdr.magic_num == TL_MBOX_SERVICES_MAGIC)) {
				if (msg->req.hdr.cmd_len != sizeof(msg->req.cmd)) {
					plat_panic_handler("Wrong cmd len=%d\r\n", count);
				}

				count = mbox_read(&regs->mailbox[fifoNum], (char *)&msg->req.cmd,
						  sizeof(msg->req.cmd), 100);
				if (count != sizeof(msg->req.cmd)) {
					plat_panic_handler("Wrong cmd len=%d \r\n", count);
				}

				if (msg->req.hdr.shrmem_len) {
					if (msg->req.hdr.shrmem_len != sizeof(msg->req.shrmem)) {
						plat_panic_handler("Wrong resp len=%d\r\n", count);
					}

					count = mbox_read(&regs->mailbox[fifoNum],
							  (char *)&msg->req.shrmem,
							  sizeof(msg->req.shrmem), 100);
					if (count != sizeof(msg->req.shrmem)) {
						plat_panic_handler("Wrong resp len=%d\r\n", count);
					}
				}

				msg->linkId = fifoNum;

				struct llist *queue_node =
					plat_allocate_memory(sizeof(struct llist));
				if (queue_node == NULL) {
					plat_panic_handler("No free memory\r\n");
				}

				queue_node->data = msg;
				tl_service_push_queue(&mbox_msgs, queue_node);
			}
		}
	}
}

static void tl_service_resp(TL_MBOX_SERVICES_req_t *req, TL_MBOX_SERVICES_resp_param_t *resp_param)
{
	TL_MBOX_SERVICES_resp_t *resp = NULL;

	// Protect Service subsytem from writing in it's own address space (first 4 GB)
	if (req->shrmem.data <= UINTPTR_MAX) {
		plat_panic_handler("The address[0x%llu] must be outside 32bit address space\r\n",
				   req->shrmem.data);
	}

	resp = (TL_MBOX_SERVICES_resp_t *)plat_map64(req->shrmem.data);
	if (resp == NULL) {
		plat_panic_handler("No free memory\r\n");
	}
	memcpy((void *)&resp->param, (void *)resp_param, sizeof(TL_MBOX_SERVICES_resp_param_t));
	plat_wmb();

	resp->state.value = TL_MBOX_SERVICES_RESP_STATE_COMPLETE;
	plat_wmb();

	plat_unmap64((uintptr_t)resp);
}

static void tl_service_cmd_handler(TL_MBOX_SERVICES_msg_t *msg)
{
	TL_MBOX_SERVICES_resp_param_t resp_param;

	switch (msg->req.cmd.hdr.service) {
	case TL_MBOX_SERVICES_INIT:
		tl_service_init_handler(msg->linkId, &msg->req.cmd, &resp_param);
		break;
	case TL_MBOX_SERVICES_WDT:
		tl_service_wdt_handler(msg->linkId, &msg->req.cmd, &resp_param);
		break;
	case TL_MBOX_SERVICES_DDR_SUBS:
		tl_service_ddr_subs_handler(msg->linkId, &msg->req.cmd, &resp_param);
		break;
	default:
		ERROR("Unsupported mbox service=%d\r\n", msg->req.cmd.hdr.service);
		break;
	}

	if (msg->req.hdr.shrmem_len) {
		tl_service_resp(&msg->req, &resp_param);
	}
}

uint32_t tl_services_start(void)
{
	COMPILE_TIME_ASSERT((sizeof(TL_MBOX_SERVICES_hdr_t) % sizeof(uint32_t)) == 0U);
	COMPILE_TIME_ASSERT((sizeof(TL_MBOX_SERVICES_cmd_t) % sizeof(uint32_t)) == 0U);
	COMPILE_TIME_ASSERT((sizeof(TL_MBOX_SERVICES_shrmem_t) % sizeof(uint32_t)) == 0U);

	mailbox_regs_t *regs = mbox_get_regs(MAILBOX0);
	int ret = mbox_attach_irq_handler(regs, tl_service_irq_handler);
	if (ret != 0) {
		plat_panic_handler("Failed to start services, ret=%d\r\n", ret);
	}

	return 0;
}

uint32_t tl_services_stop(void)
{
	mailbox_regs_t *regs = mbox_get_regs(MAILBOX0);
	int ret = mbox_detach_irq_handler(regs, tl_service_irq_handler);
	if (ret != 0) {
		plat_panic_handler("Failed to stop services, ret=%d\r\n", ret);
	}
	return 0;
}

void tl_services_handler(void)
{
	CRITICAL_SECTION_ENTER();
	struct llist *current = tl_service_pop_queue(&mbox_msgs);
	if (current != NULL) {
		if (current->data != NULL) {
			tl_service_cmd_handler((TL_MBOX_SERVICES_msg_t *)current->data);
			plat_free_memory(current->data);
		}
		plat_free_memory(current);
	}
	CRITICAL_SECTION_EXIT();
}
