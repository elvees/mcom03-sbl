// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "hs-periph.h"

hsp_urb_regs_t *hsp_get_urb_registers(void)
{
	return (hsp_urb_regs_t *)BASE_ADDR_HSP_URB;
}

void hsp_refclk_setup(void)
{
	hsp_urb_regs_t *urb = hsp_get_urb_registers();
	urb->refclk = 0;
}
