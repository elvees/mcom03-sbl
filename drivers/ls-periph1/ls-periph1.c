// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <drivers/mcom03-regs.h>

#include "ls-periph1.h"

ls1_urb_regs_t *ls1_periph_get_urb_registers(void)
{
	return (ls1_urb_regs_t *)BASE_ADDR_LS1_URB_BASE;
}
