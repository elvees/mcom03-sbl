// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "hs-periph-urb.h"

inline hs_urb_regs_t *hs_periph_get_urb_registers(void)
{
	return (hs_urb_regs_t *)BASE_ADDR_HS_URB;
}
