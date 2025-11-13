// SPDX-License-Identifier: MIT
// Copyright 2024-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include "protocol.h"

void risc0_ipc_ddr_subs_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                                risc0_ipc_resp_param_t *resp_param);

void risc0_ipc_init_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                            risc0_ipc_resp_param_t *resp_param);

void risc0_ipc_pm_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                          risc0_ipc_resp_param_t *resp_param);

void risc0_ipc_wdt_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                           risc0_ipc_resp_param_t *resp_param);

void risc0_ipc_bootstage_handler(uint32_t link_id, const risc0_ipc_cmd_t *cmd,
                                 risc0_ipc_resp_param_t *resp_param);
