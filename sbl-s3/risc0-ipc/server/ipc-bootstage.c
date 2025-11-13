// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mailbox/mailbox.h>
#include <libs/log.h>

#include "ipc.h"
#include "protocol.h"

#if defined(BOOTSTAGE_ENABLE)
#include <libs/bootstage/bootstage.h>
#endif

void risc0_ipc_bootstage_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                                 risc0_ipc_resp_param_t *resp_param)
{
	if ((link_id != FIFO4) && (link_id != FIFO5)) {
		ERROR("Bootstage request is allowed from secure world only\r\n");
		return;
	}

#if defined(BOOTSTAGE_ENABLE)
	switch (cmd->hdr.func) {
	case RISC0_IPC_BOOTSTAGE_FUNC_SET_STAGE:
		bootstage_mark(cmd->param.bootstage.set_stage.id);
		break;
	case RISC0_IPC_BOOTSTAGE_FUNC_GET_TIMESTAMP:
		resp_param->bootstage.get_timestamp.value =
			bootstage_get_timestamp(cmd->param.bootstage.get_timestamp.id);
		break;
	default:
		ERROR("Unsupported Bootstage command=%d\r\n", cmd->hdr.func);
		break;
	}
#endif
}
