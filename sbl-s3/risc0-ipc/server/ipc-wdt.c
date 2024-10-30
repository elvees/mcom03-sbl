// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stdint.h>
#include <platform_helpers.h>
#include <drivers/mailbox/mailbox.h>
#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
#endif /* WDT_ENABLE */
#include "tl_wdt.h"

void tl_service_wdt_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
			    TL_MBOX_SERVICES_resp_param_t *resp_param)
{
#ifdef WDT_ENABLE
	uint32_t ret;

	extern wdt_dev_t wdt;
	if ((linkId != FIFO4) && (linkId != FIFO5)) {
		ERROR("WDT request is allowed from secure world only\r\n");
		return;
	}

	switch (cmd->hdr.func) {
	case TL_MBOX_SERVICES_WDT_FUNC_START:
		ret = wdt_init(&wdt);
		if ((ret != MCOM03_SUCCESS) && (ret != MCOM03_ERROR_ALREADY_INITIALIZED)) {
			plat_panic_handler("WDT init failed ret=%ld\r\n", ret);
		}
		ret = wdt_set_timeout(&wdt, cmd->param.wdt.start.timeout * 1000U);
		if (ret != MCOM03_SUCCESS) {
			plat_panic_handler("WDT start failed ret=%ld\r\n", ret);
		}
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_PING:
		wdt_reset(&wdt);
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_SET_TIMEOUT_S:
		ret = wdt_set_timeout(&wdt, cmd->param.wdt.setTimeout.value * 1000U);
		if (ret != MCOM03_SUCCESS) {
			plat_panic_handler("WDT start failed ret=%ld\r\n", ret);
		}
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_IS_ENABLE:
		resp_param->wdt.isEnable.value = wdt_is_enabled(&wdt);
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_GET_TIMEOUT_S:
		resp_param->wdt.timeout.value = wdt_get_timeout(&wdt) / 1000U;
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_GET_MAX_TIMEOUT_S:
		resp_param->wdt.maxTimeout.value = wdt_get_max_timeout(&wdt) / 1000U;
		break;
	case TL_MBOX_SERVICES_WDT_FUNC_GET_MIN_TIMEOUT_S:
		resp_param->wdt.minTimeout.value = wdt_get_min_timeout(&wdt) / 1000U;
		break;
	default:
		ERROR("Unsupported wdt command=%d\r\n", cmd->hdr.func);
		break;
	}
#endif /* WDT_ENABLE */
}
