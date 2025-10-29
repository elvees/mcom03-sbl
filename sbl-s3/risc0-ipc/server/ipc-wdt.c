// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mailbox/mailbox.h>
#include <libs/errors.h>
#include <libs/log.h>

#include "ipc.h"
#include "protocol.h"

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif

void risc0_ipc_wdt_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                           risc0_ipc_resp_param_t *resp_param)
{
#ifdef WDT_ENABLE
	int ret;
	wdt_dev_t *wdt = wdt_get_instance();

	if ((link_id != FIFO4) && (link_id != FIFO5)) {
		ERROR("WDT request is allowed from secure world only\n");
		return;
	}

	switch (cmd->hdr.func) {
	case RISC0_IPC_WDT_FUNC_START:
		ret = wdt_init(wdt);
		if (ret && (ret != -EALREADYINITIALIZED))
			panic_handler("Failed to init WDT, ret=%d\n", ret);
		ret = wdt_set_timeout(wdt, cmd->param.wdt.start.timeout * 1000U);
		if (ret)
			panic_handler("Failed to set WDT timeout, ret=%d\n", ret);
		break;
	case RISC0_IPC_WDT_FUNC_PING:
		wdt_reset(wdt);
		break;
	case RISC0_IPC_WDT_FUNC_SET_TIMEOUT_S:
		ret = wdt_set_timeout(wdt, cmd->param.wdt.set_timeout.value * 1000U);
		if (ret)
			panic_handler("Failed to set WDT timeout, ret=%d\n", ret);
		break;
	case RISC0_IPC_WDT_FUNC_IS_ENABLE:
		resp_param->wdt.is_enable.value = wdt_is_enabled(wdt);
		break;
	case RISC0_IPC_WDT_FUNC_GET_TIMEOUT_S:
		resp_param->wdt.timeout.value = wdt_get_timeout(wdt) / 1000U;
		break;
	case RISC0_IPC_WDT_FUNC_GET_MAX_TIMEOUT_S:
		resp_param->wdt.max_timeout.value = wdt_get_max_timeout(wdt) / 1000U;
		break;
	case RISC0_IPC_WDT_FUNC_GET_MIN_TIMEOUT_S:
		resp_param->wdt.min_timeout.value = wdt_get_min_timeout(wdt) / 1000U;
		break;
	default:
		ERROR("Unsupported wdt command=%d\n", cmd->hdr.func);
		break;
	}
#endif
}
