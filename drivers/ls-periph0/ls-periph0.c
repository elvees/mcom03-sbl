// Copyright 2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stddef.h>
#include <stdint.h>

#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/utils-def.h>

#include "ls-periph0.h"

static int lsp0_cfg_clks(void)
{
	int ret;
	ucg_regs_t *ucg = ucg_get_registers(UCG_SUBSYS_LSP0, 0);

	ret = ucg_enable_bp(ucg, LSP0_UCG2_ALL_CH_MASK);
	if (ret)
		return ret;

	// Initialize the UCG register for clocking GPIO0
	ret = ucg_set_divider(ucg, LSP0_UCG2_CLK_GPIO0, 1, 1000);
	if (ret)
		return ret;

	return ucg_sync_and_disable_bp(ucg, LSP0_UCG2_ALL_CH_MASK, LSP0_UCG2_SYNC_MASK);
}

int lsp0_enable(void)
{
	/**
	 * Initialize and configure the LS Peripheral 0 clocking system.
	 * Release reset signal of LS Peripheral 0.
	 */
	service_enable_lsp0();

	return lsp0_cfg_clks();
}
