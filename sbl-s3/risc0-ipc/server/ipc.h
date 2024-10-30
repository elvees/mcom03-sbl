// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#ifndef __IPC_H__
#define __IPC_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <tl_services/tl_services.h>

void tl_service_ddr_subs_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
				 TL_MBOX_SERVICES_resp_param_t *resp_param);

void tl_service_init_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
			     TL_MBOX_SERVICES_resp_param_t *resp_param);

void tl_service_wdt_handler(uint32_t linkId, const TL_MBOX_SERVICES_cmd_t *cmd,
			    TL_MBOX_SERVICES_resp_param_t *resp_param);

#endif /* __IPC_H__ */
