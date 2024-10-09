// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <drivers/mcom03-regs.h>
#include <libs/helpers/helpers.h>

#include "ls-periph1.h"

ls1_urb_regs_t *ls1_periph_get_urb_registers(void)
{
	return (ls1_urb_regs_t *)BASE_ADDR_LS1_URB_BASE;
}

void lsp1_i2s_ucg1_rstn(void)
{
	ls1_urb_regs_t *urb = ls1_periph_get_urb_registers();

	SET_PPOLICY(&urb->i2s_ucg_rstn_ppolicy, PP_ON);
}
