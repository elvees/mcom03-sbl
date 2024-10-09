// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <drivers/mcom03-regs.h>
#include <libs/helpers/helpers.h>

#include "ls-periph1.h"

lsp1_urb_regs_t *lsp1_get_urb_registers(void)
{
	return (lsp1_urb_regs_t *)BASE_ADDR_LSP1_URB_BASE;
}

void lsp1_i2s_ucg1_rstn(void)
{
	lsp1_urb_regs_t *urb = lsp1_get_urb_registers();

	SET_PPOLICY(&urb->i2s_ucg_rstn_ppolicy, PP_ON);
}
