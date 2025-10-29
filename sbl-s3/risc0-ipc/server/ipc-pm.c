// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mailbox/mailbox.h>
#include <drivers/service/service.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/log.h>
#include <libs/utils-def.h>

#include "ipc.h"
#include "protocol.h"

void risc0_ipc_pm_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                          risc0_ipc_resp_param_t *resp_param)
{
	if ((link_id != FIFO4) && (link_id != FIFO5)) {
		ERROR("Power domain request is allowed from secure world only\n");
		return;
	}

	switch (cmd->hdr.func) {
	case RISC0_IPC_PM_FUNC_CHECK_SUPPORT:
		resp_param->pm.response.value =
			service_subsystem_pm_check_support(cmd->param.pm.toggle.id);
		break;
	case RISC0_IPC_PM_FUNC_ENABLE:
		resp_param->pm.response.value =
			service_subsystem_set_power(cmd->param.pm.toggle.id, PP_ON);
		break;
	case RISC0_IPC_PM_FUNC_DISABLE:
		resp_param->pm.response.value =
			service_subsystem_set_power(cmd->param.pm.toggle.id, PP_OFF);
		break;
	default:
		ERROR("Unsupported power domain command=%d\n", cmd->hdr.func);
		break;
	}
}
