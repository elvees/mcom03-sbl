// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#include "env-io.h"

int32_t env_io_ram_init(env_io_config_t *cfg, signed long offset, size_t size);

#ifdef __cplusplus
}
#endif
