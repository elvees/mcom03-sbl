// SPDX-License-Identifier: MIT
// Copyright 2020-2021 RnD Center "ELVEES", JSC

#ifndef __PLL_H__
#define __PLL_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "bitops.h"

#define PLL_CFG_SEL    GENMASK(7, 0)
#define PLL_CFG_MAN    BIT(9)
#define PLL_CFG_OD_MAN GENMASK(13, 10)
#define PLL_CFG_NF_MAN GENMASK(26, 14)
#define PLL_CFG_NR_MAN GENMASK(30, 27)
#define PLL_CFG_LOCK   BIT(31)

#define PLL_DIAG_TEST   BIT(0)
#define PLL_DIAG_ENSAT  BIT(1)
#define PLL_DIAG_FASTEN BIT(2)
#define PLL_DIAG_RFSLIP BIT(3)
#define PLL_DIAG_FBSLIP BIT(4)

/**
 * @brief Structure of the pll config
 *
 */
typedef struct {
	volatile uint8_t od_value;
	volatile uint16_t nf_value;
	volatile uint8_t nr_value;
} pll_cfg_t;

typedef uint32_t pll_cfg_reg_t;

/**
 * @brief The function sets PLL setting (manual mode)
 *        The sel and man fields are always equal to 1.
 *        The frequency is set by the fields nf, nr and od.
 *        Equation:               in_freq * (nf_val + 1)
 *                 out_freq = -----------------------------
 *                             (nr_val + 1) * (od_val + 1)
 *
 * @param pll_reg      Pointer to register address
 * @param pll_cfg      Pointer to struct with PLL settings (Value OD, Value NF, Value NR)
 * @param max_retries  Max number of tries. If max_retries == 0,
 *                     the function will be blocked in infinity loop.
 *
 * @return (0) - Success, (-1) - Error
 */
int pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t max_retries);

/**
 * @brief The function gets PLL frequency multiplier
 *
 * @param pll_reg    Pointer to register address
 * @param freq_mult  The frequency multiplier
 *
 * @return (0) - Success, (-1) - Error
 */
int pll_get_freq_mult(pll_cfg_reg_t *pll_reg, uint32_t *freq_mult);

#endif /* __PLL_H__ */
