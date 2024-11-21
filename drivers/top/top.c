// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/mcom03-regs.h>
#include <drivers/pll/pll.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "top.h"

static struct ucg_channel top_ucg_channels[] = {
	{ 0, TOP_UCG0_CHANNEL_DDR_DP, 6 }, // 198 MHz
	{ 0, TOP_UCG0_CHANNEL_DDR_VPU, 4 }, // 297 MHz
	{ 0, TOP_UCG0_CHANNEL_DDR_GPU, 4 }, // 297 MHz
	{ 0, TOP_UCG0_CHANNEL_DDR_ISP, 6 }, // 198 MHz
	{ 0, TOP_UCG0_CHANNEL_DDR_CPU, 2 }, // 594 MHz
	{ 0, TOP_UCG0_CHANNEL_CPU_ACP, 4 }, // 297 MHz
	{ 0, TOP_UCG0_CHANNEL_DDR_LSP0, 12 }, // 99 MHz
	{ 0, TOP_UCG0_CHANNEL_AXI_COH_COMM, 2 }, // 594 MHz

	/**
	 * To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{ 1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM, 30 }, // 39.6 MHz
	{ 1, TOP_UCG1_CHANNEL_AXI_FAST_COMM, 8 }, // 148.5 MHz
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_DSP, 2 }, // 594 MHz
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_PICE, 4 }, // 297 MHz
	{ 1, TOP_UCG1_CHANNEL_DDR_LSP1, 12 }, // 99 MHz
	{ 1, TOP_UCG1_CHANNEL_DDR_SERVICE, 8 }, // 148.5 MHz
	{ 1, TOP_UCG1_CHANNEL_DDR_HSP, 6 }, // 198 MHz
};

top_urb_regs_t *top_get_urb_registers(void)
{
	return (top_urb_regs_t *)(BASE_ADDR_TOP_URB_BASE);
}

int top_set_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->nonsecuretosecure = mask;

	return 0;
}

int top_reset_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->nonsecuretosecure &= ((~mask) & TOP_NONSECURETOSECURE_MASK);

	return 0;
}

int top_set_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->trustedtosecure = mask;

	return 0;
}

int top_reset_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->trustedtosecure &= ((~mask) & TOP_TRUSTEDTOSECURE_MASK);

	return 0;
}

int top_set_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->ddr_low_range_secure_ctr = mask;

	return 0;
}

int top_reset_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask)
{
	if (!urb)
		return -ENULL;

	urb->ddr_low_range_secure_ctr &= ((~mask) & TOP_DDR_LOW_RANGE_SECURE_CTR_MASK);

	return 0;
}

int top_set_clock(void)
{
	int ret;
	uint32_t top_mask[] = { TOP_UCG0_ALL_CH_MASK, TOP_UCG1_ALL_CH_MASK };
	ucg_regs_t *top_ucg[ARRAY_SIZE(top_mask)];

	top_urb_regs_t *urb = top_get_urb_registers();

	// Enable interconnect UCG[i] bypass
	for (int i = 0; i < ARRAY_SIZE(top_mask); i++) {
		top_ucg[i] = ucg_get_registers(UCG_SUBSYS_TOP, i);
		ret = ucg_enable_bp(top_ucg[i], top_mask[i]);
		if (ret)
			return ret;
	}

	/**
	 * Setup PLL to 1188 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 87;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 1;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)(&urb->pllcfg), &pll_cfg, 1000);
	if (ret)
		return ret;

	// Set dividers
	for (int i = 0; i < ARRAY_SIZE(top_ucg_channels); i++) {
		ret = ucg_set_divider(top_ucg[top_ucg_channels[i].ucg_id],
		                      top_ucg_channels[i].chan_id, top_ucg_channels[i].div, 1000);
		if (ret)
			return ret;
	}

	// Sync and disable interconnect UCG[i] bypass
	for (int i = 0; i < ARRAY_SIZE(top_mask); i++) {
		ret = ucg_sync_and_disable_bp(top_ucg[i], top_mask[i], top_mask[i]);
		if (ret)
			return ret;
	}

	return 0;
}
