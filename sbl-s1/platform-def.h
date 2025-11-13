// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/platform-def-common.h>

#define PREFIX "SBL-S1"

#undef PLAT_BOOTSTAGE_SIZE
#define PLAT_BOOTSTAGE_SIZE 0x80

#define PLAT_SBL_S1_BASE 0x80000000
// Last 4096 bytes of 64K SPRAM are used by BootROM
// PLAT_BOOTSTAGE_SIZE bytes are used by bootstage
#define PLAT_SBL_S1_SIZE (0xF000 - PLAT_BOOTSTAGE_SIZE)

#undef PLAT_BOOTSTAGE_BASE
#define PLAT_BOOTSTAGE_BASE (PLAT_SBL_S1_BASE + PLAT_SBL_S1_SIZE)
