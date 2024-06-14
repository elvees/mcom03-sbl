// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "ls-periph1-urb.h"

inline ls1_urb_regs_t *ls1_periph_get_urb_registers(void)
{
	return (ls1_urb_regs_t *)BASE_ADDR_LS1_URB_BASE;
}
