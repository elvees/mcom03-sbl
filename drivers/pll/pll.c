// Copyright 2020-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <libs/errors.h>
#include <libs/utils-def.h>

#include "pll.h"

int pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t max_retries)
{
	if ((pll_reg == NULL) || (pll_cfg == NULL))
		return -ENULL;

	bool is_timeout = (max_retries) ? true : false;
	uint32_t reg_value = 0;

	reg_value = FIELD_PREP(PLL_CFG_SEL, 1) | FIELD_PREP(PLL_CFG_MAN, 1) |
	            FIELD_PREP(PLL_CFG_OD_MAN, pll_cfg->od_value) |
	            FIELD_PREP(PLL_CFG_NF_MAN, pll_cfg->nf_value) |
	            FIELD_PREP(PLL_CFG_NR_MAN, pll_cfg->nr_value);
	*pll_reg = reg_value;

	for (;;) {
		if (FIELD_GET(PLL_CFG_LOCK, *pll_reg) == 1)
			break;
		if (is_timeout) {
			max_retries--;
			if (max_retries == 0)
				return -ETIMEOUT;
		}
		__asm__ volatile("nop");
	}

	return 0;
}

int pll_get_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg)
{
	if ((pll_reg == NULL) || (pll_cfg == NULL))
		return -ENULL;

	uint32_t freq_coef;

	if (FIELD_GET(PLL_CFG_MAN, *pll_reg) == 1) {
		pll_cfg->nf_value = FIELD_GET(PLL_CFG_NF_MAN, *pll_reg);
		pll_cfg->nr_value = FIELD_GET(PLL_CFG_NR_MAN, *pll_reg);
		pll_cfg->od_value = FIELD_GET(PLL_CFG_OD_MAN, *pll_reg);
		freq_coef = ((pll_cfg->nf_value + 1) / (pll_cfg->nr_value + 1) /
		             (pll_cfg->od_value + 1));
	} else {
		freq_coef = FIELD_GET(PLL_CFG_SEL, *pll_reg);
		if (freq_coef >= 0x73)
			freq_coef = 116;
		else
			freq_coef += 1;
	}

	pll_cfg->out_freq = pll_cfg->inp_freq * freq_coef;

	return 0;
}
