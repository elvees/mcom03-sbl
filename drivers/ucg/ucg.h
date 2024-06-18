// SPDX-License-Identifier: MIT
// Copyright 2020-2024 RnD Center "ELVEES", JSC

#ifndef __UCG_H__
#define __UCG_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <libs/utils-def.h>

#define UCG_CTR_REG_CH_ID_MAX 15

#define UCG_Q_FSM_STATE_STOPPED  0
#define UCG_Q_FSM_STATE_CLK_EN   1
#define UCG_Q_FSM_STATE_REQUEST  2
#define UCG_Q_FSM_STATE_DENIED   3
#define UCG_Q_FSM_STATE_EXIT     4
#define UCG_Q_FSM_STATE_RUN      6
#define UCG_Q_FSM_STATE_CONTINUE 7

typedef struct {
	volatile uint32_t UCG_CTR_REG[16];
	volatile uint32_t UCG_BP_CTR_REG;
	volatile uint32_t UCG_SYNC_CLK_REG;
} ucg_regs_t;

#define UCG_CTR_REG_LPI_EN      BIT(0)
#define UCG_CTR_REG_CLK_EN      BIT(1)
#define UCG_CTR_REG_Q_FSM_STATE GENMASK(9, 7)
#define UCG_CTR_REG_DIV_COEFF   GENMASK(29, 10)
#define UCG_CTR_REG_DIV_LOCK    BIT(30)
/**
 * @brief      Bypass enable function
 *
 * @param      ucg      Pointer to UCG register address
 * @param[in]  ch_mask  UCG channel mask
 *
 * @return (0) - Success, (-1) - Error
 */
int ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask);

/**
 * @brief Clock value setting function
 *
 * @param ucg         Pointer to UCG register address
 * @param ch          UCG channel number
 * @param div         Value of divider
 * @param max_retries Max number of tries. If max_retries == 0,
 *                    the function will be blocked in infinity loop.
 *
 * @return (0) - Success, (-1) - Error
 */
int ucg_set_divider(ucg_regs_t *ucg, uint32_t ch, uint32_t div, uint32_t max_retries);

/**
 * @brief Sync clocking enable and
 *        bypass disable function
 *
 * @param ucg         Pointer to UCG register address
 * @param ch_mask     UCG channel mask
 * @param sync_mask   UCG sync mask
 *
 * @return (0) - Success, (-1) - Error
 */
int ucg_sync_and_disable_bp(ucg_regs_t *ucg, uint32_t ch_mask, uint32_t sync_mask);

/**
 * @brief UCG channel state getting function
 *
 * @param ucg      Pointer to UCG register address
 * @param ch       UCG channel number
 * @param div      Value of divider
 * @param enable   Channel state: enable or disable
 *
 * @return (0) - Success, (-1) - Error
 */
int ucg_get_state(ucg_regs_t *ucg, uint32_t ch, uint32_t *div, bool *enable);

/** @} */
#endif /* __UCG_H__ */
