// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "pll.h"

mcom_err_t pll_set_mult_freq(pll_cfg_reg_t *pll_reg, uint8_t freq_mult, uint32_t timeout)
{
	if (pll_reg == NULL) {
		return MCOM03_ERROR_NULL;
	}

	bool is_timeout = (timeout) ? true : false;
	uint32_t reg_value = *pll_reg;

	reg_value &= ~(PLL_CFG_MASK);
	reg_value |= PLL_CFG_SEL(freq_mult);
	*pll_reg = reg_value;

	for (;;) {
		if ((*pll_reg & PLL_CFG_LOCK) == PLL_CFG_LOCK) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	return MCOM03_SUCCESS;
}

mcom_err_t pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t timeout)
{
	if ((pll_reg == NULL) || (pll_cfg == NULL)) {
		return MCOM03_ERROR_NULL;
	}

	bool is_timeout = (timeout) ? true : false;
	uint32_t reg_value = *pll_reg;

	reg_value &= ~(PLL_CFG_MASK);
	reg_value |= PLL_CFG_SEL(1);
	reg_value |= PLL_CFG_MAN;
	reg_value |= PLL_CFG_OD_MAN(pll_cfg->od_value);
	reg_value |= PLL_CFG_NF_MAN(pll_cfg->nf_value);
	reg_value |= PLL_CFG_NR_MAN(pll_cfg->nr_value);
	*pll_reg = reg_value;

	for (;;) {
		if ((*pll_reg & PLL_CFG_LOCK) == PLL_CFG_LOCK) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}

	uint32_t freq_coef =
		((pll_cfg->nf_value + 1) / (pll_cfg->nr_value + 1) / (pll_cfg->od_value + 1));
	pll_cfg->out_freq = pll_cfg->inp_freq * freq_coef;

	return MCOM03_SUCCESS;
}

mcom_err_t pll_get_freq_mult(pll_cfg_reg_t *pll_reg, uint32_t *freq_mult)
{
	if ((pll_reg == NULL) || (freq_mult == NULL)) {
		return MCOM03_ERROR_NULL;
	}

	uint32_t reg_value = *pll_reg;

	if ((reg_value & PLL_CFG_MAN) == PLL_CFG_MAN) {
		uint32_t nf_value = (reg_value & PLL_CFG_NF_MAN_Msk) >> PLL_CFG_NF_MAN_Pos;
		uint32_t nr_value = (reg_value & PLL_CFG_NR_MAN_Msk) >> PLL_CFG_NR_MAN_Pos;
		uint32_t od_value = (reg_value & PLL_CFG_OD_MAN_Msk) >> PLL_CFG_OD_MAN_Pos;
		*freq_mult = ((nf_value + 1) / (nr_value + 1) / (od_value + 1));
	} else {
		*freq_mult = (reg_value & PLL_CFG_SEL_Msk);
		if (*freq_mult >= 0x73) {
			*freq_mult = 116;
		} else {
			*freq_mult += 1;
		}
	}
	return MCOM03_SUCCESS;
}
