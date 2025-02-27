// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/ddr/ddr.h>
#include <drivers/mailbox/mailbox.h>
#include <libs/log.h>

#include "ipc.h"
#include "protocol.h"

#define HSPERIPH_BAR  0xDC
#define GPU_BAR       0xD8
#define LSPERIPH0_BAR 0xE0
#define LSPERIPH1_BAR 0xE4

void risc0_ipc_ddr_subs_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                                risc0_ipc_resp_param_t *resp_param)
{
	uintptr_t addr = 0;

	if ((link_id != FIFO4) && (link_id != FIFO5)) {
		ERROR("DDR subsystem request is allowed from secure world only\r\n");
		return;
	}

	switch (cmd->hdr.func) {
	case RISC0_IPC_DDR_SUBS_FUNC_SET_HSPERIPH_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + HSPERIPH_BAR;
		break;
	case RISC0_IPC_DDR_SUBS_FUNC_SET_LSPERIPH0_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + LSPERIPH0_BAR;
		break;
	case RISC0_IPC_DDR_SUBS_FUNC_SET_LSPERIPH1_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + LSPERIPH1_BAR;
		break;
	case RISC0_IPC_DDR_SUBS_FUNC_SET_GPU_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + GPU_BAR;
		break;
	default:
		ERROR("Unsupported DDR subsystem command=%d\r\n", cmd->hdr.func);
		break;
	}

	if (addr)
		*(uint32_t *)addr = cmd->param.ddr_subs.bar.value;
}
