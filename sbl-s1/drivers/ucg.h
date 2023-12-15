// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __UCG_H__
#define __UCG_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <utils-def.h>
#include <mcom03-errors.h>

/**
 * @brief Maximum number of UCG blocks in subsystems
 *
 */
#define UCG_HS_PERIPH_ID_MAX  U(3)
#define UCG_TOP_SSYS_ID_MAX   U(1)
#define UCG_DDR_SSYS_ID_MAX   U(1)
#define UCG_CTR_REG_CH_ID_MAX U(15)

/**
 * @brief The states of a finite state machine
 *        of a synchronisation system
 *
 */
#define UCG_Q_FSM_STATE_STOPPED	 U(0x00)
#define UCG_Q_FSM_STATE_CLK_EN	 U(0x01)
#define UCG_Q_FSM_STATE_REQUEST	 U(0x02)
#define UCG_Q_FSM_STATE_DENIED	 U(0x03)
#define UCG_Q_FSM_STATE_EXIT	 U(0x04)
#define UCG_Q_FSM_STATE_RUN	 U(0x06)
#define UCG_Q_FSM_STATE_CONTINUE U(0x07)

/**
 * @brief Structure of the UCG registers
 *
 */
typedef struct {
	volatile uint32_t UCG_CTR_REG
		[16]; /**< \brief Offset: 0x000 (R/W 32) UCG Control n Register, n = 0..16 is number of channel,
                                                    0x40000001, if clk_en_dflt_i[0] = 0, 0x40000302, if clk_en_dflt_i[0] = 1 */
	volatile uint32_t
		UCG_BP_CTR_REG; /**< \brief Offset: 0x040 (R/W 32) UCG BYPASS Mode Register */
	volatile uint32_t
		UCG_SYNC_CLK_REG; /**< \brief Offset: 0x044 ( /W 32) Channel Synchronization Register */
} ucg_regs_t;

/* --------  UCG_CTR_REG : (R/W 32) UCG Control n Register, n = 0..16 is number of channel, -------- */
/*!< bit:      0  Low Power Enable               */
/*!< bit:      1  Channel Clock Enable           */
/*!< bit:  2.. 4  Channel Clock Enable Status    */
/*!< bit:      5  Reserved                       */
/*!< bit:      6  Signal Q_ACTIVE Control Enable */
/*!< bit:  7.. 9  Q_FSM Status                   */
/*!< bit: 10..29  Divide coefficient             */
/*!< bit:     30  Divider Ready Status           */
/*!< bit:     31  Reserved                       */
#define UCG_CTR_REG_OFFSET     0x000
#define UCG_CTR_REG_RESETVALUE U(0x00000000)

#define UCG_CTR_REG_LPI_EN_Pos	   0
#define UCG_CTR_REG_LPI_EN	   (U(0x1) << UCG_CTR_REG_LPI_EN_Pos)
#define UCG_CTR_REG_CLK_EN_Pos	   1
#define UCG_CTR_REG_CLK_EN	   (U(0x1) << UCG_CTR_REG_CLK_EN_Pos)
#define UCG_CTR_REG_CLK_EN_STS_Pos 2
#define UCG_CTR_REG_CLK_EN_STS_Msk (U(0x7) << UCG_CTR_REG_CLK_EN_STS_Pos)
#define UCG_CTR_REG_CLK_EN_STS(value) \
	(UCG_CTR_REG_CLK_EN_STS_Msk & ((value) << UCG_CTR_REG_CLK_EN_STS_Pos))
#define UCG_CTR_REG_QACTIVE_CTL_EN_Pos 6
#define UCG_CTR_REG_QACTIVE_CTL_EN     (U(0x1) << UCG_CTR_REG_QACTIVE_CTL_EN_Pos)
#define UCG_CTR_REG_Q_FSM_STATE_Pos    7
#define UCG_CTR_REG_Q_FSM_STATE_Msk    (U(0x7) << UCG_CTR_REG_Q_FSM_STATE_Pos)
#define UCG_CTR_REG_Q_FSM_STATE(value) \
	(UCG_CTR_REG_Q_FSM_STATE_Msk & ((value) << UCG_CTR_REG_Q_FSM_STATE_Pos))
#define UCG_CTR_REG_DIV_COEFF_Pos 10
#define UCG_CTR_REG_DIV_COEFF_Msk (U(0xFFFFF) << UCG_CTR_REG_DIV_COEFF_Pos)
#define UCG_CTR_REG_DIV_COEFF(value) \
	(UCG_CTR_REG_DIV_COEFF_Msk & ((value) << UCG_CTR_REG_DIV_COEFF_Pos))
#define UCG_CTR_REG_DIV_LOCK_Pos 30
#define UCG_CTR_REG_DIV_LOCK	 (U(0x1) << UCG_CTR_REG_DIV_LOCK_Pos)
#define UCG_CTR_REG_MASK	 U(0x7FFFFFDF)

/* --------  UCG_BP_CTR_REG : (R/W 32) UCG BYPASS Mode Register -------- */
/*!< bit:      0  Bypass Mode Channel  0 Enable */
/*!< bit:      1  Bypass Mode Channel  1 Enable */
/*!< bit:      2  Bypass Mode Channel  2 Enable */
/*!< bit:      3  Bypass Mode Channel  3 Enable */
/*!< bit:      4  Bypass Mode Channel  4 Enable */
/*!< bit:      5  Bypass Mode Channel  5 Enable */
/*!< bit:      6  Bypass Mode Channel  6 Enable */
/*!< bit:      7  Bypass Mode Channel  7 Enable */
/*!< bit:      8  Bypass Mode Channel  8 Enable */
/*!< bit:      9  Bypass Mode Channel  9 Enable */
/*!< bit:     10  Bypass Mode Channel 10 Enable */
/*!< bit:     11  Bypass Mode Channel 11 Enable */
/*!< bit:     12  Bypass Mode Channel 12 Enable */
/*!< bit:     13  Bypass Mode Channel 13 Enable */
/*!< bit:     14  Bypass Mode Channel 14 Enable */
/*!< bit:     15  Bypass Mode Channel 15 Enable */
/*!< bit: 16..31  Reserved                      */
#define UCG_BP_CTR_REG_OFFSET	  0x040
#define UCG_BP_CTR_REG_RESETVALUE U(0x00000000)

#define UCG_BP_CTR_REG_XTI_EN_CH0_Pos  0
#define UCG_BP_CTR_REG_XTI_EN_CH0      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH0_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH1_Pos  1
#define UCG_BP_CTR_REG_XTI_EN_CH1      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH1_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH2_Pos  2
#define UCG_BP_CTR_REG_XTI_EN_CH2      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH2_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH3_Pos  3
#define UCG_BP_CTR_REG_XTI_EN_CH3      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH3_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH4_Pos  4
#define UCG_BP_CTR_REG_XTI_EN_CH4      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH4_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH5_Pos  5
#define UCG_BP_CTR_REG_XTI_EN_CH5      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH5_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH6_Pos  6
#define UCG_BP_CTR_REG_XTI_EN_CH6      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH6_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH7_Pos  7
#define UCG_BP_CTR_REG_XTI_EN_CH7      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH7_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH8_Pos  8
#define UCG_BP_CTR_REG_XTI_EN_CH8      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH8_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH9_Pos  9
#define UCG_BP_CTR_REG_XTI_EN_CH9      (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH9_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH10_Pos 10
#define UCG_BP_CTR_REG_XTI_EN_CH10     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH10_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH11_Pos 11
#define UCG_BP_CTR_REG_XTI_EN_CH11     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH11_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH12_Pos 12
#define UCG_BP_CTR_REG_XTI_EN_CH12     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH12_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH13_Pos 13
#define UCG_BP_CTR_REG_XTI_EN_CH13     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH13_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH14_Pos 14
#define UCG_BP_CTR_REG_XTI_EN_CH14     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH14_Pos)
#define UCG_BP_CTR_REG_XTI_EN_CH15_Pos 15
#define UCG_BP_CTR_REG_XTI_EN_CH15     (U(0x1) << UCG_BP_CTR_REG_XTI_EN_CH15_Pos)
#define UCG_BP_CTR_REG_MASK	       U(0x0000FFFF)

/* --------  UCG_SYNC_CLK_REG : ( /W 32) Channel Synchronization Register -------- */
/*!< bit:      0  Request Sync CLK Channel  0 */
/*!< bit:      1  Request Sync CLK Channel  1 */
/*!< bit:      2  Request Sync CLK Channel  2 */
/*!< bit:      3  Request Sync CLK Channel  3 */
/*!< bit:      4  Request Sync CLK Channel  4 */
/*!< bit:      5  Request Sync CLK Channel  5 */
/*!< bit:      6  Request Sync CLK Channel  6 */
/*!< bit:      7  Request Sync CLK Channel  7 */
/*!< bit:      8  Request Sync CLK Channel  8 */
/*!< bit:      9  Request Sync CLK Channel  9 */
/*!< bit:     10  Request Sync CLK Channel 10 */
/*!< bit:     11  Request Sync CLK Channel 11 */
/*!< bit:     12  Request Sync CLK Channel 12 */
/*!< bit:     13  Request Sync CLK Channel 13 */
/*!< bit:     14  Request Sync CLK Channel 14 */
/*!< bit:     15  Request Sync CLK Channel 15 */
/*!< bit: 16..31  Reserved                    */
#define UCG_SYNC_CLK_REG_OFFSET	    0x044
#define UCG_SYNC_CLK_REG_RESETVALUE U(0x00000000)

#define UCG_SYNC_CLK_REG_SYNC_CLK_CH0_Pos  0
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH0	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH0_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH1_Pos  1
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH1	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH1_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH2_Pos  2
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH2	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH2_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH3_Pos  3
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH3	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH3_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH4_Pos  4
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH4	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH4_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH5_Pos  5
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH5	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH5_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH6_Pos  6
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH6	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH6_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH7_Pos  7
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH7	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH7_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH8_Pos  8
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH8	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH8_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH9_Pos  9
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH9	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH9_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH10_Pos 10
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH10	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH10_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH11_Pos 11
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH11	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH11_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH12_Pos 12
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH12	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH12_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH13_Pos 13
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH13	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH13_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH14_Pos 14
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH14	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH14_Pos)
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH15_Pos 15
#define UCG_SYNC_CLK_REG_SYNC_CLK_CH15	   (U(0x1) << UCG_SYNC_CLK_REG_SYNC_CLK_CH15_Pos)

/**
 * @name Processor subsystems
 *
 * @{
 */

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the Service subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the Service subsystem
 */
ucg_regs_t *ucg_get_service_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the CPU subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the CPU subsystem
 */
ucg_regs_t *ucg_get_cpu_subs_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the SDR subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the SDR subsystem
 */
ucg_regs_t *ucg_get_sdr_registers(uint32_t ucg_id);
/** @} */

/**
 * @name Peripheral device subsystems
 *
 * @{
 */

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the TOP subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the TOP subsystem
 */
ucg_regs_t *ucg_get_top_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the DDR subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the DDR subsystem
 */
ucg_regs_t *ucg_get_ddr_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the LSPERIPH0 subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the LSPERIPH0 subsystem
 */
ucg_regs_t *ucg_get_ls_periph0_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the LSPERIPH1 subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the LSPERIPH1 subsystem
 */
ucg_regs_t *ucg_get_ls_periph1_registers(uint32_t ucg_id);

/**
 * @brief The function returns a pointer
 *        to the UCG register address
 *        of the HSPERIPH subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the HSPERIPH subsystem
 */
ucg_regs_t *ucg_get_hs_periph_registers(uint32_t ucg_id);

/**
 * @brief      Bypass disable function
 *
 * @param      ucg      Pointer to UCG register address
 * @param[in]  ch_mask  UCG channel mask
 */
void ucg_enable_bp(ucg_regs_t *ucg, uint32_t ch_mask);

/**
 * @brief Clock value setting function
 *
 * @param ucg - Pointer to UCG register address
 * @param ucg_id - UCG channel number
 * @param div - Value of divider
 * @param enable - Enable/disable channel
 * @param timeout - Timeout in system cycles
 *
 * @return Error value
 */
mcom_err_t ucg_set_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t div, bool enable,
			   uint32_t timeout);

/**
 * @brief Sync clocking enable and
 *        bypass disable function
 *
 * @param ucg - Pointer to UCG register address
 * @param ch_mask - UCG channel mask
 */
void ucg_sync_and_disable_bp(ucg_regs_t *ucg, uint32_t ch_mask);

/**
 * @brief UCG channel state getting function
 *
 * @param ucg - Pointer to UCG register address
 * @param ucg_id - UCG channel number
 * @param div - Value of divider
 * @param enable - State channel Enable or disable
 *
 * @return     Error value
 */
mcom_err_t ucg_get_state(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div, bool *enable);

/**
 * @brief Clock value getting function
 *
 * @param ucg - Pointer to UCG register address
 * @param ucg_id - UCG channel number
 * @param div - Value of divider
 *
 * @return Error value
 */
mcom_err_t ucg_get_divider(ucg_regs_t *ucg, uint32_t ucg_id, uint32_t *div);

/** @} */
#endif /* __UCG_H__ */
