// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stdint.h>
#include <platform_helpers.h>
#include <tl_services/tl_services.h>
#include <drivers/mailbox/mailbox.h>
#include <mcom03.h>

#define HSPERIPH_BAR  0xDC
#define GPU_BAR	      0xD8
#define LSPERIPH0_BAR 0xE0
#define LSPERIPH1_BAR 0xE4

void tl_service_ddr_subs_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
				 TL_MBOX_SERVICES_resp_param_t *resp_param)
{
	uintptr_t addr = 0;

	if ((linkId != FIFO4) && (linkId != FIFO5)) {
		ERROR("DDR subsystem request is allowed from secure world only\r\n");
		return;
	}

	switch (cmd->hdr.func) {
	case TL_MBOX_SERVICES_DDR_SUBS_FUNC_SET_HSPERIPH_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + HSPERIPH_BAR;
		break;
	case TL_MBOX_SERVICES_DDR_SUBS_FUNC_SET_LSPERIPH0_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + LSPERIPH0_BAR;
		break;
	case TL_MBOX_SERVICES_DDR_SUBS_FUNC_SET_LSPERIPH1_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + LSPERIPH1_BAR;
		break;
	case TL_MBOX_SERVICES_DDR_SUBS_FUNC_SET_GPU_BAR:
		addr = BASE_ADDR_DDR_SYS_URB + GPU_BAR;
		break;
	default:
		ERROR("Unsupported DDR subsystem command=%d\r\n", cmd->hdr.func);
		break;
	}

	if (addr)
		*(uint32_t *)addr = cmd->param.ddr_subs.bar.value;
}
