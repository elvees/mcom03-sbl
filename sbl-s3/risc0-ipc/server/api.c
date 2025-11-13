// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <malloc.h>
#include <string.h>
#include <sys/cdefs.h>

#include <drivers/iommu/iommu.h>
#include <drivers/irq/irq.h>
#include <drivers/mailbox/mailbox.h>
#include <drivers/mips-cp0/mips-cp0.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/queue/queue.h>
#include <libs/utils-def.h>

#include "api.h"
#include "ipc.h"
#include "protocol.h"

#define CRITICAL_SECTION_ENTER mips_global_irq_disable
#define CRITICAL_SECTION_EXIT  mips_global_irq_enable

typedef struct {
	uint32_t link_id;
	risc0_ipc_req_t req;
} risc0_ipc_msg_t;

static queue_t mbox_msgs = { 0 };

static void risc0_ipc_irq_handler(unsigned int fifo_num)
{
	mailbox_regs_t *regs = mbox_get_regs();

	int empty = mbox_is_empty(&regs->mailbox[fifo_num]);
	if (empty)
		return;

	risc0_ipc_msg_t *msg = malloc(sizeof(risc0_ipc_msg_t));
	if (!msg)
		panic_handler("No free memory\r\n");

	unsigned int count =
		mbox_read(&regs->mailbox[fifo_num], (char *)&msg->req.hdr, sizeof(msg->req.hdr));
	if (count != sizeof(msg->req.hdr) || msg->req.hdr.magic_num != RISC0_IPC_MAGIC)
		return;

	if (msg->req.hdr.cmd_len != sizeof(msg->req.cmd))
		panic_handler("Wrong cmd len=%d\r\n", count);

	count = mbox_read(&regs->mailbox[fifo_num], (char *)&msg->req.cmd, sizeof(msg->req.cmd));
	if (count != sizeof(msg->req.cmd))
		panic_handler("Wrong cmd len=%d \r\n", count);

	if (msg->req.hdr.shrmem_len) {
		if (msg->req.hdr.shrmem_len != sizeof(msg->req.shrmem))
			panic_handler("Wrong resp len=%d\r\n", count);

		count = mbox_read(&regs->mailbox[fifo_num], (char *)&msg->req.shrmem,
		                  sizeof(msg->req.shrmem));
		if (count != sizeof(msg->req.shrmem))
			panic_handler("Wrong resp len=%d\r\n", count);
	}

	msg->link_id = fifo_num;

	queue_push(&mbox_msgs, msg);
}

static void risc0_ipc_resp(risc0_ipc_req_t *req, risc0_ipc_resp_param_t *resp_param)
{
	risc0_ipc_resp_t *resp = NULL;
	iommu_regs_t *iommu_regs = iommu_get_registers();

	// Protect Service subsystem from writing in it's own address space (first 4 GB)
	if (req->shrmem.data <= UINTPTR_MAX)
		panic_handler("The address[0x%llu] must be outside 32bit address space\r\n",
		              req->shrmem.data);

	resp = (risc0_ipc_resp_t *)iommu_map(iommu_regs, req->shrmem.data);
	if (!resp)
		panic_handler("No free memory\r\n");
	memcpy((void *)&resp->param, (void *)resp_param, sizeof(risc0_ipc_resp_param_t));
	wmem_barrier();

	resp->state.value = RISC0_IPC_RESP_STATE_COMPLETE;
	wmem_barrier();

	iommu_unmap(iommu_regs, (uintptr_t)resp);
}

static void risc0_ipc_cmd_handler(risc0_ipc_msg_t *msg)
{
	risc0_ipc_resp_param_t resp_param;

	switch (msg->req.cmd.hdr.service) {
	case RISC0_IPC_INIT:
		risc0_ipc_init_handler(msg->link_id, &msg->req.cmd, &resp_param);
		break;
	case RISC0_IPC_WDT:
		risc0_ipc_wdt_handler(msg->link_id, &msg->req.cmd, &resp_param);
		break;
	case RISC0_IPC_PM:
		risc0_ipc_pm_handler(msg->link_id, &msg->req.cmd, &resp_param);
		break;
	case RISC0_IPC_DDR_SUBS:
		risc0_ipc_ddr_subs_handler(msg->link_id, &msg->req.cmd, &resp_param);
		break;
	case RISC0_IPC_BOOTSTAGE:
		risc0_ipc_bootstage_handler(msg->link_id, &msg->req.cmd, &resp_param);
		break;
	default:
		ERROR("Unsupported mbox service=%d\r\n", msg->req.cmd.hdr.service);
		break;
	}

	if (msg->req.hdr.shrmem_len)
		risc0_ipc_resp(&msg->req, &resp_param);
}

uint32_t risc0_ipc_start(void)
{
	COMPILE_TIME_ASSERT(!(sizeof(risc0_ipc_hdr_t) % sizeof(uint32_t)));
	COMPILE_TIME_ASSERT(!(sizeof(risc0_ipc_cmd_t) % sizeof(uint32_t)));
	COMPILE_TIME_ASSERT(!(sizeof(risc0_ipc_shrmem_t) % sizeof(uint32_t)));

	int ret = mbox_attach_irq_handler(risc0_ipc_irq_handler);
	if (ret)
		panic_handler("Failed to start services, ret=%d\r\n", ret);

	return 0;
}

uint32_t risc0_ipc_stop(void)
{
	int ret = mbox_detach_irq_handler(risc0_ipc_irq_handler);
	if (ret)
		panic_handler("Failed to stop services, ret=%d\r\n", ret);
	return 0;
}

void risc0_ipc_handler(void)
{
	// Ensure that all instructions are completed before enter to critical section
	mem_barrier();

	CRITICAL_SECTION_ENTER();
	void *data = queue_pop(&mbox_msgs);
	if (data) {
		risc0_ipc_cmd_handler((risc0_ipc_msg_t *)data);
		free(data);
	}
	CRITICAL_SECTION_EXIT();
}
