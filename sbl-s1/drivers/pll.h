// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __PLL_H__
#define __PLL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <mcom03-errors.h>
#include <utils-def.h>

/* --------  PLL_CFG : (R/W 32) PLL Configuration Register -------- */
typedef uint32_t pll_cfg_reg_t;

typedef struct {
	volatile uint8_t od_value;
	volatile uint16_t nf_value;
	volatile uint8_t nr_value;
	volatile uint32_t inp_freq;
	volatile uint32_t out_freq;
} pll_cfg_t;

/* bit:  0.. 7  sel      - Frequency multiplier      */
/* bit:      8  Reserved                            */
/* bit:      9  man      - Manual mode               */
/* bit: 10..13  od_man   - Value OD for manual mode  */
/* bit: 14..26  nf_man   - Value NF for manual mode  */
/* bit: 27..30  nr_man   - Value NR for manual mode  */
/* bit:     31  lock     - Frequency capture signal  */
#define PLL_CFG_SEL_Pos       0
#define PLL_CFG_SEL_Msk       (U(0xFF) << PLL_CFG_SEL_Pos)
#define PLL_CFG_SEL(value)    (PLL_CFG_SEL_Msk & ((value) << PLL_CFG_SEL_Pos))
#define PLL_CFG_MAN_Pos       9
#define PLL_CFG_MAN           (U(0x1) << PLL_CFG_MAN_Pos)
#define PLL_CFG_OD_MAN_Pos    10
#define PLL_CFG_OD_MAN_Msk    (U(0xF) << PLL_CFG_OD_MAN_Pos)
#define PLL_CFG_OD_MAN(value) (PLL_CFG_OD_MAN_Msk & ((value) << PLL_CFG_OD_MAN_Pos))
#define PLL_CFG_NF_MAN_Pos    14
#define PLL_CFG_NF_MAN_Msk    (U(0x1FFF) << PLL_CFG_NF_MAN_Pos)
#define PLL_CFG_NF_MAN(value) (PLL_CFG_NF_MAN_Msk & ((value) << PLL_CFG_NF_MAN_Pos))
#define PLL_CFG_NR_MAN_Pos    27
#define PLL_CFG_NR_MAN_Msk    (U(0xF) << PLL_CFG_NR_MAN_Pos)
#define PLL_CFG_NR_MAN(value) (PLL_CFG_NR_MAN_Msk & ((value) << PLL_CFG_NR_MAN_Pos))
#define PLL_CFG_LOCK_Pos      31
#define PLL_CFG_LOCK          (U(0x1) << PLL_CFG_LOCK_Pos)
#define PLL_CFG_MASK          U(0xFFFFFEFF)

/* --------  PLL_DIAG : (R/W 32) PLL Diagnostic Register -------- */
typedef uint32_t pll_diag_reg_t;
/* bit:      0  test     - Bypass reference frequency         */
/* bit:      1  ensat    - Saturation enable                  */
/* bit:      2  fasten   - Fast lock enable                   */
/* bit:      3  rfslip   - Registration reference cycle slip  */
/* bit:      4  fbslip   - Registration feedback cycle slip   */
/* bit:  5..31  Reserved    */
#define PLL_DIAG_TEST_Pos   0
#define PLL_DIAG_TEST       (U(0x1) << PLL_DIAG_TEST_Pos)
#define PLL_DIAG_ENSAT_Pos  1
#define PLL_DIAG_ENSAT      (U(0x1) << PLL_DIAG_ENSAT_Pos)
#define PLL_DIAG_FASTEN_Pos 2
#define PLL_DIAG_FASTE      (U(0x1) << PLL_DIAG_FASTEN_Pos)
#define PLL_DIAG_RFSLIP_Pos 3
#define PLL_DIAG_RFSLIP     (U(0x1) << PLL_DIAG_RFSLIP_Pos)
#define PLL_DIAG_FBSLIP_Pos 4
#define PLL_DIAG_FBSLIP     (U(0x1) << PLL_DIAG_FBSLIP_Pos)
#define PLL_DIAG_MASK       U(0x0000001F)

/**
 * @brief PLL value setting function
 *        The man field are always equal to 0.
 *        The frequency is set by the multiplier sel
 *        Equation: if freq_mult = 0x01 then out_freq = in_freq * 2
 *                  if freq_mult = 0x02 then out_freq = in_freq * 3
 *                  if freq_mult = 0x03 then out_freq = in_freq * 4
 *                  ...
 *                  if freq_mult = 0x73 then out_freq = in_freq * 116
 *                  if freq_mult = 0x74 then out_freq = in_freq * 116
 *                  ...
 *                  if freq_mult = 0xFF then out_freq = in_freq * 116
 *
 * @param pll_reg   - Pointer to register address
 * @param freq_mult - Value of multiplier
 * @param timeout   - Timeout in system cycles
 *
 * @return Error value
 */
mcom_err_t pll_set_mult_freq(pll_cfg_reg_t *pll_reg, uint8_t freq_mult, uint32_t timeout);

/**
 * @brief PLL value setting function (manual mode)
 *        The sel and man fields are always equal to 1.
 *        The frequency is set by the fields nf, nr and od.
 *        Equation:               in_freq * (nf_val + 1)
 *                 out_freq = -----------------------------
 *                             (nr_val + 1) * (od_val + 1)
 *
 * @param pll_cfg - Pointer to register address
 * @param pll_cfg - Pointer to struct with settings PLL
 *                  (Value OD, Value NF, Value NR)
 * @param timeout - Timeout in system cycles
 *
 * @return Error value
 */
mcom_err_t pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t timeout);

/**
 * @brief PLL getting multiply frequency function
 *
 * @param pll_reg - Pointer to register address
 * @param freq_mult - The frequency multiply
 *
 * @return Error value
 */
mcom_err_t pll_get_freq_mult(pll_cfg_reg_t *pll_reg, uint32_t *freq_mult);

#endif /* __PLL_H__ */
