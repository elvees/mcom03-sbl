// Copyright 2020-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/mcom03-regs.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "ucg.h"

ucg_regs_t *ucg_get_registers(ucg_subsys_id subsys_id, uint32_t ucg_id)
{
	switch (subsys_id) {
	case UCG_SUBSYS_SERV:
		if (ucg_id > UCG_SERVICE_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_SERVICE_UCG1);
	case UCG_SUBSYS_CPU:
		if (ucg_id > UCG_CPU_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_CPU_UCG);
	case UCG_SUBSYS_TOP:
		if (ucg_id > UCG_TOP_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_TOP_UCG0_BASE + (0x1000UL * ucg_id));
	case UCG_SUBSYS_LSP0:
		if (ucg_id > UCG_LSP0_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_LSP0_UCG2_BASE);
	case UCG_SUBSYS_LSP1:
		if (ucg_id > UCG_LSP1_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_LSP1_UCG_BASE);
	case UCG_SUBSYS_HSP:
		if (ucg_id > UCG_HSP_ID_MAX)
			return NULL;
		return (ucg_regs_t *)(BASE_ADDR_HSP_UCG0 + (0x10000UL * ucg_id));
	default:
		return NULL;
	}
}

int ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask)
{
	if (ucg == NULL)
		return -ENULL;

	if (ch_mask == 0)
		return -EINVALIDPARAM;

	if ((31 - __builtin_clz(ch_mask)) > UCG_CTR_REG_CH_ID_MAX)
		return -EINVALIDPARAM;

	unsigned int id = 0;
	unsigned int val = ucg->UCG_BP_CTR_REG;

	for (; ch_mask != 0; ch_mask >>= 1U) {
		if ((ch_mask & 0x1) && (FIELD_GET(UCG_CTR_REG_Q_FSM_STATE, ucg->UCG_CTR_REG[id]) ==
		                        UCG_Q_FSM_STATE_RUN))
			val |= BIT(id);
		id++;
	}

	ucg->UCG_BP_CTR_REG = val;

	return 0;
}

int ucg_set_divider(ucg_regs_t *ucg, uint32_t ch, uint32_t div, uint32_t max_retries)
{
	if (ucg == NULL)
		return -ENULL;

	if (ch > UCG_CTR_REG_CH_ID_MAX)
		return -EINVALIDPARAM;

	unsigned int is_timeout = (max_retries) ? (1) : (0);

	/* Set Divider */
	if (FIELD_GET(UCG_CTR_REG_DIV_COEFF, ucg->UCG_CTR_REG[ch]) != div) {
		ucg->UCG_CTR_REG[ch] = FIELD_PREP(UCG_CTR_REG_DIV_COEFF, div);

		for (;;) {
			if (FIELD_GET(UCG_CTR_REG_DIV_LOCK, ucg->UCG_CTR_REG[ch]) == 1)
				break;

			if (is_timeout) {
				max_retries--;
				if (max_retries == 0)
					return -ETIMEOUT;
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
				return -ETIMEOUT;
		}
		__asm__ volatile("nop");
	}

	return 0;
}

int ucg_sync_and_disable_bp(ucg_regs_t *ucg, uint32_t ch_mask, uint32_t sync_mask)
{
	if (ucg == NULL)
		return -ENULL;

	if (ch_mask == 0)
		return -EINVALIDPARAM;

	if ((31 - __builtin_clz(ch_mask)) > UCG_CTR_REG_CH_ID_MAX)
		return -EINVALIDPARAM;

	unsigned int id = 0;
	unsigned int bp_val = ucg->UCG_BP_CTR_REG;

	for (; ch_mask != 0; ch_mask >>= 1U) {
		if ((ch_mask & 0x1) && (FIELD_GET(BIT(id), bp_val) == 0x1))
			bp_val &= ~BIT(id);
		id++;
	}

	if (sync_mask)
		ucg->UCG_SYNC_CLK_REG = sync_mask;
	ucg->UCG_BP_CTR_REG = bp_val;

	return 0;
}

int ucg_get_state(ucg_regs_t *ucg, uint32_t ch, uint32_t *div, bool *enable)
{
	if (ucg == NULL)
		return -ENULL;

	if (ch > UCG_CTR_REG_CH_ID_MAX)
		return -EINVALIDPARAM;

	if (div == NULL)
		return -ENULL;

	if (enable == NULL)
		return -ENULL;

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

int ucg_get_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div)
{
	if (ucg == NULL)
		return -ENULL;

	if (div == NULL)
		return -ENULL;

	if (ucg_id > UCG_CTR_REG_CH_ID_MAX)
		return -EINVALIDPARAM;

	unsigned int ucg_ctr_reg;
	ucg_ctr_reg = ucg->UCG_CTR_REG[ucg_id];

	/* Get Divider */
	*div = FIELD_GET(UCG_CTR_REG_DIV_COEFF, ucg_ctr_reg);
	if (*div == 0)
		*div += 1;
	return 0;
}
