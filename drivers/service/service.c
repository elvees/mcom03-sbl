// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/mcom03-regs.h>
#include <drivers/pll/pll.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/platform-def-common.h>

#include "service.h"

#define SERV_UCG_APB_CHANNEL_DIV  6
#define SERV_UCG_CORE_CHANNEL_DIV 1

// Service Subsystem PLL output frequency is 594 MHz, assuming that XTI = 27 MHz
struct ucg_channel serv_ucg_channels[] = {
	{ 1, SERVICE_UCG1_CHANNEL_CLK_APB, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_CORE, SERV_UCG_CORE_CHANNEL_DIV }, // 594 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_QSPI0, SERV_UCG_CORE_CHANNEL_DIV }, // 594 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_BPAM, SERV_UCG_CORE_CHANNEL_DIV }, // 594 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_RISC0, SERV_UCG_CORE_CHANNEL_DIV }, // 594 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_MFBSP0, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_MFBSP1, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_MAILBOX0, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_PVTCTR, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_I2C4, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_TRNG, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_SPIOTP, SERV_UCG_APB_CHANNEL_DIV }, // 99 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_I2C4_EXT, 12 }, // 49.5 MHz
	{ 1, SERVICE_UCG1_CHANNEL_CLK_QSPI0_EXT, 22 }, // 27 MHz
};

void service_enable_arm_cpu(void)
{
	service_urb_regs_t *urb = service_get_urb_registers();
	SET_PPOLICY(&urb->cpu_ppolicy, PP_ON);
	urb->top_clkgate |= SERVICE_TOP_CLK_GATE_CPU;
}

void service_disable_arm_cpu(void)
{
	service_urb_regs_t *urb = service_get_urb_registers();
	urb->top_clkgate &= ~SERVICE_TOP_CLK_GATE_CPU;
	SET_PPOLICY(&urb->cpu_ppolicy, PP_OFF);
}

void service_enable_lsp1(void)
{
	service_urb_regs_t *urb = service_get_urb_registers();
	SET_PPOLICY(&urb->lsperiph1_subs_ppolicy, PP_ON);
	urb->top_clkgate |= SERVICE_TOP_CLK_GATE_LSP1;
}

void service_enable_lsp0(void)
{
	service_urb_regs_t *urb = service_get_urb_registers();
	SET_PPOLICY(&urb->lsperiph0_subs_ppolicy, PP_ON);
	urb->top_clkgate |= SERVICE_TOP_CLK_GATE_LSP0;
}

void service_enable_sdr(void)
{
	service_urb_regs_t *urb = service_get_urb_registers();
	urb->top_clkgate |= SERVICE_TOP_CLK_GATE_SDR;
	uint32_t bp_mask =
		(BIT(TOP_UCG1_CHANNEL_AXI_SLOW_COMM) | BIT(TOP_UCG1_CHANNEL_AXI_FAST_COMM) |
	         BIT(TOP_UCG1_CHANNEL_DDR_SDR_DSP) | BIT(TOP_UCG1_CHANNEL_DDR_SDR_PICE));

	ucg_regs_t *interconnect_ucg1 = ucg_get_registers(UCG_SUBSYS_TOP, 1);
	ucg_enable_bp(interconnect_ucg1, bp_mask);

	SET_PPOLICY(&urb->sdr_ppolicy, PP_ON);

	ucg_sync_and_disable_bp(interconnect_ucg1, bp_mask, bp_mask);
}

service_urb_regs_t *service_get_urb_registers(void)
{
	return (service_urb_regs_t *)BASE_ADDR_SERVICE_URB;
}

int service_get_apb_clock(uint32_t *apb_freq)
{
	int ret;
	uint32_t freq_div = 0;

	if (!apb_freq)
		return -ENULL;

	ucg_regs_t *ucg = ucg_get_registers(UCG_SUBSYS_SERV, 0);
	service_urb_regs_t *serv_urb = service_get_urb_registers();

	pll_cfg_t pll_cfg;
	pll_cfg.inp_freq = MCOM03_XTI_CLK_HZ;
	pll_cfg.out_freq = 0;

	ret = pll_get_freq((pll_cfg_reg_t *)(&serv_urb->service_subs_pllcnfg), &pll_cfg);
	if (ret)
		return ret;

	ret = ucg_get_divider(ucg, SERVICE_UCG1_CHANNEL_CLK_APB, &freq_div);
	if (ret)
		return ret;

	*apb_freq = pll_cfg.out_freq / freq_div;

	return 0;
}

int service_set_clock(uint32_t ch_mask, uint32_t sync_mask)
{
	int ret;

	ucg_regs_t *serv_ucg = ucg_get_registers(UCG_SUBSYS_SERV, 0);
	service_urb_regs_t *serv_urb = service_get_urb_registers();

	// Enable Service Sub UCG1 bypass
	ret = ucg_enable_bp(serv_ucg, ch_mask);
	if (ret)
		return ret;

	/* Setup PLL to 594 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 131;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 5;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)(&serv_urb->service_subs_pllcnfg), &pll_cfg,
	                          1000);
	if (ret)
		return ret;

	// Check the divisibility of CORE ucg channel frequency by APB ucg channel frequency
	if (SERV_UCG_APB_CHANNEL_DIV % SERV_UCG_CORE_CHANNEL_DIV)
		return -EINVALIDPARAM;

	// Set dividers
	for (int i = 0; i < ARRAY_SIZE(serv_ucg_channels); i++) {
		ret = ucg_set_divider(serv_ucg, serv_ucg_channels[i].chan_id,
		                      serv_ucg_channels[i].div, 1000);
		if (ret)
			return ret;
	}

	// Sync and disable Service Sub UCG1 bypass
	ret = ucg_sync_and_disable_bp(serv_ucg, ch_mask, sync_mask);
	if (ret)
		return ret;

	return 0;
}
