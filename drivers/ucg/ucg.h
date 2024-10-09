// SPDX-License-Identifier: MIT
// Copyright 2020-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <libs/utils-def.h>

#define UCG_CTR_REG_CH_ID_MAX 15

#define UCG_Q_FSM_STATE_STOPPED  0
#define UCG_Q_FSM_STATE_CLK_EN   1
#define UCG_Q_FSM_STATE_REQUEST  2
#define UCG_Q_FSM_STATE_DENIED   3
#define UCG_Q_FSM_STATE_EXIT     4
#define UCG_Q_FSM_STATE_RUN      6
#define UCG_Q_FSM_STATE_CONTINUE 7

// Valid UCG subsystem IDs
#define UCG_SERVICE_ID_MAX 0
#define UCG_CPU_ID_MAX     0
#define UCG_TOP_ID_MAX     1
#define UCG_LSP0_ID_MAX    0
#define UCG_LSP1_ID_MAX    0
#define UCG_HSP_ID_MAX     3

typedef enum {
	UCG_SUBSYS_SERV,
	UCG_SUBSYS_CPU,
	UCG_SUBSYS_TOP,
	UCG_SUBSYS_LSP0,
	UCG_SUBSYS_LSP1,
	UCG_SUBSYS_HSP,
} ucg_subsys_id;

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

struct ucg_channel {
	int ucg_id;
	int chan_id;
	int div;
};

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of required subsystem
 *
 * @param subsys_id   Subsystem identifier
 * @param ucg_id      UCG number
 *
 * @return Pointer to the UCG register address
 *         of required subsystem
 */
ucg_regs_t *ucg_get_registers(ucg_subsys_id subsys_id, uint32_t ucg_id);

/**
 * @brief      Bypass enable function
 *
 * @param      ucg      Pointer to UCG register address
 * @param[in]  ch_mask  UCG channel mask
 *
 * @return  0             - Success,
 *         -ENULL         - ucg param is not provided (NULL pointers),
 *         -EINVALIDPARAM - ch_mask param is equal to 0 or installed bit positions
 *                          in it are bigger than the maximum ucg channel id
 */
int ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask);

/**
 * @brief Clock divider value setting function
 *
 * @param ucg         Pointer to UCG register address
 * @param ch          UCG channel number
 * @param div         Value of divider
 * @param max_retries Max number of tries. If max_retries == 0,
 *                    the function will be blocked in infinity loop.
 *
 * @return  0             - Success,
 *         -ENULL         - ucg param is not provided (NULL pointers),
 *         -ETIMEOUT      - Operation is timed out,
 *         -EINVALIDPARAM - ch param is bigger than the maximux ucg channel id
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
 * @return  0             - Success,
 *         -ENULL         - ucg param is not provided (NULL pointers),
 *         -EINVALIDPARAM - ch_mask param is equal to 0 or installed bit positions
 *                          in it are bigger than the maximum ucg channel id
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
 * @return  0             - Success,
 *         -ENULL         - ucg or div or enable params are not provided (NULL pointers),
 *         -EINVALIDPARAM - ch param is bigger than the maximum ucg channel id
 */
int ucg_get_state(ucg_regs_t *ucg, uint32_t ch, uint32_t *div, bool *enable);

/**
 * @brief Clock divider value getting function
 *
 * @param ucg      Pointer to UCG register address
 * @param ucg_id   UCG channel number
 * @param div      Value of divider
 *
 * @return  0             - Success,
 *         -ENULL         - ucg or div params are not provided (NULL pointers),
 *         -EINVALIDPARAM - ucg_id param is bigger than the maximum ucg channel id
 */
int ucg_get_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div);

/** @} */
