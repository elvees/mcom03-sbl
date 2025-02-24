// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#define RISC0_IPC_MAGIC 0x4D424F58 // "MBOX" in ASCII

typedef enum {
	RISC0_IPC_INIT = 0x00U,
	RISC0_IPC_RESERVED = 0x01U,
	RISC0_IPC_WDT = 0x02U,
	RISC0_IPC_PM = 0x04U,
	RISC0_IPC_DDR_SUBS = 0x05U,
	RISC0_IPC_COUNT,
} risc0_ipc;

typedef enum {
	RISC0_IPC_INIT_FUNC_GET_CAPABILITY = 0x00U,
	RISC0_IPC_INIT_FUNC_COUNT,
} risc0_ipc_init_func;

typedef enum {
	RISC0_IPC_WDT_FUNC_IS_ENABLE = 0x01U,
	RISC0_IPC_WDT_FUNC_START = 0x02U,
	RISC0_IPC_WDT_FUNC_PING = 0x03U,
	RISC0_IPC_WDT_FUNC_SET_TIMEOUT_S = 0x04U,
	RISC0_IPC_WDT_FUNC_GET_TIMEOUT_S = 0x05U,
	RISC0_IPC_WDT_FUNC_GET_MAX_TIMEOUT_S = 0x06U,
	RISC0_IPC_WDT_FUNC_GET_MIN_TIMEOUT_S = 0x07U,
	RISC0_IPC_WDT_FUNC_COUNT,
} risc0_ipc_wdt_func;

typedef enum {
	RISC0_IPC_PM_FUNC_CHECK_SUPPORT = 0x01U,
	RISC0_IPC_PM_FUNC_ENABLE = 0x02U,
	RISC0_IPC_PM_FUNC_DISABLE = 0x03U,
	RISC0_IPC_PM_FUNC_COUNT,
} risc0_ipc_pm_func;

typedef enum {
	RISC0_IPC_DDR_SUBS_FUNC_SET_HSPERIPH_BAR = 0x01U,
	RISC0_IPC_DDR_SUBS_FUNC_SET_LSPERIPH0_BAR = 0x02U,
	RISC0_IPC_DDR_SUBS_FUNC_SET_LSPERIPH1_BAR = 0x03U,
	RISC0_IPC_DDR_SUBS_FUNC_SET_GPU_BAR = 0x04U,
	RISC0_IPC_DDR_SUBS_FUNC_COUNT,
} risc0_ipc_ddr_subs_func;

typedef enum {
	RISC0_IPC_RESP_STATE_BUSY = 0x00U,
	RISC0_IPC_RESP_STATE_COMPLETE = 0x01U,
	RISC0_IPC_RESP_STATE_COUNT,
} risc0_ipc_resp_state;

#pragma pack(push, 1)
// Command params
typedef struct {
	uint64_t unused;
} risc0_ipc_reserved_t;

typedef struct {
	uint32_t timeout;
} risc0_ipc_wdt_start_t;

typedef struct {
	uint32_t value;
} risc0_ipc_wdt_set_timeout_t;

typedef struct {
	uint32_t id;
} risc0_ipc_pm_toggle_t;

typedef struct {
	uint32_t value;
} risc0_ipc_ddr_subs_set_bar_t;

typedef union {
	risc0_ipc_reserved_t reserved;
	union {
		risc0_ipc_wdt_start_t start;
		risc0_ipc_wdt_set_timeout_t set_timeout;
	} wdt;
	union {
		risc0_ipc_pm_toggle_t toggle;
	} pm;
	union {
		risc0_ipc_ddr_subs_set_bar_t bar;
	} ddr_subs;
} risc0_ipc_cmd_param_t;

// Response params
typedef struct {
	uint32_t value;
} risc0_ipc_init_get_capability_t;

typedef struct {
	uint32_t value;
} risc0_ipc_wdt_is_enable_t;

typedef struct {
	uint32_t value;
} risc0_ipc_wdt_get_timeout_t;

typedef struct {
	uint32_t value;
} risc0_ipc_wdt_get_max_timeout_t;

typedef struct {
	uint32_t value;
} risc0_ipc_wdt_get_min_timeout_t;

typedef struct {
	uint32_t value;
} risc0_ipc_pm_resp_t;

typedef union {
	union {
		risc0_ipc_init_get_capability_t capability;
	} init;
	union {
		risc0_ipc_wdt_is_enable_t is_enable;
		risc0_ipc_wdt_get_timeout_t timeout;
		risc0_ipc_wdt_get_max_timeout_t min_timeout;
		risc0_ipc_wdt_get_min_timeout_t max_timeout;
	} wdt;
	union {
		risc0_ipc_pm_resp_t response;
	} pm;
} risc0_ipc_resp_param_t;

// Command message
typedef struct {
	uint16_t service;
	uint16_t func;
} risc0_ipc_cmd_hdr_t;

typedef struct {
	risc0_ipc_cmd_hdr_t hdr;
	risc0_ipc_cmd_param_t param;
} risc0_ipc_cmd_t;

// Response message
typedef struct {
	uint32_t value;
} risc0_ipc_resp_state_t;

typedef struct {
	risc0_ipc_resp_state_t state;
	risc0_ipc_resp_param_t param;
} risc0_ipc_resp_t;

typedef struct {
	uint64_t data;
} risc0_ipc_shrmem_t;

typedef struct {
	uint32_t magic_num;
	uint16_t cmd_len;
	uint16_t shrmem_len;
} risc0_ipc_hdr_t;

typedef struct {
	risc0_ipc_hdr_t hdr;
	risc0_ipc_cmd_t cmd;
	risc0_ipc_shrmem_t shrmem;
} risc0_ipc_req_t;
#pragma pack(pop)
