// Copyright 2020-2021 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "ucg.h"

int ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask)
{
	if (ucg == NULL)
		return -1;

	if (ch_mask == 0)
		return -1;

	if ((31 - __builtin_clz(ch_mask)) > UCG_CTR_REG_CH_ID_MAX)
		return -1;

	unsigned int id = 0;
	unsigned int val = ucg->UCG_BP_CTR_REG;

	for (; ch_mask != 0; ch_mask >>= 1U) {
		if ((ch_mask & 0x1) && (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg->UCG_CTR_REG[id]) ==
					UCG_Q_FSM_STATE_RUN)) {
			val |= BIT(id);
		}
		id++;
	}

	ucg->UCG_BP_CTR_REG = val;

	return 0;
}

int ucg_set_divider(ucg_regs_t *ucg, uint32_t ch, uint32_t div, uint32_t max_retries)
{
	if (ucg == NULL)
		return -1;

	if (ch > UCG_CTR_REG_CH_ID_MAX)
		return -1;

	unsigned int is_timeout = (max_retries) ? (1) : (0);

	/* Disable CLK */
	if ((FIELD_GET(UCG_CTR_REG_CLK_EN, ucg->UCG_CTR_REG[ch]) != 0) &&
	    (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg->UCG_CTR_REG[ch]) == UCG_Q_FSM_STATE_RUN)) {
		unsigned int ucg_ctr_reg = ucg->UCG_CTR_REG[ch];
		ucg_ctr_reg &= ~(UCG_CTR_REG_CLK_EN);
		ucg_ctr_reg &= ~(UCG_CTR_REG_LPI_EN);
		ucg->UCG_CTR_REG[ch] = ucg_ctr_reg;

		for (;;) {
			if (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg->UCG_CTR_REG[ch]) ==
			    UCG_Q_FSM_STATE_STOPPED)
				break;

			if (max_retries) {
				max_retries--;
				if (max_retries == 0)
					return -1;
			}
			__asm__ volatile("nop");
		}
	}

	/* Set Divider */
	if (FIELD_GET(UCG_CTR_REG_DIV_COEFF, ucg->UCG_CTR_REG[ch]) != div) {
		ucg->UCG_CTR_REG[ch] = FIELD_PREP(UCG_CTR_REG_DIV_COEFF, div);

		for (;;) {
			if (FIELD_GET(UCG_CTR_REG_DIV_LOCK, ucg->UCG_CTR_REG[ch]) == 1)
				break;

			if (is_timeout) {
				max_retries--;
				if (max_retries == 0)
					return -1;
			}
			__asm__ volatile("nop");
		}
	}

	/* Enable CLK */
	unsigned int ucg_ctr_reg = ucg->UCG_CTR_REG[ch];
	ucg_ctr_reg |= (UCG_CTR_REG_CLK_EN);
	ucg_ctr_reg &= ~(UCG_CTR_REG_LPI_EN);
	ucg->UCG_CTR_REG[ch] = ucg_ctr_reg;

	for (;;) {
		if (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg->UCG_CTR_REG[ch]) == UCG_Q_FSM_STATE_RUN)
			break;

		if (is_timeout) {
			max_retries--;
			if (max_retries == 0)
				return -1;
		}
		__asm__ volatile("nop");
	}

	return 0;
}

int ucg_sync_and_disable_bp(ucg_regs_t *ucg, uint32_t ch_mask)
{
	if (ucg == NULL)
		return -1;

	if (ch_mask == 0)
		return -1;

	if ((31 - __builtin_clz(ch_mask)) > UCG_CTR_REG_CH_ID_MAX)
		return -1;

	unsigned int id = 0;
	unsigned int bp_val = ucg->UCG_BP_CTR_REG;
	unsigned int sync_val = ucg->UCG_SYNC_CLK_REG;

	for (; ch_mask != 0; ch_mask >>= 1U) {
		if ((ch_mask & 0x1) && (FIELD_GET(BIT(id), bp_val) == 0x1)) {
			sync_val |= BIT(id);
			bp_val &= ~BIT(id);
		}
		id++;
	}

	ucg->UCG_SYNC_CLK_REG = sync_val;
	ucg->UCG_BP_CTR_REG = bp_val;

	return 0;
}

int ucg_get_state(ucg_regs_t *ucg, uint32_t ch, uint32_t *div, bool *enable)
{
	if (ucg == NULL)
		return -1;

	if (ch > UCG_CTR_REG_CH_ID_MAX)
		return -1;

	if (div == NULL)
		return -1;

	if (enable == NULL)
		return -1;

	unsigned int ucg_ctr_reg = ucg->UCG_CTR_REG[ch];

	/* Get State Enable or Disable */
	if ((FIELD_GET(UCG_CTR_REG_CLK_EN, ucg_ctr_reg) != 0) &&
	    (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg_ctr_reg) == UCG_Q_FSM_STATE_RUN))
		*enable = true;

	if ((FIELD_GET(UCG_CTR_REG_CLK_EN, ucg_ctr_reg) == 0) &&
	    (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg_ctr_reg) == UCG_Q_FSM_STATE_STOPPED))
		*enable = false;

	/* Get Divider */
	*div = FIELD_GET(UCG_CTR_REG_DIV_COEFF, ucg_ctr_reg);

	return 0;
}
