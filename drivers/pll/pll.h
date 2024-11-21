// SPDX-License-Identifier: MIT
// Copyright 2020-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/utils-def.h>

#define PLL_CFG_SEL    GENMASK(7, 0)
#define PLL_CFG_MAN    BIT(9)
#define PLL_CFG_OD_MAN GENMASK(13, 10)
#define PLL_CFG_NF_MAN GENMASK(26, 14)
#define PLL_CFG_NR_MAN GENMASK(30, 27)
#define PLL_CFG_LOCK   BIT(31)
#define PLL_CFG_MASK   BIT(8)

typedef struct {
	volatile uint8_t od_value;
	volatile uint16_t nf_value;
	volatile uint8_t nr_value;
	volatile uint32_t inp_freq;
	volatile uint32_t out_freq;
} pll_cfg_t;

typedef uint32_t pll_cfg_reg_t;

/**
 * @brief The function sets PLL setting (manual mode).
 *        The sel and man fields are always equal to 1.
 *        The frequency is set by the fields nf, nr and od.
 *        Equation:               in_freq * (nf_val + 1)
 *                 out_freq = -----------------------------
 *                             (nr_val + 1) * (od_val + 1)
 *
 * @param pll_reg     - Pointer to register address
 * @param pll_cfg     - Pointer to struct with PLL settings (Value OD, Value NF,
 *                      Value NR, input and output freq)
 * @param max_retries - Max number of tries. If max_retries == 0,
 *                      the function will be blocked in infinity loop.
 *
 * @return  0        - Success,
 *         -ENULL    - pll_reg or pll_cfg params are not provided (NULL pointers),
 *         -ETIMEOUT - Operation is timed out
 */
int pll_set_manual_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg, uint32_t max_retries);

/**
 * @brief The function gets PLL frequency.
 *        If the frequency is set by the multiplier sel then output freq is calculated
 *        according to equation:
 *                  if freq_mult = 0x01 then out_freq = input_freq * 2
 *                  if freq_mult = 0x02 then out_freq = input_freq * 3
 *                  if freq_mult = 0x03 then out_freq = input_freq * 4
 *                  ...
 *                  if freq_mult = 0x73 then out_freq = input_freq * 116
 *                  if freq_mult = 0x74 then out_freq = input_freq * 116
 *                  ...
 *                  if freq_mult = 0xFF then out_freq = input_freq * 116
 *
 *        If the frequency is set by the fields nf, nr and od then output freq is calculated
 *        according to equation:
 *                               input_freq * (nf_val + 1)
 *                 out_freq = ------------------------------
 *                              (nr_val + 1) * (od_val + 1)
 *
 * @param pll_reg - Pointer to register address
 * @param pll_cfg - Pointer to struct with PLL settings (Value OD, Value NF,
 *                  Value NR, input and output freq)
 *
 * @return  0     - Success,
 *         -ENULL - pll_reg or freq_mult params are not provided (NULL pointers)
 */
int pll_get_freq(pll_cfg_reg_t *pll_reg, pll_cfg_t *pll_cfg);
