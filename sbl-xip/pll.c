// Copyright 2020-2021 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "pll.h"

int pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t max_retries)
{
	if ((pll_reg == NULL) || (pll_cfg == NULL)) {
		return -1;
	}

	bool is_timeout = (max_retries) ? true : false;
	uint32_t reg_value = 0;

	reg_value = FIELD_PREP(PLL_CFG_SEL, 1) | FIELD_PREP(PLL_CFG_MAN, 1) |
		    FIELD_PREP(PLL_CFG_OD_MAN, pll_cfg->od_value) |
		    FIELD_PREP(PLL_CFG_NF_MAN, pll_cfg->nf_value) |
		    FIELD_PREP(PLL_CFG_NR_MAN, pll_cfg->nr_value);
	*pll_reg = reg_value;

	for (;;) {
		if (FIELD_GET(PLL_CFG_LOCK, *pll_reg) == 1) {
			break;
		}
		if (is_timeout) {
			max_retries--;
			if (max_retries == 0) {
				return -1;
			}
		}
		__asm__ volatile("nop");
	}

	return 0;
}

int pll_get_freq_mult(pll_cfg_reg_t *pll_reg, uint32_t *freq_mult)
{
	if ((pll_reg == NULL) || (freq_mult == NULL)) {
		return -1;
	}

	if (FIELD_GET(PLL_CFG_MAN, *pll_reg) == 1) {
		uint32_t nf_value = FIELD_GET(PLL_CFG_NF_MAN, *pll_reg);
		uint32_t nr_value = FIELD_GET(PLL_CFG_NR_MAN, *pll_reg);
		uint32_t od_value = FIELD_GET(PLL_CFG_OD_MAN, *pll_reg);
		*freq_mult = ((nf_value + 1) / (nr_value + 1) / (od_value + 1));
	} else {
		*freq_mult = FIELD_GET(PLL_CFG_SEL, *pll_reg);
		if (*freq_mult >= 0x73) {
			*freq_mult = 116;
		} else {
			*freq_mult += 1;
		}
	}
	return 0;
}
