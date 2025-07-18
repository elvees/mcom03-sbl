// SPDX-License-Identifier: MIT
// Copyright 2019-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/platform-def-common.h>

#define PLAT_SBL_S2_BASE 0x48000000
#define PLAT_SBL_S2_SIZE 0x02000000

#define PLAT_ENV_SIZE          0x10000
#define PLAT_SBL_ENV_OFF       -0x30000
#define PLAT_UBOOT_ENV_OFF     0xFE0000
#define PLAT_UBOOT_OLD_ENV_OFF -0x40000

#define PLAT_OTP_OFFSET        0xA00000
#define PLAT_OFFSET_FIRMWARE_A 0x200000
#define PLAT_OFFSET_FIRMWARE_B 0x600000
#define PLAT_OFFSET_FIRMWARE_R 0xA10000

#define PLAT_RECOVERY_TIMEOUT_SEC 30
