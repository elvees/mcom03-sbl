// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <drivers/pll/pll.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>

#include "cpu.h"

// CPU Subsystem PLL output frequency is 1161 MHz, assuming that XTI = 27 MHz
static struct ucg_channel cpu_ucg_core_channel = { 0, CPU_UCG_CHANNEL_CLK_CORE, 1 }; // 1161 MHz

// It is not permitted to change the coefficients in the table below
static struct ucg_channel cpu_ucg_fixed_channels[] = {
	{ 0, CPU_UCG_CHANNEL_CLK_SYS, 4 }, // 290.25 MHz
	{ 0, CPU_UCG_CHANNEL_CLK_DBUS, 2 }, // 580.5 MHz
};

static cpu_urb_regs_t *cpu_get_urb_registers()
{
	return (cpu_urb_regs_t *)BASE_ADDR_CPU_URB;
}

int cpu_set_clock(void)
{
	int ret;

	service_enable_arm_cpu();

	ucg_regs_t *ucg = ucg_get_registers(UCG_SUBSYS_CPU, 0);
	cpu_urb_regs_t *urb = cpu_get_urb_registers();

	// Enable CPU Sub UCG bypass
	ret = ucg_enable_bp(ucg, CPU_UCG_ALL_CH_MASK);
	if (ret)
		return ret;

	/**
	 * Setup PLL to 1161 MHz, assuming that XTI = 27 MHz. Use NR = 0 to minimize PLL output
	 * jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 85;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 1;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)(&urb->pllcnfg), &pll_cfg, 1000);
	if (ret)
		return ret;

	// Set divider for CLK_CORE channel
	ret = ucg_set_divider(ucg, cpu_ucg_core_channel.chan_id, cpu_ucg_core_channel.div, 1000);
	if (ret)
		return ret;

	// Set dividers for the remaining channels relying on the CLK_CORE divider
	for (int i = 0; i < ARRAY_SIZE(cpu_ucg_fixed_channels); i++) {
		ret = ucg_set_divider(ucg, cpu_ucg_fixed_channels[i].chan_id,
		                      cpu_ucg_core_channel.div * cpu_ucg_fixed_channels[i].div,
		                      1000);
		if (ret)
			return ret;
	}

	// Sync and disable CPU UCG bypass
	ret = ucg_sync_and_disable_bp(ucg, CPU_UCG_ALL_CH_MASK, CPU_UCG_ALL_CH_MASK);
	if (ret)
		return ret;

	return 0;
}

int cpu_start_arm0_core(const unsigned long long int entrypoint)
{
	cpu_urb_regs_t *cpu = cpu_get_urb_registers();
	cpu->cpu_startcfg = 0xF; // Aarch64 En

	SET_PPOLICY(&cpu->a53sys_ppolicy, PP_ON);
	SET_PPOLICY(&cpu->cpu_core[0].a53cpu_ppolicy, PP_WARM_RST);
	SET_PPOLICY(&cpu->cpu_core[1].a53cpu_ppolicy, PP_WARM_RST);
	SET_PPOLICY(&cpu->cpu_core[2].a53cpu_ppolicy, PP_WARM_RST);
	SET_PPOLICY(&cpu->cpu_core[3].a53cpu_ppolicy, PP_WARM_RST);

	cpu->cpu_rvbaddr[0].cpu_rvbaddrl = (volatile unsigned int)entrypoint;
	cpu->cpu_rvbaddr[0].cpu_rvbaddrh = (volatile unsigned int)(entrypoint >> 32ULL);
	SET_PPOLICY(&cpu->cpu_core[0].a53cpu_ppolicy, PP_ON);

	return 0;
}

int cpu_disable_arm_core(void)
{
	cpu_urb_regs_t *cpu = cpu_get_urb_registers();
	SET_PPOLICY(&cpu->cpu_core[0].a53cpu_ppolicy, PP_OFF);
	SET_PPOLICY(&cpu->cpu_core[1].a53cpu_ppolicy, PP_OFF);
	SET_PPOLICY(&cpu->cpu_core[2].a53cpu_ppolicy, PP_OFF);
	SET_PPOLICY(&cpu->cpu_core[3].a53cpu_ppolicy, PP_OFF);
	cpu->cpu_rvbaddr[0].cpu_rvbaddrl = 0;
	cpu->cpu_rvbaddr[0].cpu_rvbaddrh = 0;
	SET_PPOLICY(&cpu->a53sys_ppolicy, PP_OFF);

	service_disable_arm_cpu();

	return 0;
}
