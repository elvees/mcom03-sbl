// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <libs/mmio.h>
#include <libs/utils-def.h>

// PPolicy
#define PP_OFF      0x01
#define PP_WARM_RST 0x08
#define PP_ON       0x10

#define SET_PPOLICY(addr, value)                                                             \
	({                                                                                   \
		mmio_write_32((uintptr_t)(addr), (value) & 0x1F);                            \
		while ((mmio_read_32((uintptr_t)(addr) + 0x4) & 0x1F) != ((value) & 0x1F)) { \
		}                                                                            \
	})

void set_secure_region(void);
int soc_debug_if_disable(void);
