// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

uint32_t risc0_ipc_start(void);
uint32_t risc0_ipc_stop(void);
void risc0_ipc_handler(void);
