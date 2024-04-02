// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <mcom03.h>
#include "ucg.h"

ucg_regs_t *ucg_get_service_registers(uint32_t ucg_id)
{
	(void)ucg_id;
	return (ucg_regs_t *)(BASE_ADDR_SERVICE_UCG1);
}

ucg_regs_t *ucg_get_cpu_subs_registers(uint32_t ucg_id)
{
	(void)ucg_id;
	return (ucg_regs_t *)(BASE_ADDR_CPU_SUBS_UCG);
}

ucg_regs_t *ucg_get_sdr_registers(uint32_t ucg_id)
{
	return (ucg_regs_t *)(BASE_ADDR_SDR_UCG0);
}

ucg_regs_t *ucg_get_ls_periph0_registers(uint32_t ucg_id)
{
	(void)ucg_id;
	return (ucg_regs_t *)(BASE_ADDR_LS0_UCG2_BASE);
}

ucg_regs_t *ucg_get_ls_periph1_registers(uint32_t ucg_id)
{
	(void)ucg_id;
	return (ucg_regs_t *)(BASE_ADDR_LS1_UCG_BASE);
}

ucg_regs_t *ucg_get_hs_periph_registers(uint32_t ucg_id)
{
	if (ucg_id < 0 || ucg_id > UCG_HS_PERIPH_ID_MAX)
		return 0;

	return (ucg_regs_t *)(BASE_ADDR_HS_UCG0 + (0x10000UL * ucg_id));
}

ucg_regs_t *ucg_get_top_registers(uint32_t ucg_id)
{
	if (ucg_id < 0 || ucg_id > UCG_TOP_SSYS_ID_MAX)
		return 0;

	return (ucg_regs_t *)(BASE_ADDR_TOP_UCG0_BASE + (0x1000UL * ucg_id));
}

ucg_regs_t *ucg_get_ddr_registers(uint32_t ucg_id)
{
	if (ucg_id < 0 || ucg_id > UCG_DDR_SSYS_ID_MAX)
		return 0;

	return (ucg_regs_t *)(BASE_ADDR_DDR_SYS_UCG0 + (0x10000UL * ucg_id));
}

void ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask)
{
	unsigned int reg_val = ucg->UCG_BP_CTR_REG;
	reg_val |= ch_mask;
	ucg->UCG_BP_CTR_REG = reg_val;
}

mcom_err_t ucg_set_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t div, bool enable,
                           uint32_t timeout)
{
	if (ucg == NULL) {
		return MCOM03_ERROR_NULL;
	}

	if (ucg_id > UCG_CTR_REG_CH_ID_MAX) {
		return MCOM03_ERROR_INVALID_PARAM;
	}

	unsigned int ucg_ctr_reg;
	unsigned int bp_on = 0;
	unsigned int is_timeout = (timeout) ? (1) : (0);

	ucg_ctr_reg = ucg->UCG_CTR_REG[ucg_id];

	/* Disable CLK */
	if (((ucg_ctr_reg & UCG_CTR_REG_CLK_EN) != 0) &&
	    ((ucg_ctr_reg & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
	     UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_RUN))) {
		/* Enable Bypass */
		if ((ucg->UCG_BP_CTR_REG & (1 << ucg_id)) == 0) {
			ucg->UCG_BP_CTR_REG |= (1 << ucg_id);
			bp_on = 1;
		}

		ucg_ctr_reg &= ~(UCG_CTR_REG_CLK_EN);
		ucg_ctr_reg &= ~(UCG_CTR_REG_LPI_EN);
		ucg->UCG_CTR_REG[ucg_id] = ucg_ctr_reg;

		for (;;) {
			if ((ucg->UCG_CTR_REG[ucg_id] & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
			    UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_STOPPED)) {
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
	}

	/* Set Divider */
	if ((ucg_ctr_reg & UCG_CTR_REG_DIV_COEFF_Msk) != UCG_CTR_REG_DIV_COEFF(div)) {
		ucg_ctr_reg = UCG_CTR_REG_DIV_COEFF(div);
		ucg->UCG_CTR_REG[ucg_id] = ucg_ctr_reg;

		for (;;) {
			if ((ucg->UCG_CTR_REG[ucg_id] & UCG_CTR_REG_DIV_LOCK) ==
			    UCG_CTR_REG_DIV_LOCK) {
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
	}

	/* Enable CLK */
	if (enable == true) {
		ucg_ctr_reg |= (UCG_CTR_REG_CLK_EN);
		ucg_ctr_reg &= ~(UCG_CTR_REG_LPI_EN);
		ucg->UCG_CTR_REG[ucg_id] = ucg_ctr_reg;

		for (;;) {
			if ((ucg->UCG_CTR_REG[ucg_id] & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
			    UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_RUN)) {
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
	} else {
		ucg_ctr_reg &= ~(UCG_CTR_REG_CLK_EN);
		ucg_ctr_reg &= ~(UCG_CTR_REG_LPI_EN);
		ucg->UCG_CTR_REG[ucg_id] = ucg_ctr_reg;

		for (;;) {
			if ((ucg->UCG_CTR_REG[ucg_id] & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
			    UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_STOPPED)) {
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
	}

	if (bp_on) {
		/* Disable Bypass if it was enabled by this function */
		ucg->UCG_BP_CTR_REG &= ~(1 << ucg_id);
	}
	return MCOM03_SUCCESS;
}

void ucg_sync_and_disable_bp(ucg_regs_t *ucg, uint32_t ch_mask)
{
	unsigned int reg_val;

	/* Sync */
	reg_val = ucg->UCG_SYNC_CLK_REG;
	reg_val |= ch_mask;
	ucg->UCG_SYNC_CLK_REG = reg_val;

	/* Disable Bypass */
	reg_val = ucg->UCG_BP_CTR_REG;
	reg_val &= ~ch_mask;
	ucg->UCG_BP_CTR_REG = reg_val;
}

mcom_err_t ucg_get_state(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div, bool *enable)
{
	if (ucg == NULL)
		return MCOM03_ERROR_NULL;

	if (ucg_id > UCG_CTR_REG_CH_ID_MAX)
		return MCOM03_ERROR_INVALID_PARAM;

	if (div == NULL)
		return MCOM03_ERROR_NULL;

	if (enable == NULL)
		return MCOM03_ERROR_NULL;

	unsigned int ucg_ctr_reg = ucg->UCG_CTR_REG[ucg_id];

	/* Get State Enable or Disable */
	if ((((ucg_ctr_reg & UCG_CTR_REG_CLK_EN) >> UCG_CTR_REG_CLK_EN_Pos) != 0) &&
	    ((ucg_ctr_reg & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
	     UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_RUN))) {
		*enable = true;
	}

	if ((((ucg_ctr_reg & UCG_CTR_REG_CLK_EN) >> UCG_CTR_REG_CLK_EN_Pos) == 0) &&
	    (ucg->UCG_CTR_REG[ucg_id] & UCG_CTR_REG_Q_FSM_STATE_Msk) ==
	            UCG_CTR_REG_Q_FSM_STATE(UCG_Q_FSM_STATE_STOPPED)) {
		*enable = false;
	}

	/* Get Divider */
	*div = ((ucg_ctr_reg & UCG_CTR_REG_DIV_COEFF_Msk) >> UCG_CTR_REG_DIV_COEFF_Pos);

	return MCOM03_SUCCESS;
}

mcom_err_t ucg_get_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div)
{
	if (ucg == NULL) {
		return MCOM03_ERROR_NULL;
	}

	if (ucg_id > UCG_CTR_REG_CH_ID_MAX) {
		return MCOM03_ERROR_INVALID_PARAM;
	}

	unsigned int ucg_ctr_reg;
	ucg_ctr_reg = ucg->UCG_CTR_REG[ucg_id];

	/* Get Divider */
	*div = (ucg_ctr_reg & UCG_CTR_REG_DIV_COEFF_Msk) >> UCG_CTR_REG_DIV_COEFF_Pos;
	if (*div == 0)
		*div += 1;
	return MCOM03_SUCCESS;
}
