// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#pragma once

#include "utils-def.h"

#define PLAT_RANDOM_CANARY_VALUE UL(0xBAADF00D)

#define PLAT_HEAP_POINTER_ALIGN 16

#define PLAT_IOMMU_BASE 0x40000000
#define PLAT_IOMMU_SIZE 0x3000

#define PLAT_IOMMU_VIRT_BASE_START 0xC0000000 // start of 2MiB slot section
#define PLAT_IOMMU_VIRT_SLOT_COUNT 256 // count of 2MiB blocks in the section

// Frequencies
#define MCOM03_XTI_CLK_HZ (27 * _MHz_)
#define HSP_CLK_HZ        MCOM03_XTI_CLK_HZ
#define QSPI1_CLK_HZ      MCOM03_XTI_CLK_HZ
#define QSPI1_EXT_CLK_HZ  (MCOM03_XTI_CLK_HZ / 2)
#define UART_CLK_HZ       MCOM03_XTI_CLK_HZ
