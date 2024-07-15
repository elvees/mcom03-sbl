// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <mcom03.h>
#include "urb.h"

cpu_subs_urb_regs_t *getCPUSubsURBRegisters()
{
	return (cpu_subs_urb_regs_t *)BASE_ADDR_CPU_SUBS_URB;
}
