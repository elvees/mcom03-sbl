// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __LS1_URB_H__
#define __LS1_URB_H__

#include <utils-def-exp.h>
#include <mcom03.h>

/**
 * @brief Pin slew rate limitation
 *
 */
#define LS1_URB_PAD_CTR_SL_LO_RISE U(0x00)
#define LS1_URB_PAD_CTR_SL_HI_RISE U(0x03)

/**
 * @brief Maximum pin current
 *
 */
#define LS1_URB_PAD_CTR_CTL_Z_STATE	    U(0x00)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_2mA  U(0x01)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_4mA  U(0x03)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_6mA  U(0x07)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_8mA  U(0x0F)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_10mA U(0x1F)
#define LS1_URB_PAD_CTR_CTL_MAX_OUTPUT_12mA U(0x3F)

/**
 * @brief Maximum pin voltage
 *
 */
#define LS1_URB_PAD_1V8 U(0x01)
#define LS1_URB_PAD_3V3 U(0x00)

// clang-format off
/**
 * @brief Structure of the URB registers of the LSPERIPH1 subsystem
 *
 */
typedef struct {
    volatile unsigned int pll_cfg;                   /**< \brief Offset: 0x000 (R/W 32) PLL Configuration Register */
    volatile unsigned int pll_diag;                  /**< \brief Offset: 0x004 (R/W 32) PLL Diagnostic Register */
    volatile unsigned int i2s_ucg_rstn_ppolicy;      /**< \brief Offset: 0x008 ( /W 32) I2S UCG Mode Control Register */
    volatile unsigned int i2s_ucg_rstn_pstatus;      /**< \brief Offset: 0x00C (R/  32) I2S UCG Status Register */
    volatile unsigned int i2s_ucg_ref_clk;           /**< \brief Offset: 0x010 (R/W 32) I2S UCG Reference Frequency Source Selection Register */
    volatile unsigned int i2c_highspeed_pull_enable; /**< \brief Offset: 0x014 (R/W 32) Hardware Control Register of the SCL I2C Interface Pull-up Lines */
    volatile unsigned int gpio1_port_a_pad_ctr[8];   /**< \brief Offset: 0x020 (R/W 32) Port A Pin Control Register */
    volatile unsigned int gpio1_port_b_pad_ctr[8];   /**< \brief Offset: 0x040 (R/W 32) Port B Pin Control Register */
    volatile unsigned int gpio1_port_c_pad_ctr[8];   /**< \brief Offset: 0x060 (R/W 32) Port C Pin Control Register */
    volatile unsigned int gpio1_port_d_pad_ctr[8];   /**< \brief Offset: 0x080 (R/W 32) Port D Pin Control Register */
    volatile unsigned int gpio1_v18;                 /**< \brief Offset: 0x0A0 (R/W 32) Output Voltage Selection Register GPIO1 Pins */
} ls1_urb_regs_t;
// clang-format on

/* --------  LS1_URB_PLL_CFG : (R/W 32) PLL Configuration Register -------- */
/* bit:  0.. 7  sel         */
/* bit:      8  Reserved    */
/* bit:      9  man         */
/* bit: 10..13  od_man      */
/* bit: 14..26  nf_man      */
/* bit: 27..30  nr_man      */
/* bit:     31  lock        */
#define LS1_URB_PLL_CFG_OFFSET	   0x000
#define LS1_URB_PLL_CFG_RESETVALUE U(0x00000000)

#define LS1_URB_PLL_CFG_SEL_Pos	   0
#define LS1_URB_PLL_CFG_SEL_Msk	   (U(0xFF) << LS1_URB_PLL_CFG_SEL_Pos)
#define LS1_URB_PLL_CFG_SEL(value) (LS1_URB_PLL_CFG_SEL_Msk & ((value) << LS1_URB_PLL_CFG_SEL_Pos))
#define LS1_URB_PLL_CFG_MAN_Pos	   9
#define LS1_URB_PLL_CFG_MAN	   (U(0x1) << LS1_URB_PLL_CFG_MAN_Pos)
#define LS1_URB_PLL_CFG_OD_MAN_Pos 10
#define LS1_URB_PLL_CFG_OD_MAN_Msk (U(0xF) << LS1_URB_PLL_CFG_OD_MAN_Pos)
#define LS1_URB_PLL_CFG_OD_MAN(value) \
	(LS1_URB_PLL_CFG_OD_MAN_Msk & ((value) << LS1_URB_PLL_CFG_OD_MAN_Pos))
#define LS1_URB_PLL_CFG_NF_MAN_Pos 14
#define LS1_URB_PLL_CFG_NF_MAN_Msk (U(0x1FFF) << LS1_URB_PLL_CFG_NF_MAN_Pos)
#define LS1_URB_PLL_CFG_NF_MAN(value) \
	(LS1_URB_PLL_CFG_NF_MAN_Msk & ((value) << LS1_URB_PLL_CFG_NF_MAN_Pos))
#define LS1_URB_PLL_CFG_NR_MAN_Pos 27
#define LS1_URB_PLL_CFG_NR_MAN_Msk (U(0xF) << LS1_URB_PLL_CFG_NR_MAN_Pos)
#define LS1_URB_PLL_CFG_NR_MAN(value) \
	(LS1_URB_PLL_CFG_NR_MAN_Msk & ((value) << LS1_URB_PLL_CFG_NR_MAN_Pos))
#define LS1_URB_PLL_CFG_LOCK_Pos 31
#define LS1_URB_PLL_CFG_LOCK	 (U(0x1) << LS1_URB_PLL_CFG_LOCK_Pos)
#define LS1_URB_PLL_CFG_MASK	 U(0xFFFFFEFF)

/* --------  LS1_URB_PLL_DIAG : (R/W 32) PLL Diagnostic Register -------- */
/* bit:      0  test        */
/* bit:      1  ensat       */
/* bit:      2  fasten      */
/* bit:      3  rfslip      */
/* bit:      4  fbslip      */
/* bit:  5..31  Reserved    */
#define LS1_URB_PLL_DIAG_OFFSET	    0x004
#define LS1_URB_PLL_DIAG_RESETVALUE U(0x00000002)

#define LS1_URB_PLL_DIAG_TEST_Pos   0
#define LS1_URB_PLL_DIAG_TEST	    (U(0x1) << LS1_URB_PLL_DIAG_TEST_Pos)
#define LS1_URB_PLL_DIAG_ENSAT_Pos  1
#define LS1_URB_PLL_DIAG_ENSAT	    (U(0x1) << LS1_URB_PLL_DIAG_ENSAT_Pos)
#define LS1_URB_PLL_DIAG_FASTEN_Pos 2
#define LS1_URB_PLL_DIAG_FASTE	    (U(0x1) << LS1_URB_PLL_DIAG_FASTEN_Pos)
#define LS1_URB_PLL_DIAG_RFSLIP_Pos 3
#define LS1_URB_PLL_DIAG_RFSLIP	    (U(0x1) << LS1_URB_PLL_DIAG_RFSLIP_Pos)
#define LS1_URB_PLL_DIAG_FBSLIP_Pos 4
#define LS1_URB_PLL_DIAG_FBSLIP	    (U(0x1) << LS1_URB_PLL_DIAG_FBSLIP_Pos)
#define LS1_URB_PLL_DIAG_MASK	    U(0x0000001F)

/* --------  LS1_I2S_UCG_RSTN_PPOLICY : ( /W 32) I2S UCG Mode Control Register -------- */
/* bit:  0.. 4  policy      */
/* bit:  5..31  Reserved    */
#define LS1_URB_I2S_UCG_RSTN_PPOLICY_OFFSET	0x008
#define LS1_URB_I2S_UCG_RSTN_PPOLICY_RESETVALUE U(0x00000000)

#define LS1_URB_I2S_UCG_RSTN_PPOLICY_Pos 0
#define LS1_URB_I2S_UCG_RSTN_PPOLICY_Msk (U(0x1F) << LS1_I2S_UCG_RSTN_PPOLICY_Pos)
#define LS1_URB_I2S_UCG_RSTN_PPOLICY(value) \
	(LS1_I2S_UCG_RSTN_PPOLICY_Msk & ((value) << LS1_I2S_UCG_RSTN_PPOLICY_Pos))
#define LS1_URB_I2S_UCG_RSTN_PPOLICY_MASK U(0x0000001F)

/* --------  LS1_URB_I2S_UCG_RSTN_PSTATUS : (R/  32) I2S UCG Status Register -------- */
/* bit:  0.. 4  policy      */
/* bit:  5..31  Reserved    */
#define LS1_URB_I2S_UCG_RSTN_PSTATUS_OFFSET	0x00C
#define LS1_URB_I2S_UCG_RSTN_PSTATUS_RESETVALUE U(0x00000000)

#define LS1_URB_I2S_UCG_RSTN_PSTATUS_Pos 0
#define LS1_URB_I2S_UCG_RSTN_PSTATUS_Msk (U(0x1F) << LS1_I2S_UCG_RSTN_PSTATUS_Pos)
#define LS1_URB_I2S_UCG_RSTN_PSTATUS(value) \
	(LS1_I2S_UCG_RSTN_PSTATUS_Msk & ((value) << LS1_I2S_UCG_RSTN_PSTATUS_Pos))
#define LS1_URB_I2S_UCG_RSTN_PSTATUS_MASK U(0x0000001F)

/* --------  LS1_URB_I2S_UCG_REF_CLK : (R/W 32) I2S UCG Reference Frequency Source Selection Register -------- */
/* bit:      0  I2S_UCG_REF_CLK */
/* bit:  1..31  Reserved        */
#define LS1_URB_I2S_UCG_REF_CLK_OFFSET	   0x010
#define LS1_URB_I2S_UCG_REF_CLK_RESETVALUE U(0x00000000)

#define LS1_URB_I2S_UCG_REF_CLK_Pos  0
#define LS1_URB_I2S_UCG_REF_CLK	     (U(0x1) << LS1_URB_I2S_UCG_REF_CLK_Pos)
#define LS1_URB_I2S_UCG_REF_CLK_MASK U(0x00000001)

/* --------  LS1_URB_I2C_HIGHSPEED_PULL_ENABLE : (R/W 32) Hardware Control Register of the SCL I2C Interface Pull-up Lines -------- */
/* bit:      0  I2C1_PULL_ENABLE    */
/* bit:      1  I2C2_PULL_ENABLE    */
/* bit:      2  I2C3_PULL_ENABLE    */
/* bit:  3..31  Reserved            */
#define LS1_URB_I2C_HIGHSPEED_PULL_ENABLE_OFFSET     0x014
#define LS1_URB_I2C_HIGHSPEED_PULL_ENABLE_RESETVALUE U(0x00000000)

#define LS1_URB_I2C1_PULL_ENABLE_Pos	       0
#define LS1_URB_I2C1_PULL_ENABLE	       (U(0x1) << LS1_URB_I2C1_PULL_ENABLE_Pos)
#define LS1_URB_I2C2_PULL_ENABLE_Pos	       0
#define LS1_URB_I2C2_PULL_ENABLE	       (U(0x1) << LS1_URB_I2C2_PULL_ENABLE_Pos)
#define LS1_URB_I2C3_PULL_ENABLE_Pos	       0
#define LS1_URB_I2C3_PULL_ENABLE	       (U(0x1) << LS1_URB_I2C3_PULL_ENABLE_Pos)
#define LS1_URB_I2C_HIGHSPEED_PULL_ENABLE_MASK U(0x00000003)

/* --------  LS1_URB_GPIO1_PORT_X_PAD_N_CTR : (R/W 32) Port X Pin N Control Register -------- */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit:     11  Reserved    */
/* bit:     12  e           */
/* bit:     13  cle         */
/* bit:     14  od          */
/* bit: 15..31  Reserved    */
#define LS1_URB_GPIO1_PORT_A_PAD_0_CTR_OFFSET	   0x20
#define LS1_URB_GPIO1_PORT_A_PAD_CTR_OFFSET(value) (0x20 + (0x04 * (value)))
#define LS1_URB_GPIO1_PORT_B_PAD_0_CTR_OFFSET	   0x40
#define LS1_URB_GPIO1_PORT_B_PAD_CTR_OFFSET(value) (0x40 + (0x04 * (value)))
#define LS1_URB_GPIO1_PORT_C_PAD_0_CTR_OFFSET	   0x60
#define LS1_URB_GPIO1_PORT_C_PAD_CTR_OFFSET(value) (0x60 + (0x04 * (value)))
#define LS1_URB_GPIO1_PORT_D_PAD_0_CTR_OFFSET	   0x80
#define LS1_URB_GPIO1_PORT_D_PAD_CTR_OFFSET(value) (0x80 + (0x04 * (value)))
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_RESETVALUE  U(0x00000018)

#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SUS_Pos 0
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SUS     (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SUS_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PU_Pos  1
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PU      (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PU_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PD_Pos  2
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PD      (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_PD_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL_Pos  3
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL_Msk  (U(0x3) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL(value) \
	(LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL_Msk & ((value) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_SL_Pos))
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL_Pos 5
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL_Msk (U(0x3F) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL(value) \
	(LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL_Msk & \
	 ((value) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CTL_Pos))
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_E_Pos   12
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_E       (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_E_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CLE_Pos 13
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CLE     (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_CLE_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_OD_Pos  14
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_OD      (U(0x1) << LS1_URB_GPIO1_PORT_X_PAD_N_CTR_OD_Pos)
#define LS1_URB_GPIO1_PORT_X_PAD_N_CTR_MASK    U(0x00007FFF)

/* --------  LS1_URB_GPIO1_V18 : (R/W 32) Output Voltage Selection Register GPIO1 Pins -------- */
/* bit:      0  v18         */
/* bit:  1..31  Reserved    */
#define LS1_URB_GPIO1_V18_OFFSET     0x0A0
#define LS1_URB_GPIO1_V18_RESETVALUE U(0x00000000)

#define LS1_URB_GPIO1_V18_Pos  0
#define LS1_URB_GPIO1_V18      (U(0x1) << LS1_URB_GPIO1_V18_Pos)
#define LS1_URB_GPIO1_V18_MASK U(0x00000001)

/**
 * @brief The function returns a pointer
 *        to the URB register address
 *        of the LSPERIPH1 subsystem
 *
 * @return Pointer to the URB register address
 *         of the LSPERIPH1 subsystem
 */
ls1_urb_regs_t *ls1_periph_get_urb_registers(void);

#endif /* __LS1_URB_H__ */
