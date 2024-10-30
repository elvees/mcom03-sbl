// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stdint.h>
#include <platform_helpers.h>
#include <drivers/mailbox/mailbox.h>
#include "tl_init.h"

void tl_service_init_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
			     TL_MBOX_SERVICES_resp_param_t *resp_param)
{
	switch (cmd->hdr.func) {
	case TL_MBOX_SERVICES_INIT_FUNC_GET_CAPABILITY:
		resp_param->init.capability.value = BIT(TL_MBOX_SERVICES_INIT);
		switch (linkId) {
		case FIFO4:
		case FIFO5:
			resp_param->init.capability.value |= BIT(TL_MBOX_SERVICES_WDT);
			resp_param->init.capability.value |= BIT(TL_MBOX_SERVICES_DDR_SUBS);
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
