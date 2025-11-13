// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mailbox/mailbox.h>
#include <libs/log.h>
#include <libs/utils-def.h>

#include "ipc.h"
#include "protocol.h"

void risc0_ipc_init_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                            risc0_ipc_resp_param_t *resp_param)
{
	switch (cmd->hdr.func) {
	case RISC0_IPC_INIT_FUNC_GET_CAPABILITY:
		resp_param->init.capability.value = BIT(RISC0_IPC_INIT);
		switch (link_id) {
		case FIFO4:
		case FIFO5:
			resp_param->init.capability.value |= BIT(RISC0_IPC_WDT);
			resp_param->init.capability.value |= BIT(RISC0_IPC_PM);
			resp_param->init.capability.value |= BIT(RISC0_IPC_DDR_SUBS);
#if defined(BOOTSTAGE_ENABLE)
			resp_param->init.capability.value |= BIT(RISC0_IPC_BOOTSTAGE);
#endif
			break;
		default:
			break;
		}
		break;
	default:
		ERROR("Unsupported init command=%d\r\n", cmd->hdr.func);
		break;
	}
}
