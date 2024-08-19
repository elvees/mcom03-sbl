// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <drivers/mcom03-regs.h>

#include "hs-periph.h"

hs_urb_regs_t *hs_periph_get_urb_registers(void)
{
	return (hs_urb_regs_t *)BASE_ADDR_HS_URB;
}
