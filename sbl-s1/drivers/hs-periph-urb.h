// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __HS_URB_H__
#define __HS_URB_H__

#include <utils-def.h>
#include <mcom03.h>

/**
 * @brief Pin slew rate limitation
 *
 */
#define HS_URB_PADCFG_SL_LO_RISE U(0x00)
#define HS_URB_PADCFG_SL_HI_RISE U(0x03)

/**
 * @brief Maximum pin current
 *
 */
#define HS_URB_PADCFG_CTL_Z_STATE	  U(0x00)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_2mA  U(0x01)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_4mA  U(0x03)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_6mA  U(0x07)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_8mA  U(0x0F)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_10mA U(0x1F)
#define HS_URB_PADCFG_CTL_MAX_OUTPUT_12mA U(0x3F)

// clang-format off
/**
 * @brief Structure of the URB registers of the HSPERIPH subsystem
 *
 */
typedef struct {
    volatile unsigned int pll_cfg;             /**< \brief Offset: 0x000 (R/W 32) PLL Configuration Register */
    volatile unsigned int pll_diag;            /**< \brief Offset: 0x004 (R/W 32) PLL Diagnostic Register */
    volatile unsigned int rst;                 /**< \brief Offset: 0x008 (R/W 32) Reset Control Register */
    volatile unsigned int refclk;              /**< \brief Offset: 0x00C (R/W 32) Select Reference Frequency Register */
    volatile unsigned int qspi1_xip_en_req;    /**< \brief Offset: 0x010 (R/W 32) QSPI XiP Mode Enable Register */
    volatile unsigned int qspi1_xip_en_out;    /**< \brief Offset: 0x014 (R/  32) QSPI XiP Mode Status Register */
    volatile unsigned int qspi1_xip_cfg;       /**< \brief Offset: 0x018 (R/W 32) QSPI XiP Configuration Register */
    volatile unsigned int qspi1_padcfg;        /**< \brief Offset: 0x01C (R/W 32) QSPI XiP Pad Configuration Register */
    volatile unsigned int qspi1_ss_padcfg;     /**< \brief Offset: 0x020 (R/W 32) QSPI XiP Pad SS Configuration Register */
    volatile unsigned int qspi1_siso_padcfg;   /**< \brief Offset: 0x024 (R/W 32) QSPI XiP Pad SISO Configuration Register */
    volatile unsigned int qspi1_sclk_padcfg;   /**< \brief Offset: 0x028 (R/W 32) QSPI XiP Pad SCLK Configuration Register */
    struct {                                   /*                 SDMMC0|SDMM1                 */
        volatile unsigned int padcfg;          /**< \brief Offset: 0x02C|0x068 (R/W 32) SDMMCn Pad Configuration Register except SDMMCn_18EN, SDMMCn_PWR */
        volatile unsigned int clk_padcfg;      /**< \brief Offset: 0x030|0x06C (R/W 32) SDMMCn Pad CLK Configuration Register */
        volatile unsigned int cmd_padcfg;      /**< \brief Offset: 0x034|0x070 (R/W 32) SDMMCn Pad CMD Configuration Register */
        volatile unsigned int dat_padcfg;      /**< \brief Offset: 0x038|0x074 (R/W 32) SDMMCn Pad DATn Configuration Register */
        volatile unsigned int corecfg_0;       /**< \brief Offset: 0x03C|0x078 (R/W 32) SDMMCn Core Configuration 0 Register */
        volatile unsigned int corecfg_1;       /**< \brief Offset: 0x040|0x07C (R/W 32) SDMMCn Core Configuration 1 Register */
        volatile unsigned int corecfg_2;       /**< \brief Offset: 0x044|0x080 (R/W 32) SDMMCn Core Configuration 2 Register */
        volatile unsigned int corecfg_3;       /**< \brief Offset: 0x048|0x084 (R/W 32) SDMMCn Core Configuration 3 Register */
        volatile unsigned int corecfg_4;       /**< \brief Offset: 0x04C|0x088 (R/W 32) SDMMCn Core Configuration 4 Register */
        volatile unsigned int corecfg_5;       /**< \brief Offset: 0x050|0x08C (R/W 32) SDMMCn Core Configuration 5 Register */
        volatile unsigned int corecfg_6;       /**< \brief Offset: 0x054|0x090 (R/W 32) SDMMCn Core Configuration 6 Register */
        volatile unsigned int corecfg_7;       /**< \brief Offset: 0x058|0x094 (R/W 32) SDMMCn Core Configuration 7 Register */
        volatile unsigned int sdhc_dbg0;       /**< \brief Offset: 0x05C|0x098 (R/  32) SDMMCn SDHC Debug 0 Register */
        volatile unsigned int sdhc_dbg1;       /**< \brief Offset: 0x060|0x09C (R/  32) SDMMCn SDHC Debug 1 Register */
        volatile unsigned int sdhc_dbg2;       /**< \brief Offset: 0x064|0x0A0 (R/  32) SDMMCn SDHC Debug 2 Register */
    } sdmmc[2];
    volatile unsigned int pdma2_dbg0;          /**< \brief Offset: 0x0A4 (R/  32) PDMA2 Debug 0 Register */
    volatile unsigned int pdma2_dbg1;          /**< \brief Offset: 0x0A8 (R/  32) PDMA2 Debug 1 Register */
    volatile unsigned int pdma2_dbg2;          /**< \brief Offset: 0x0AC (R/  32) PDMA2 Debug 2 Register */
    volatile unsigned int pdma2_dbg3;          /**< \brief Offset: 0x0B0 (R/  32) PDMA2 Debug 3 Register */
    volatile unsigned int pdma2_dbg4;          /**< \brief Offset: 0x0B4 (R/  32) PDMA2 Debug 4 Register */
    volatile unsigned int pdma2_dbg5;          /**< \brief Offset: 0x0B8 (R/  32) PDMA2 Debug 5 Register */
    volatile unsigned int pdma2_dbg6;          /**< \brief Offset: 0x0BC (R/  32) PDMA2 Debug 6 Register */
    volatile unsigned int pdma2_dbg7;          /**< \brief Offset: 0x0C0 (R/  32) PDMA2 Debug 7 Register */
    volatile unsigned int pdma2_dbg8;          /**< \brief Offset: 0x0C4 (R/  32) PDMA2 Debug 8 Register */
    volatile unsigned int pdma2_bar0;          /**< \brief Offset: 0x0C8 (R/W 32) Base Address 0 AXI PDMA2 */
    volatile unsigned int pdma2_bar1;          /**< \brief Offset: 0x0CC (R/W 32) Base Address 1 AXI PDMA2 */
    volatile unsigned int pdma2_bar2;          /**< \brief Offset: 0x0D0 (R/W 32) Base Address 2 AXI PDMA2 */
    volatile unsigned int pdma2_hmx_ctr;       /**< \brief Offset: 0x0D4 (R/W 32) HMX Signal Control Register */
    struct {                                   /*                   USB0|USB1                   */
        volatile unsigned int cntr_misc;       /**< \brief Offset: 0x0D8|0x10C (R/W 32) USBn Controller Control Register */
        volatile unsigned int phy_ctr;         /**< \brief Offset: 0x0DC|0x110 (R/W 32) USBn PHY Control Register */
        volatile unsigned int phy_params0;     /**< \brief Offset: 0x0E0|0x114 (R/W 32) USBn PHY Parameter 0 Register */
        volatile unsigned int phy_params1;     /**< \brief Offset: 0x0E4|0x118 (R/W 32) USBn PHY Parameter 1 Register */
        volatile unsigned int phy_cr;          /**< \brief Offset: 0x0E8|0x11C (R/W 32) USBn PHY Control Register */
        volatile unsigned int phy_tst;         /**< \brief Offset: 0x0EC|0x120 (R/W 32) USBn PHY Test Signal Register */
        volatile unsigned int phy_tst_bypass;  /**< \brief Offset: 0x0F0|0x124 (R/W 32) USBn PHY Test Bypass Register */
        volatile unsigned int phy_rtune;       /**< \brief Offset: 0x0F4|0x128 (R/W 32) USBn PHY Re-Tune Register */
        volatile unsigned int dbg0;            /**< \brief Offset: 0x0F8|0x12C (R/  32) USBn Debug 0 Register */
        volatile unsigned int dbg1;            /**< \brief Offset: 0x0FC|0x130 (R/  32) USBn Debug 1 Register */
        volatile unsigned int dbg2;            /**< \brief Offset: 0x100|0x134 (R/  32) USBn Debug 2 Register */
        volatile unsigned int dbg3;            /**< \brief Offset: 0x104|0x138 (R/  32) USBn Debug 3 Register */
        volatile unsigned int dbg4;            /**< \brief Offset: 0x108|0x13C (R/  32) USBn Debug 4 Register */
    } usb[2];
    volatile unsigned int emac_padcfg;         /**< \brief Offset: 0x140 (R/W 32) EMAC Pad Configuration Register */
    struct {                                   /*                  EMAC0|EMAC1                  */
        volatile unsigned int padcfg;          /**< \brief Offset: 0x144|0x164 (R/W 32) EMACn Pad RGMII Configuration Register */
        volatile unsigned int tx_padcfg;       /**< \brief Offset: 0x148|0x168 (R/W 32) EMACn Pad RGMII TXD & TXCTL Configuration Register */
        volatile unsigned int txc_padcfg;      /**< \brief Offset: 0x14C|0x16C (R/W 32) EMACn Pad RGMII TXC Configuration Register */
        volatile unsigned int rx_padcfg;       /**< \brief Offset: 0x150|0x170 (R/W 32) EMACn Pad RGMII RXD & RXCTL Configuration Register */
        volatile unsigned int rxc_padcfg;      /**< \brief Offset: 0x154|0x174 (R/W 32) EMACn Pad RGMII RXC Configuration Register */
        volatile unsigned int md_padcfg;       /**< \brief Offset: 0x158|0x178 (R/W 32) EMACn Pad RGMII MDIO Configuration Register */
        volatile unsigned int mdc_padcfg;      /**< \brief Offset: 0x15C|0x17C (R/W 32) EMACn Pad RGMII MDC Configuration Register */
        volatile unsigned int bar;             /**< \brief Offset: 0x160|0x180 (R/W 32) EMACn Base Address AXI */
    } emac[2];
    volatile unsigned int nand_padcfg;         /**< \brief Offset: 0x184 (R/W 32) NAND Pad Configuration Register */
    volatile unsigned int nand_ale_cle_padcfg; /**< \brief Offset: 0x188 (R/W 32) NAND Pad NFC_ALE & NFC_CLE Configuration Register */
    volatile unsigned int nand_cen_padcfg;     /**< \brief Offset: 0x18C (R/W 32) NAND Pad NFC_CEN* Configuration Register */
    volatile unsigned int nand_ren_padcfg;     /**< \brief Offset: 0x190 (R/W 32) NAND Pad NFC_REN Configuration Register */
    volatile unsigned int nand_wen_padcfg;     /**< \brief Offset: 0x194 (R/W 32) NAND Pad NFC_WEN Configuration Register */
    volatile unsigned int nand_io_padcfg;      /**< \brief Offset: 0x198 (R/W 32) NAND Pad NFC_IO* Configuration Register */
    volatile unsigned int nand_dqs_padcfg;     /**< \brief Offset: 0x19C (R/W 32) NAND Pad NFC_DQS Configuration Register */
    volatile unsigned int nand_rbn_padcfg;     /**< \brief Offset: 0x1A0 (R/W 32) NAND Pad NFC_RBN* Configuration Register */
    volatile unsigned int misc_padcfg;         /**< \brief Offset: 0x1A4 (R/W 32) NAND Pad MISC Configuration Register */
    volatile unsigned int dbg_ctr;             /**< \brief Offset: 0x1A8 (R/W 32) Debug Register */
    volatile unsigned int dma_axcache;         /**< \brief Offset: 0x1AC (R/W 32) AXCACHE DMA Signal Configuration Register */
} hs_urb_regs_t;
// clang-format on

/*  HS_URB_PLL_CFG : (R/W 32) PLL Configuration Register */
/* bit:  0.. 7  sel         */
/* bit:      8  Reserved    */
/* bit:      9  man         */
/* bit: 10..13  od_man      */
/* bit: 14..26  nf_man      */
/* bit: 27..30  nr_man      */
/* bit:     31  lock        */
#define HS_URB_PLL_CFG_OFFSET	  0x000
#define HS_URB_PLL_CFG_RESETVALUE U(0x00000000)

#define HS_URB_PLL_CFG_SEL_Pos	  0
#define HS_URB_PLL_CFG_SEL_Msk	  (U(0xFF) << HS_URB_PLL_CFG_SEL_Pos)
#define HS_URB_PLL_CFG_SEL(value) (HS_URB_PLL_CFG_SEL_Msk & ((value) << HS_URB_PLL_CFG_SEL_Pos))
#define HS_URB_PLL_CFG_MAN_Pos	  9
#define HS_URB_PLL_CFG_MAN	  (U(0x1) << HS_URB_PLL_CFG_MAN_Pos)
#define HS_URB_PLL_CFG_OD_MAN_Pos 10
#define HS_URB_PLL_CFG_OD_MAN_Msk (U(0xF) << HS_URB_PLL_CFG_OD_MAN_Pos)
#define HS_URB_PLL_CFG_OD_MAN(value) \
	(HS_URB_PLL_CFG_OD_MAN_Msk & ((value) << HS_URB_PLL_CFG_OD_MAN_Pos))
#define HS_URB_PLL_CFG_NF_MAN_Pos 14
#define HS_URB_PLL_CFG_NF_MAN_Msk (U(0x1FFF) << HS_URB_PLL_CFG_NF_MAN_Pos)
#define HS_URB_PLL_CFG_NF_MAN(value) \
	(HS_URB_PLL_CFG_NF_MAN_Msk & ((value) << HS_URB_PLL_CFG_NF_MAN_Pos))
#define HS_URB_PLL_CFG_NR_MAN_Pos 27
#define HS_URB_PLL_CFG_NR_MAN_Msk (U(0xF) << HS_URB_PLL_CFG_NR_MAN_Pos)
#define HS_URB_PLL_CFG_NR_MAN(value) \
	(HS_URB_PLL_CFG_NR_MAN_Msk & ((value) << HS_URB_PLL_CFG_NR_MAN_Pos))
#define HS_URB_PLL_CFG_LOCK_Pos 9
#define HS_URB_PLL_CFG_LOCK	(U(0x1) << HS_URB_PLL_CFG_LOCK_Pos)
#define HS_URB_PLL_CFG_MASK	U(0xFFFFFEFF)

/*  HS_URB_PLL_DIAG : (R/W 32) PLL Diagnostic Register */
/* bit:      0  test        */
/* bit:      1  ensat       */
/* bit:      2  fasten      */
/* bit:      3  rfslip      */
/* bit:      4  fbslip      */
/* bit:  5..31  Reserved    */
#define HS_URB_PLL_DIAG_OFFSET	   0x004
#define HS_URB_PLL_DIAG_RESETVALUE U(0x00000002)

#define HS_URB_PLL_DIAG_TEST_Pos   0
#define HS_URB_PLL_DIAG_TEST	   (U(0x1) << HS_URB_PLL_DIAG_TEST_Pos)
#define HS_URB_PLL_DIAG_ENSAT_Pos  1
#define HS_URB_PLL_DIAG_ENSAT	   (U(0x1) << HS_URB_PLL_DIAG_ENSAT_Pos)
#define HS_URB_PLL_DIAG_FASTEN_Pos 2
#define HS_URB_PLL_DIAG_FASTE	   (U(0x1) << HS_URB_PLL_DIAG_FASTEN_Pos)
#define HS_URB_PLL_DIAG_RFSLIP_Pos 3
#define HS_URB_PLL_DIAG_RFSLIP	   (U(0x1) << HS_URB_PLL_DIAG_RFSLIP_Pos)
#define HS_URB_PLL_DIAG_FBSLIP_Pos 4
#define HS_URB_PLL_DIAG_FBSLIP	   (U(0x1) << HS_URB_PLL_DIAG_FBSLIP_Pos)
#define HS_URB_PLL_DIAG_MASK	   U(0x0000001F)

/*  HS_URB_RST : (R/W 32) Reset Control Register */
/* bit:      0  dbg         */
/* bit:      1  pdma2       */
/* bit:      2  qspi        */
/* bit:      3  nfc         */
/* bit:      4  sdmmc0      */
/* bit:      5  sdmmc1      */
/* bit:      6  usb0        */
/* bit:      7  usb1        */
/* bit:      8  emac0       */
/* bit:      9  emac1       */
/* bit: 10..15  Reserved    */
/* bit:     16  dbg_wr      */
/* bit:     17  pdma2_wr    */
/* bit:     18  qspi_wr     */
/* bit:     19  nfc_wr      */
/* bit:     20  sdmmc0_wr   */
/* bit:     21  sdmmc1_wr   */
/* bit:     22  usb0_wr     */
/* bit:     23  usb1_wr     */
/* bit:     24  emac0_wr    */
/* bit:     25  emac1_wr    */
/* bit: 26..31  Reserved    */
#define HS_URB_RST_OFFSET     0x008
#define HS_URB_RST_RESETVALUE U(0x000003FF)

#define HS_URB_RST_DBG_Pos	 0
#define HS_URB_RST_DBG		 (U(0x1) << HS_URB_RST_DBG_Pos)
#define HS_URB_RST_PDMA2_Pos	 1
#define HS_URB_RST_PDMA2	 (U(0x1) << HS_URB_RST_PDMA2_Pos)
#define HS_URB_RST_QSPI_Pos	 2
#define HS_URB_RST_QSPI		 (U(0x1) << HS_URB_RST_QSPI_Pos)
#define HS_URB_RST_NFC_Pos	 3
#define HS_URB_RST_NFC		 (U(0x1) << HS_URB_RST_NFC_Pos)
#define HS_URB_RST_SDMMC0_Pos	 4
#define HS_URB_RST_SDMMC0	 (U(0x1) << HS_URB_RST_SDMMC0_Pos)
#define HS_URB_RST_SDMMC1_Pos	 5
#define HS_URB_RST_SDMMC1	 (U(0x1) << HS_URB_RST_SDMMC1_Pos)
#define HS_URB_RST_USB0_Pos	 6
#define HS_URB_RST_USB0		 (U(0x1) << HS_URB_RST_USB0_Pos)
#define HS_URB_RST_USB1_Pos	 7
#define HS_URB_RST_USB1		 (U(0x1) << HS_URB_RST_USB1_Pos)
#define HS_URB_RST_EMAC0_Pos	 8
#define HS_URB_RST_EMAC0	 (U(0x1) << HS_URB_RST_EMAC0_Pos)
#define HS_URB_RST_EMAC1_Pos	 9
#define HS_URB_RST_EMAC1	 (U(0x1) << HS_URB_RST_EMAC1_Pos)
#define HS_URB_RST_DBG_WR_Pos	 16
#define HS_URB_RST_DBG_WR	 (U(0x1) << HS_URB_RST_DBG_WR_Pos)
#define HS_URB_RST_PDMA2_WR_Pos	 17
#define HS_URB_RST_PDMA2_WR	 (U(0x1) << HS_URB_RST_PDMA2_WR_Pos)
#define HS_URB_RST_QSPI_WR_Pos	 18
#define HS_URB_RST_QSPI_WR	 (U(0x1) << HS_URB_RST_QSPI_WR_Pos)
#define HS_URB_RST_NFC_WR_Pos	 19
#define HS_URB_RST_NFC_WR	 (U(0x1) << HS_URB_RST_NFC_WR_Pos)
#define HS_URB_RST_SDMMC0_WR_Pos 20
#define HS_URB_RST_SDMMC0_WR	 (U(0x1) << HS_URB_RST_SDMMC0_WR_Pos)
#define HS_URB_RST_SDMMC1_WR_Pos 21
#define HS_URB_RST_SDMMC1_WR	 (U(0x1) << HS_URB_RST_SDMMC1_WR_Pos)
#define HS_URB_RST_USB0_WR_Pos	 22
#define HS_URB_RST_USB0_WR	 (U(0x1) << HS_URB_RST_USB0_WR_Pos)
#define HS_URB_RST_USB1_WR_Pos	 23
#define HS_URB_RST_USB1_WR	 (U(0x1) << HS_URB_RST_USB1_WR_Pos)
#define HS_URB_RST_EMAC0_WR_Pos	 24
#define HS_URB_RST_EMAC0_WR	 (U(0x1) << HS_URB_RST_EMAC0_WR_Pos)
#define HS_URB_RST_EMAC1_WR_Pos	 25
#define HS_URB_RST_EMAC1_WR	 (U(0x1) << HS_URB_RST_EMAC1_WR_Pos)
#define HS_URB_RST_MASK		 U(0x03FF03FF)

/*  HS_URB_REFCLK : (R/W 32) Select Reference Frequency Register */
/* bit:  0.. 1  ucg0        */
/* bit:  2.. 3  ucg1        */
/* bit:  4.. 5  ucg2        */
/* bit:  6.. 7  ucg3        */
/* bit:  8..31  Reserved    */
#define HS_URB_REFCLK_OFFSET	 0x00C
#define HS_URB_REFCLK_RESETVALUE U(0x00000050)

#define HS_URB_REFCLK_UCG0_Pos	  0
#define HS_URB_REFCLK_UCG0_Msk	  (U(0x3) << HS_URB_REFCLK_UCG0_Pos)
#define HS_URB_REFCLK_UCG0(value) (HS_URB_REFCLK_UCG0_Msk & ((value) << HS_URB_REFCLK_UCG0_Pos))
#define HS_URB_REFCLK_UCG1_Pos	  2
#define HS_URB_REFCLK_UCG1_Msk	  (U(0x3) << HS_URB_REFCLK_UCG1_Pos)
#define HS_URB_REFCLK_UCG1(value) (HS_URB_REFCLK_UCG1_Msk & ((value) << HS_URB_REFCLK_UCG1_Pos))
#define HS_URB_REFCLK_UCG2_Pos	  4
#define HS_URB_REFCLK_UCG2_Msk	  (U(0x3) << HS_URB_REFCLK_UCG2_Pos)
#define HS_URB_REFCLK_UCG2(value) (HS_URB_REFCLK_UCG2_Msk & ((value) << HS_URB_REFCLK_UCG2_Pos))
#define HS_URB_REFCLK_UCG3_Pos	  6
#define HS_URB_REFCLK_UCG3_Msk	  (U(0x3) << HS_URB_REFCLK_UCG3_Pos)
#define HS_URB_REFCLK_UCG3(value) (HS_URB_REFCLK_UCG3_Msk & ((value) << HS_URB_REFCLK_UCG3_Pos))
#define HS_URB_REFCLK_MASK	  U(0x000000FF)

/*  HS_URB_QSPI1_XIP_EN_REQ : (R/W 32) QSPI XiP Mode Enable Register */
/* bit:      0  en          */
/* bit:  1..31  Reserved    */
#define HS_URB_QSPI1_XIP_EN_REQ_OFFSET	   0x010
#define HS_URB_QSPI1_XIP_EN_REQ_RESETVALUE U(0x00000000)

#define HS_URB_QSPI1_XIP_EN_REQ_EN_Pos 0
#define HS_URB_QSPI1_XIP_EN_REQ_EN     (U(0x1) << HS_URB_QSPI1_XIP_EN_REQ_EN_Pos)
#define HS_URB_QSPI1_XIP_EN_REQ_MASK   U(0x00000001)

/*  HS_URB_QSPI1_XIP_EN_OUT : (R/  32) QSPI XiP Mode Status Register */
/* bit:      0  status      */
/* bit:  1..31  Reserved    */
#define HS_URB_QSPI1_XIP_EN_OUT_OFFSET	   0x014
#define HS_URB_QSPI1_XIP_EN_OUT_RESETVALUE U(0x00000000)

#define HS_URB_QSPI1_XIP_EN_REQ_STATUS_Pos 0
#define HS_URB_QSPI1_XIP_EN_REQ_STATUS	   (U(0x1) << HS_URB_QSPI1_XIP_EN_REQ_STATUS_Pos)
#define HS_URB_QSPI1_XIP_EN_OUT_MASK	   U(0x00000001)

/*  HS_URB_QSPI1_XIP_CFG : (R/W 32) QSPI XiP Configuration Register */
/* bit:  0.. 7  cmd             */
/* bit:  8..15  hp_mode         */
/* bit: 16..19  ssen            */
/* bit: 20..23  dummy_cycles    */
/* bit: 24..25  hp_end_dummy    */
/* bit:     26  cpha            */
/* bit:     27  cpol            */
/* bit:     28  addr4           */
/* bit:     29  le32            */
/* bit:     30  hpen            */
/* bit:     31  Reserved        */
#define HS_URB_QSPI1_XIP_CFG_OFFSET	0x018
#define HS_URB_QSPI1_XIP_CFG_RESETVALUE U(0x20010003)

#define HS_URB_QSPI1_XIP_CFG_CMD_Pos 0
#define HS_URB_QSPI1_XIP_CFG_CMD_Msk (U(0xFF) << HS_URB_QSPI1_XIP_CFG_CMD_Pos)
#define HS_URB_QSPI1_XIP_CFG_CMD(value) \
	(HS_URB_QSPI1_XIP_CFG_CMD_Msk & ((value) << HS_URB_QSPI1_XIP_CFG_CMD_Pos))
#define HS_URB_QSPI1_XIP_CFG_HP_MODE_Pos 8
#define HS_URB_QSPI1_XIP_CFG_HP_MODE_Msk (U(0xFF) << HS_URB_QSPI1_XIP_CFG_HP_MODE_Pos)
#define HS_URB_QSPI1_XIP_CFG_HP_MODE(value) \
	(HS_URB_QSPI1_XIP_CFG_HP_MODE_Msk & ((value) << HS_URB_QSPI1_XIP_CFG_HP_MODE_Pos))
#define HS_URB_QSPI1_XIP_CFG_SSEN_Pos 16
#define HS_URB_QSPI1_XIP_CFG_SSEN_Msk (U(0xF) << HS_URB_QSPI1_XIP_CFG_SSEN_Pos)
#define HS_URB_QSPI1_XIP_CFG_SSEN(value) \
	(HS_URB_QSPI1_XIP_CFG_SSEN_Msk & ((value) << HS_URB_QSPI1_XIP_CFG_SSEN_Pos))
#define HS_URB_QSPI1_XIP_CFG_DMY_CYCLES_Pos 20
#define HS_URB_QSPI1_XIP_CFG_DMY_CYCLES_Msk (U(0xF) << HS_URB_QSPI1_XIP_CFG_DMY_CYCLES_Pos)
#define HS_URB_QSPI1_XIP_CFG_DMY_CYCLES(value) \
	(HS_URB_QSPI1_XIP_CFG_DMY_CYCLES & ((value) << HS_URB_QSPI1_XIP_CFG_DMY_CYCLES_Pos))
#define HS_URB_QSPI1_XIP_CFG_HP_END_DMY_Pos 24
#define HS_URB_QSPI1_XIP_CFG_HP_END_DMY_Msk (U(0x3) << HS_URB_QSPI1_XIP_CFG_HP_END_DMY_Pos)
#define HS_URB_QSPI1_XIP_CFG_HP_END_DMY(value) \
	(HS_URB_QSPI1_XIP_CFG_HP_END_DMY_Msk & ((value) << HS_URB_QSPI1_XIP_CFG_HP_END_DMY_Pos))
#define HS_URB_QSPI1_XIP_CFG_CPHA_Pos  26
#define HS_URB_QSPI1_XIP_CFG_CPHA      (U(0x1) << HS_URB_QSPI1_XIP_CFG_CPHA_Pos)
#define HS_URB_QSPI1_XIP_CFG_CPOL_Pos  27
#define HS_URB_QSPI1_XIP_CFG_CPOL      (U(0x1) << HS_URB_QSPI1_XIP_CFG_CPOL_Pos)
#define HS_URB_QSPI1_XIP_CFG_ADDR4_Pos 28
#define HS_URB_QSPI1_XIP_CFG_ADDR4     (U(0x1) << HS_URB_QSPI1_XIP_CFG_ADDR4_Pos)
#define HS_URB_QSPI1_XIP_CFG_LE32_Pos  29
#define HS_URB_QSPI1_XIP_CFG_LE32      (U(0x1) << HS_URB_QSPI1_XIP_CFG_LE32_Pos)
#define HS_URB_QSPI1_XIP_CFG_HPEN_Pos  30
#define HS_URB_QSPI1_XIP_CFG_HPEN      (U(0x1) << HS_URB_QSPI1_XIP_CFG_HPEN_Pos)
#define HS_URB_QSPI1_XIP_CFG_MASK      U(0x7FFFFFFF)

/*  HS_URB_QSPI1_PADCFG : (R/W 32) QSPI XiP Pad Configuration Register */
/* bit:      0  en              */
/* bit:      1  v18             */
/* bit:      2  cle             */
/* bit:      3  od              */
/* bit:  4..31  Reserved        */
#define HS_URB_QSPI1_PADCFG_OFFSET     0x01C
#define HS_URB_QSPI1_PADCFG_RESETVALUE U(0x00000004)

#define HS_URB_QSPI1_PADCFG_EN_Pos  0
#define HS_URB_QSPI1_PADCFG_EN	    (U(0x1) << HS_URB_QSPI1_PADCFG_EN_Pos)
#define HS_URB_QSPI1_PADCFG_V18_Pos 1
#define HS_URB_QSPI1_PADCFG_V18	    (U(0x1) << HS_URB_QSPI1_PADCFG_V18_Pos)
#define HS_URB_QSPI1_PADCFG_CLE_Pos 2
#define HS_URB_QSPI1_PADCFG_CLE	    (U(0x1) << HS_URB_QSPI1_PADCFG_CLE_Pos)
#define HS_URB_QSPI1_PADCFG_OD_Pos  3
#define HS_URB_QSPI1_PADCFG_OD	    (U(0x1) << HS_URB_QSPI1_PADCFG_OD_Pos)
#define HS_URB_QSPI1_PADCFG_MASK    U(0x0000000F)

/*  HS_URB_QSPI1_SS_PADCFG : (R/W 32) QSPI XiP Pad SS Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_QSPI1_SS_PADCFG_OFFSET	  0x020
#define HS_URB_QSPI1_SS_PADCFG_RESETVALUE U(0x00000078)

#define HS_URB_QSPI1_SS_PADCFG_SUS_Pos 0
#define HS_URB_QSPI1_SS_PADCFG_SUS     (U(0x1) << HS_URB_QSPI1_SS_PADCFG_SUS_Pos)
#define HS_URB_QSPI1_SS_PADCFG_PU_Pos  1
#define HS_URB_QSPI1_SS_PADCFG_PU      (U(0x1) << HS_URB_QSPI1_SS_PADCFG_PU_Pos)
#define HS_URB_QSPI1_SS_PADCFG_PD_Pos  2
#define HS_URB_QSPI1_SS_PADCFG_PD      (U(0x1) << HS_URB_QSPI1_SS_PADCFG_PD_Pos)
#define HS_URB_QSPI1_SS_PADCFG_SL_Pos  3
#define HS_URB_QSPI1_SS_PADCFG_SL_Msk  (U(0x3) << HS_URB_QSPI1_SS_PADCFG_SL_Pos)
#define HS_URB_QSPI1_SS_PADCFG_SL(value) \
	(HS_URB_QSPI1_SS_PADCFG_SL_Msk & ((value) << HS_URB_QSPI1_SS_PADCFG_SL_Pos))
#define HS_URB_QSPI1_SS_PADCFG_CTL_Pos 5
#define HS_URB_QSPI1_SS_PADCFG_CTL_Msk (U(0x3F) << HS_URB_QSPI1_SS_PADCFG_CTL_Pos)
#define HS_URB_QSPI1_SS_PADCFG_CTL(value) \
	(HS_URB_QSPI1_SS_PADCFG_CTL_Msk & ((value) << HS_URB_QSPI1_SS_PADCFG_CTL_Pos))
#define HS_URB_QSPI1_SS_PADCFG_ST_Pos 15
#define HS_URB_QSPI1_SS_PADCFG_ST     (U(0x1) << HS_URB_QSPI1_SS_PADCFG_ST_Pos)
#define HS_URB_QSPI1_SS_PADCFG_MASK   U(0x000087FF)

/*  HS_URB_QSPI1_SISO_PADCFGG : (R/W 32) QSPI XiP Pad SISO Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_QSPI1_SISO_PADCFGG_OFFSET     0x024
#define HS_URB_QSPI1_SISO_PADCFGG_RESETVALUE U(0x00000078)

#define HS_URB_QSPI1_SISO_PADCFG_SUS_Pos 0
#define HS_URB_QSPI1_SISO_PADCFG_SUS	 (U(0x1) << HS_URB_QSPI1_SISO_PADCFG_SUS_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_PU_Pos	 1
#define HS_URB_QSPI1_SISO_PADCFG_PU	 (U(0x1) << HS_URB_QSPI1_SISO_PADCFG_PU_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_PD_Pos	 2
#define HS_URB_QSPI1_SISO_PADCFG_PD	 (U(0x1) << HS_URB_QSPI1_SISO_PADCFG_PD_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_SL_Pos	 3
#define HS_URB_QSPI1_SISO_PADCFG_SL_Msk	 (U(0x3) << HS_URB_QSPI1_SISO_PADCFG_SL_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_SL(value) \
	(HS_URB_QSPI1_SISO_PADCFG_SL_Msk & ((value) << HS_URB_QSPI1_SISO_PADCFG_SL_Pos))
#define HS_URB_QSPI1_SISO_PADCFG_CTL_Pos 5
#define HS_URB_QSPI1_SISO_PADCFG_CTL_Msk (U(0x3F) << HS_URB_QSPI1_SISO_PADCFG_CTL_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_CTL(value) \
	(HS_URB_QSPI1_SISO_PADCFG_CTL_Msk & ((value) << HS_URB_QSPI1_SISO_PADCFG_CTL_Pos))
#define HS_URB_QSPI1_SISO_PADCFG_ST_Pos 15
#define HS_URB_QSPI1_SISO_PADCFG_ST	(U(0x1) << HS_URB_QSPI1_SISO_PADCFG_ST_Pos)
#define HS_URB_QSPI1_SISO_PADCFG_MASK	U(0x000087FF)

/*  HS_URB_QSPI1_SCLK_PADCFG : (R/W 32) QSPI XiP Pad SCLK Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_QSPI1_SCLK_PADCFG_OFFSET	    0x028
#define HS_URB_QSPI1_SCLK_PADCFG_RESETVALUE U(0x00000078)

#define HS_URB_QSPI1_SCLK_PADCFG_SUS_Pos 0
#define HS_URB_QSPI1_SCLK_PADCFG_SUS	 (U(0x1) << HS_URB_QSPI1_SCLK_PADCFG_SUS_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_PU_Pos	 1
#define HS_URB_QSPI1_SCLK_PADCFG_PU	 (U(0x1) << HS_URB_QSPI1_SCLK_PADCFG_PU_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_PD_Pos	 2
#define HS_URB_QSPI1_SCLK_PADCFG_PD	 (U(0x1) << HS_URB_QSPI1_SCLK_PADCFG_PD_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_SL_Pos	 3
#define HS_URB_QSPI1_SCLK_PADCFG_SL_Msk	 (U(0x3) << HS_URB_QSPI1_SCLK_PADCFG_SL_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_SL(value) \
	(HS_URB_QSPI1_SCLK_PADCFG_SL_Msk & ((value) << HS_URB_QSPI1_SCLK_PADCFG_SL_Pos))
#define HS_URB_QSPI1_SCLK_PADCFG_CTL_Pos 5
#define HS_URB_QSPI1_SCLK_PADCFG_CTL_Msk (U(0x3F) << HS_URB_QSPI1_SCLK_PADCFG_CTL_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_CTL(value) \
	(HS_URB_QSPI1_SCLK_PADCFG_CTL_Msk & ((value) << HS_URB_QSPI1_SCLK_PADCFG_CTL_Pos))
#define HS_URB_QSPI1_SCLK_PADCFG_ST_Pos 15
#define HS_URB_QSPI1_SCLK_PADCFG_ST	(U(0x1) << HS_URB_QSPI1_SCLK_PADCFG_ST_Pos)
#define HS_URB_QSPI1_SCLK_PADCFG_MASK	U(0x000087FF)

/*  HS_URB_SDMMC_PADCFG : (R/W 32) SDMMCn Pad Configuration Register except SDMMCn_18EN, SDMMCn_PWR */
/* bit:      0  en          */
/* bit:      1  v18         */
/* bit:      2  cle         */
/* bit:      3  use_sw_v18  */
/* bit:  4..23  v18delay    */
/* bit: 24..31  Reserved    */
#define HS_URB_SDMMC0_PADCFG_OFFSET    0x02C
#define HS_URB_SDMMC1_PADCFG_OFFSET    0x068
#define HS_URB_SDMMC_PADCFG_RESETVALUE U(0x00186A00)

#define HS_URB_SDMMC_PADCFG_EN_Pos	 0
#define HS_URB_SDMMC_PADCFG_EN		 (U(0x1) << HS_URB_SDMMC_PADCFG_EN_Pos)
#define HS_URB_SDMMC_PADCFG_V18_Pos	 1
#define HS_URB_SDMMC_PADCFG_V18		 (U(0x1) << HS_URB_SDMMC_PADCFG_V18_Pos)
#define HS_URB_SDMMC_PADCFG_CLE_Pos	 2
#define HS_URB_SDMMC_PADCFG_CLE		 (U(0x1) << HS_URB_SDMMC_PADCFG_CLE_Pos)
#define HS_URB_SDMMC_PADCFG_SW_V18_Pos	 3
#define HS_URB_SDMMC_PADCFG_SW_V18	 (U(0x1) << HS_URB_SDMMC_PADCFG_SW_V18_Pos)
#define HS_URB_SDMMC_PADCFG_V18DELAY_Pos 4
#define HS_URB_SDMMC_PADCFG_V18DELAY_Msk (U(0xFFFFF) << HS_URB_SDMMC_PADCFG_V18DELAY_Pos)
#define HS_URB_SDMMC_PADCFG_V18DELAY(value) \
	(HS_URB_SDMMC_PADCFG_V18DELAY_Msk & ((value) << HS_URB_SDMMC_PADCFG_V18DELAY_Pos))
#define HS_URB_SDMMC_PADCFG_MASK U(0x00FFFFFF)

/*  HS_URB_SDMMC_CLK_PADCFG : (R/W 32) SDMMCn Pad CLK Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit:     16  od          */
/* bit:     17  use_sw_ctl  */
/* bit: 18..31  Reserved    */
#define HS_URB_SDMMC0_CLK_PADCFG_OFFSET	   0x030
#define HS_URB_SDMMC1_CLK_PADCFG_OFFSET	   0x06C
#define HS_URB_SDMMC_CLK_PADCFG_RESETVALUE U(0x00000078)

#define HS_URB_SDMMC_CLK_PADCFG_SUS_Pos 0
#define HS_URB_SDMMC_CLK_PADCFG_SUS	(U(0x1) << HS_URB_SDMMC_CLK_PADCFG_SUS_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_PU_Pos	1
#define HS_URB_SDMMC_CLK_PADCFG_PU	(U(0x1) << HS_URB_SDMMC_CLK_PADCFG_PU_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_PD_Pos	2
#define HS_URB_SDMMC_CLK_PADCFG_PD	(U(0x1) << HS_URB_SDMMC_CLK_PADCFG_PD_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_SL_Pos	3
#define HS_URB_SDMMC_CLK_PADCFG_SL_Msk	(U(0x3) << HS_URB_SDMMC_CLK_PADCFG_SL_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_SL(value) \
	(HS_URB_SDMMC_CLK_PADCFG_SL_Msk & ((value) << HS_URB_SDMMC_CLK_PADCFG_SL_Pos))
#define HS_URB_SDMMC_CLK_PADCFG_CTL_Pos 5
#define HS_URB_SDMMC_CLK_PADCFG_CTL_Msk (U(0x3F) << HS_URB_SDMMC_CLK_PADCFG_CTL_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_CTL(value) \
	(HS_URB_SDMMC_CLK_PADCFG_CTL_Msk & ((value) << HS_URB_SDMMC_CLK_PADCFG_CTL_Pos))
#define HS_URB_SDMMC_CLK_PADCFG_ST_Pos	   15
#define HS_URB_SDMMC_CLK_PADCFG_ST	   (U(0x1) << HS_URB_SDMMC_CLK_PADCFG_ST_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_OD_Pos	   16
#define HS_URB_SDMMC_CLK_PADCFG_OD	   (U(0x1) << HS_URB_SDMMC_CLK_PADCFG_OD_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_SW_CTL_Pos 17
#define HS_URB_SDMMC_CLK_PADCFG_SW_CTL	   (U(0x1) << HS_URB_SDMMC_CLK_PADCFG_SW_CTL_Pos)
#define HS_URB_SDMMC_CLK_PADCFG_MASK	   U(0x000387FF)

/*  HS_URB_SDMMC_CMD_PADCFG : (R/W 32) SDMMCn Pad CMD Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit:     16  od          */
/* bit:     17  use_sw_ctl  */
/* bit: 18..31  Reserved    */
#define HS_URB_SDMMC0_CMD_PADCFG_OFFSET	   0x034
#define HS_URB_SDMMC1_CMD_PADCFG_OFFSET	   0x070
#define HS_URB_SDMMC_CMD_PADCFG_RESETVALUE U(0x00000078)

#define HS_URB_SDMMC_CMD_PADCFG_SUS_Pos 0
#define HS_URB_SDMMC_CMD_PADCFG_SUS	(U(0x1) << HS_URB_SDMMC_CMD_PADCFG_SUS_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_PU_Pos	1
#define HS_URB_SDMMC_CMD_PADCFG_PU	(U(0x1) << HS_URB_SDMMC_CMD_PADCFG_PU_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_PD_Pos	2
#define HS_URB_SDMMC_CMD_PADCFG_PD	(U(0x1) << HS_URB_SDMMC_CMD_PADCFG_PD_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_SL_Pos	3
#define HS_URB_SDMMC_CMD_PADCFG_SL_Msk	(U(0x3) << HS_URB_SDMMC_CMD_PADCFG_SL_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_SL(value) \
	(HS_URB_SDMMC_CMD_PADCFG_SL_Msk & ((value) << HS_URB_SDMMC_CMD_PADCFG_SL_Pos))
#define HS_URB_SDMMC_CMD_PADCFG_CTL_Pos 5
#define HS_URB_SDMMC_CMD_PADCFG_CTL_Msk (U(0x3F) << HS_URB_SDMMC_CMD_PADCFG_CTL_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_CTL(value) \
	(HS_URB_SDMMC_CMD_PADCFG_CTL_Msk & ((value) << HS_URB_SDMMC_CMD_PADCFG_CTL_Pos))
#define HS_URB_SDMMC_CMD_PADCFG_ST_Pos	   15
#define HS_URB_SDMMC_CMD_PADCFG_ST	   (U(0x1) << HS_URB_SDMMC_CMD_PADCFG_ST_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_OD_Pos	   16
#define HS_URB_SDMMC_CMD_PADCFG_OD	   (U(0x1) << HS_URB_SDMMC_CMD_PADCFG_OD_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_SW_CTL_Pos 17
#define HS_URB_SDMMC_CMD_PADCFG_SW_CTL	   (U(0x1) << HS_URB_SDMMC_CMD_PADCFG_SW_CTL_Pos)
#define HS_URB_SDMMC_CMD_PADCFG_MASK	   U(0x000387FF)

/*  HS_URB_SDMMC_DAT_PADCFG : (R/W 32) SDMMCn Pad DAT Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit:     16  od          */
/* bit:     17  use_sw_ctl  */
/* bit: 18..31  Reserved    */
#define HS_URB_SDMMC0_DAT_PADCFG_OFFSET	   0x038
#define HS_URB_SDMMC1_DAT_PADCFG_OFFSET	   0x074
#define HS_URB_SDMMC_DAT_PADCFG_RESETVALUE U(0x00000078)

#define HS_URB_SDMMC_DAT_PADCFG_SUS_Pos 0
#define HS_URB_SDMMC_DAT_PADCFG_SUS	(U(0x1) << HS_URB_SDMMC_DAT_PADCFG_SUS_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_PU_Pos	1
#define HS_URB_SDMMC_DAT_PADCFG_PU	(U(0x1) << HS_URB_SDMMC_DAT_PADCFG_PU_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_PD_Pos	2
#define HS_URB_SDMMC_DAT_PADCFG_PD	(U(0x1) << HS_URB_SDMMC_DAT_PADCFG_PD_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_SL_Pos	3
#define HS_URB_SDMMC_DAT_PADCFG_SL_Msk	(U(0x3) << HS_URB_SDMMC_DAT_PADCFG_SL_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_SL(value) \
	(HS_URB_SDMMC_DAT_PADCFG_SL_Msk & ((value) << HS_URB_SDMMC_DAT_PADCFG_SL_Pos))
#define HS_URB_SDMMC_DAT_PADCFG_CTL_Pos 5
#define HS_URB_SDMMC_DAT_PADCFG_CTL_Msk (U(0x3F) << HS_URB_SDMMC_DAT_PADCFG_CTL_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_CTL(value) \
	(HS_URB_SDMMC_DAT_PADCFG_CTL_Msk & ((value) << HS_URB_SDMMC_DAT_PADCFG_CTL_Pos))
#define HS_URB_SDMMC_DAT_PADCFG_ST_Pos	   15
#define HS_URB_SDMMC_DAT_PADCFG_ST	   (U(0x1) << HS_URB_SDMMC_DAT_PADCFG_ST_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_OD_Pos	   16
#define HS_URB_SDMMC_DAT_PADCFG_OD	   (U(0x1) << HS_URB_SDMMC_DAT_PADCFG_OD_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_SW_CTL_Pos 17
#define HS_URB_SDMMC_DAT_PADCFG_SW_CTL	   (U(0x1) << HS_URB_SDMMC_DAT_PADCFG_SW_CTL_Pos)
#define HS_URB_SDMMC_DAT_PADCFG_MASK	   U(0x000387FF)

/*  HS_URB_SDMMC_CORECFG_0: (R/W 32) SDMMCn Core Configuration 0 Register */
/* bit:      0  sdr50support        */
/* bit:      1  sdr104support       */
/* bit:      2  ddr50support        */
/* bit:      3  Reserved            */
/* bit:      4  adriversupport      */
/* bit:      5  cdriversupport      */
/* bit:      6  ddriversupport      */
/* bit:      7  Reserved            */
/* bit:  8..11  retuningtimercnt    */
/* bit:     12  Reserved            */
/* bit:     13  tuningforsdr50      */
/* bit: 14..15  retuningmodes       */
/* bit: 16..23  clockmultiplier     */
/* bit:     24  spisupport          */
/* bit:     25  spiblkmode          */
/* bit: 26..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_0_OFFSET	  0x03C
#define HS_URB_SDMMC1_CORECFG_0_OFFSET	  0x078
#define HS_URB_SDMMC_CORECFG_0_RESETVALUE U(0x01000077)

#define HS_URB_SDMMC_CORECFG_0_SDR50_SUP_Pos   0
#define HS_URB_SDMMC_CORECFG_0_SDR50_SUP       (U(0x1) << HS_URB_SDMMC_CORECFG_0_SDR50_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_SDR104_SUP_Pos  1
#define HS_URB_SDMMC_CORECFG_0_SDR104_SUP      (U(0x1) << HS_URB_SDMMC_CORECFG_0_SDR104_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_DDR50_SUP_Pos   2
#define HS_URB_SDMMC_CORECFG_0_DDR50_SUP       (U(0x1) << HS_URB_SDMMC_CORECFG_0_DDR50_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_ADRV_SUP_Pos    4
#define HS_URB_SDMMC_CORECFG_0_ADRV_SUP	       (U(0x1) << HS_URB_SDMMC_CORECFG_0_ADRV_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_CDRV_SUP_Pos    5
#define HS_URB_SDMMC_CORECFG_0_CDRV_SUP	       (U(0x1) << HS_URB_SDMMC_CORECFG_0_CDRV_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_DDRV_SUP_Pos    6
#define HS_URB_SDMMC_CORECFG_0_DDRV_SUP	       (U(0x1) << HS_URB_SDMMC_CORECFG_0_DDRV_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT_Pos 8
#define HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT_Msk (U(0xF) << HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT_Pos)
#define HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT(value) \
	(HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_0_RETUNTIMCNT_Pos))
#define HS_URB_SDMMC_CORECFG_0_TUNSDR50_Pos 13
#define HS_URB_SDMMC_CORECFG_0_TUNSDR50	    (U(0x1) << HS_URB_SDMMC_CORECFG_0_TUNSDR50_Pos)
#define HS_URB_SDMMC_CORECFG_0_RETUNMOD_Pos 14
#define HS_URB_SDMMC_CORECFG_0_RETUNMOD_Msk (U(0x3) << HS_URB_SDMMC_CORECFG_0_RETUNMOD_Pos)
#define HS_URB_SDMMC_CORECFG_0_RETUNMOD(value) \
	(HS_URB_SDMMC_CORECFG_0_RETUNMOD_Msk & ((value) << HS_URB_SDMMC_CORECFG_0_RETUNMOD_Pos))
#define HS_URB_SDMMC_CORECFG_0_CLKMULT_Pos 16
#define HS_URB_SDMMC_CORECFG_0_CLKMULT_Msk (U(0xFF) << HS_URB_SDMMC_CORECFG_0_CLKMULT_Pos)
#define HS_URB_SDMMC_CORECFG_0_CLKMULT(value) \
	(HS_URB_SDMMC_CORECFG_0_CLKMULT_Msk & ((value) << HS_URB_SDMMC_CORECFG_0_CLKMULT_Pos))
#define HS_URB_SDMMC_CORECFG_0_SPI_SUP_Pos   24
#define HS_URB_SDMMC_CORECFG_0_SPI_SUP	     (U(0x1) << HS_URB_SDMMC_CORECFG_0_SPI_SUP_Pos)
#define HS_URB_SDMMC_CORECFG_0_SPIBLKMOD_Pos 25
#define HS_URB_SDMMC_CORECFG_0_SPIBLKMOD     (U(0x1) << HS_URB_SDMMC_CORECFG_0_SPIBLKMOD_Pos)
#define HS_URB_SDMMC_CORECFG_0_MASK	     U(0x3FFEF77)

/*  HS_URB_SDMMC_CORECFG_1: (R/W 32) SDMMCn Core Configuration 1 Register */
/* bit:  0.. 5  timeoutclkfreq      */
/* bit:      6  Reserved            */
/* bit:      7  timeoutclkunit      */
/* bit:  8..15  baseclkfreq         */
/* bit: 16..17  maxblklength        */
/* bit:     18  sdif_8bit           */
/* bit:     19  adma2               */
/* bit:     20  Reserved            */
/* bit:     21  highspeed           */
/* bit:     22  sdma                */
/* bit:     23  suspres             */
/* bit:     24  volt_3p3            */
/* bit:     25  volt_3p0            */
/* bit:     26  volt_1p8            */
/* bit:     27  Reserved            */
/* bit:     28  addr_64bit          */
/* bit:     29  asynchintr          */
/* bit: 30..31  slottype            */
#define HS_URB_SDMMC0_CORECFG_1_OFFSET	  0x040
#define HS_URB_SDMMC1_CORECFG_1_OFFSET	  0x07C
#define HS_URB_SDMMC_CORECFG_1_RESETVALUE U(0x35EE1980)

#define HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ_Pos 0
#define HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ_Msk (U(0x3F) << HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ_Pos)
#define HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ(value) \
	(HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_1_TOUTCLKFREQ_Pos))
#define HS_URB_SDMMC_CORECFG_1_TOUTCLKUNIT_Pos 7
#define HS_URB_SDMMC_CORECFG_1_TOUTCLKUNIT     (U(0x1) << HS_URB_SDMMC_CORECFG_1_TOUTCLKUNIT_Pos)
#define HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Pos 8
#define HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Msk (U(0xFF) << HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Pos)
#define HS_URB_SDMMC_CORECFG_1_BASECLKFREQ(value) \
	(HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Pos))
#define HS_URB_SDMMC_CORECFG_1_MAXBLKLEN_Pos 16
#define HS_URB_SDMMC_CORECFG_1_MAXBLKLEN_Msk (U(0x3) << HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Pos)
#define HS_URB_SDMMC_CORECFG_1_MAXBLKLEN(value)   \
	(HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_1_BASECLKFREQ_Pos))
#define HS_URB_SDMMC_CORECFG_1_SDIF_8BIT_Pos  18
#define HS_URB_SDMMC_CORECFG_1_SDIF_8BIT      (U(0x1) << HS_URB_SDMMC_CORECFG_1_SDIF_8BIT_Pos)
#define HS_URB_SDMMC_CORECFG_1_ADMA2_Pos      19
#define HS_URB_SDMMC_CORECFG_1_ADMA2	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_ADMA2_Pos)
#define HS_URB_SDMMC_CORECFG_1_HISPEED_Pos    21
#define HS_URB_SDMMC_CORECFG_1_HISPEED	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_HISPEED_Pos)
#define HS_URB_SDMMC_CORECFG_1_SDMA_Pos	      22
#define HS_URB_SDMMC_CORECFG_1_SDMA	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_SDMA_Pos)
#define HS_URB_SDMMC_CORECFG_1_SUSPRES_Pos    23
#define HS_URB_SDMMC_CORECFG_1_SUSPRES	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_SUSPRES_Pos)
#define HS_URB_SDMMC_CORECFG_1_3V3_Pos	      24
#define HS_URB_SDMMC_CORECFG_1_3V3	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_3V3_Pos)
#define HS_URB_SDMMC_CORECFG_1_3V0_Pos	      25
#define HS_URB_SDMMC_CORECFG_1_3V0	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_3V0_Pos)
#define HS_URB_SDMMC_CORECFG_1_1V8_Pos	      26
#define HS_URB_SDMMC_CORECFG_1_1V8	      (U(0x1) << HS_URB_SDMMC_CORECFG_1_1V8_Pos)
#define HS_URB_SDMMC_CORECFG_1_ADDR64BIT_Pos  28
#define HS_URB_SDMMC_CORECFG_1_ADDR64BIT      (U(0x1) << HS_URB_SDMMC_CORECFG_1_ADDR64BIT_Pos)
#define HS_URB_SDMMC_CORECFG_1_ASYNCCHINT_Pos 29
#define HS_URB_SDMMC_CORECFG_1_ASYNCCHINT     (U(0x1) << HS_URB_SDMMC_CORECFG_1_ASYNCCHINT_Pos)
#define HS_URB_SDMMC_CORECFG_1_SLOTTYPE_Pos   30
#define HS_URB_SDMMC_CORECFG_1_SLOTTYPE_Msk   (U(0x3) << HS_URB_SDMMC_CORECFG_1_SLOTTYPE_Pos)
#define HS_URB_SDMMC_CORECFG_1_SLOTTYPE(value) \
	(HS_URB_SDMMC_CORECFG_1_SLOTTYPE_Msk & ((value) << HS_URB_SDMMC_CORECFG_1_SLOTTYPE_Pos))
#define HS_URB_SDMMC_CORECFG_1_MASK U(0xF7EFFFBF)

/*  HS_URB_SDMMC_CORECFG_2: (R/W 32) SDMMCn Core Configuration 2 Register */
/* bit:  0..12  initpresetval       */
/* bit: 13..15  Reserved            */
/* bit: 16..28  dspdpresetval       */
/* bit: 29..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_2_OFFSET	  0x044
#define HS_URB_SDMMC1_CORECFG_2_OFFSET	  0x080
#define HS_URB_SDMMC_CORECFG_2_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_CORECFG_2_INITPRESETVAL_Pos 0
#define HS_URB_SDMMC_CORECFG_2_INITPRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_2_INITPRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_2_INITPRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_2_INITPRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_2_INITPRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL_Pos 16
#define HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_2_DSPDPRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_2_MASK U(0x1FFF1FFF)

/*  HS_URB_SDMMC_CORECFG_3: (R/W 32) SDMMCn Core Configuration 3 Register */
/* bit:  0..12  hspdpresetval       */
/* bit: 13..15  Reserved            */
/* bit: 16..28  sdr12presetval      */
/* bit: 29..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_3_OFFSET	  0x048
#define HS_URB_SDMMC1_CORECFG_3_OFFSET	  0x084
#define HS_URB_SDMMC_CORECFG_3_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL_Pos 0
#define HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_3_HSPDPRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL_Pos 16
#define HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_3_SDR12PRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_3_MASK U(0x1FFF1FFF)

/*  HS_URB_SDMMC_CORECFG_4: (R/W 32) SDMMCn Core Configuration 4 Register */
/* bit:  0..12  sdr25presetval      */
/* bit: 13..15  Reserved            */
/* bit: 16..28  sdr50presetval      */
/* bit: 29..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_4_OFFSET	  0x04C
#define HS_URB_SDMMC1_CORECFG_4_OFFSET	  0x088
#define HS_URB_SDMMC_CORECFG_4_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL_Pos 0
#define HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_4_SDR25PRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL_Pos 16
#define HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_4_SDR50PRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_4_MASK U(0x1FFF1FFF)

/*  HS_URB_SDMMC_CORECFG_5: (R/W 32) SDMMCn Core Configuration 5 Register */
/* bit:  0..12  sdr104presetval     */
/* bit: 13..15  Reserved            */
/* bit: 16..28  ddr50presetval      */
/* bit: 29..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_5_OFFSET	  0x050
#define HS_URB_SDMMC1_CORECFG_5_OFFSET	  0x08C
#define HS_URB_SDMMC_CORECFG_5_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL_Pos 0
#define HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_5_SDR104PRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL_Pos 16
#define HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL_Msk \
	(U(0x1FFF) << HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL_Pos)
#define HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL(value) \
	(HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL_Msk & \
	 ((value) << HS_URB_SDMMC_CORECFG_5_DDR50PRESETVAL_Pos))
#define HS_URB_SDMMC_CORECFG_5_MASK U(0x1FFF1FFF)

/*  HS_URB_SDMMC_CORECFG_6: (R/W 32) SDMMCn Core Configuration 6 Register */
/* bit:  0.. 7  maxcurrent3p3v      */
/* bit:  8..15  maxcurrent3p0v      */
/* bit: 16..23  maxcurrent1p8v      */
/* bit: 24..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_6_OFFSET	  0x054
#define HS_URB_SDMMC1_CORECFG_6_OFFSET	  0x090
#define HS_URB_SDMMC_CORECFG_6_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V3_Pos 0
#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V3_Msk (U(0xFF) << HS_URB_SDMMC_CORECFG_6_MAXCURR3V3_Pos)
#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V3(value) \
	(HS_URB_SDMMC_CORECFG_6_MAXCURR3V3_Msk & ((value) << HS_URB_SDMMC_CORECFG_6_MAXCURR3V3_Pos))
#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V0_Pos 8
#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V0_Msk (U(0xFF) << HS_URB_SDMMC_CORECFG_6_MAXCURR3V0_Pos)
#define HS_URB_SDMMC_CORECFG_6_MAXCURR3V0(value) \
	(HS_URB_SDMMC_CORECFG_6_MAXCURR3V0_Msk & ((value) << HS_URB_SDMMC_CORECFG_6_MAXCURR3V0_Pos))
#define HS_URB_SDMMC_CORECFG_6_MAXCURR1V8_Pos 16
#define HS_URB_SDMMC_CORECFG_6_MAXCURR1V8_Msk (U(0xFF) << HS_URB_SDMMC_CORECFG_6_MAXCURR1V8_Pos)
#define HS_URB_SDMMC_CORECFG_6_MAXCURR1V8(value) \
	(HS_URB_SDMMC_CORECFG_6_MAXCURR1V8_Msk & ((value) << HS_URB_SDMMC_CORECFG_6_MAXCURR1V8_Pos))
#define HS_URB_SDMMC_CORECFG_6_MASK U(0x00FFFFFF)

/*  HS_URB_SDMMC_CORECFG_7: (R/W 32) SDMMCn Core Configuration 7 Register */
/* bit:  0.. 4  itapdlysel          */
/* bit:      5  itapdlyena          */
/* bit:      6  itapchgwin          */
/* bit:      7  Reserved            */
/* bit:  8..11  otapdlysel          */
/* bit:     12  otapdlyena          */
/* bit: 13..18  tuningcount         */
/* bit:     19  asyncwkupena        */
/* bit: 20..31  Reserved            */
#define HS_URB_SDMMC0_CORECFG_7_OFFSET	   0x058
#define HS_URB_SDMMC0_CORECFG_7_RESETVALUE U(0x000C0000)
#define HS_URB_SDMMC1_CORECFG_7_OFFSET	   0x094
#define HS_URB_SDMMC1_CORECFG_7_RESETVALUE U(0x00040000)

#define HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL_Pos 0
#define HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL_Msk (U(0x1F) << HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL_Pos)
#define HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL(value) \
	(HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL_Msk & ((value) << HS_URB_SDMMC_CORECFG_7_ITAPDLYSEL_Pos))
#define HS_URB_SDMMC_CORECFG_7_ITAPDLYENA_Pos 5
#define HS_URB_SDMMC_CORECFG_7_ITAPDLYENA     (U(0x1) << HS_URB_SDMMC_CORECFG_7_ITAPDLYENA_Pos)
#define HS_URB_SDMMC_CORECFG_7_ITAPCHGWIN_Pos 6
#define HS_URB_SDMMC_CORECFG_7_ITAPCHGWIN     (U(0x1) << HS_URB_SDMMC_CORECFG_7_ITAPCHGWIN_Pos)
#define HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL_Pos 8
#define HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL_Msk (U(0xF) << HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL_Pos)
#define HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL(value) \
	(HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL_Msk & ((value) << HS_URB_SDMMC_CORECFG_7_OTAPDLYSEL_Pos))
#define HS_URB_SDMMC_CORECFG_7_OTAPDLYENA_Pos 12
#define HS_URB_SDMMC_CORECFG_7_OTAPDLYENA     (U(0x1) << HS_URB_SDMMC_CORECFG_7_OTAPDLYENA_Pos)
#define HS_URB_SDMMC_CORECFG_7_TUNCNT_Pos     13
#define HS_URB_SDMMC_CORECFG_7_TUNCNT_Msk     (U(0x3F) << HS_URB_SDMMC_CORECFG_7_TUNCNT_Pos)
#define HS_URB_SDMMC_CORECFG_7_TUNCNT(value) \
	(HS_URB_SDMMC_CORECFG_7_TUNCNT_Msk & ((value) << HS_URB_SDMMC_CORECFG_7_TUNCNT_Pos))
#define HS_URB_SDMMC_CORECFG_7_ASYNCWKUPENA_Pos 19
#define HS_URB_SDMMC_CORECFG_7_ASYNCWKUPENA	(U(0x1) << HS_URB_SDMMC_CORECFG_7_ASYNCWKUPENA_Pos)
#define HS_URB_SDMMC_CORECFG_7_MASK		U(0x000FFF7F)

/*  HS_URB_SDMMC_SDHC_DBG0: (R/  32) SDMMCn SDHC Debug 0 Register */
/* bit:  0..15  tundebugbus         */
/* bit: 16..31  txddebugbus         */
#define HS_URB_SDMMC0_SDHC_DBG0_OFFSET	  0x05C
#define HS_URB_SDMMC1_SDHC_DBG0_OFFSET	  0x098
#define HS_URB_SDMMC_SDHC_DBG0_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS_Pos 0
#define HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS_Pos)
#define HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS(value) \
	(HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG0_TUNDBGBUS_Pos))
#define HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS_Pos 16
#define HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS_Pos)
#define HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS(value) \
	(HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG0_TXDDBGBUS_Pos))
#define HS_URB_SDMMC_SDHC_DBG0_MASK U(0xFFFFFFFF)

/*  HS_URB_SDMMC_SDHC_DBG1: (R/  32) SDMMCn SDHC Debug 1 Register */
/* bit:  0..15  rxddebugbus0        */
/* bit: 16..31  rxddebugbus1        */
#define HS_URB_SDMMC0_SDHC_DBG1_OFFSET	  0x060
#define HS_URB_SDMMC1_SDHC_DBG1_OFFSET	  0x09C
#define HS_URB_SDMMC_SDHC_DBG1_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0_Pos 0
#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0_Pos)
#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0(value) \
	(HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS0_Pos))
#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1_Pos 16
#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1_Pos)
#define HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1(value) \
	(HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG1_RXDDBGBUS1_Pos))
#define HS_URB_SDMMC_SDHC_DBG1_MASK U(0xFFFFFFFF)

/*  HS_URB_SDMMC_SDHC_DBG2: (R/  32) SDMMCn SDHC Debug 2 Register */
/* bit:  0..15  cmddebugbus         */
/* bit: 16..31  dmadebugbus         */
#define HS_URB_SDMMC0_SDHC_DBG2_OFFSET	  0x064
#define HS_URB_SDMMC1_SDHC_DBG2_OFFSET	  0x0A0
#define HS_URB_SDMMC_SDHC_DBG2_RESETVALUE U(0x00000000)

#define HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS_Pos 0
#define HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS_Pos)
#define HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS(value) \
	(HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG2_CMDDBGBUS_Pos))
#define HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS_Pos 16
#define HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS_Msk (U(0xFFFF) << HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS_Pos)
#define HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS(value) \
	(HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS_Msk & ((value) << HS_URB_SDMMC_SDHC_DBG2_DMADBGBUS_Pos))
#define HS_URB_SDMMC_SDHC_DBG2_MASK U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG0: (R/  32) PDMA2 Debug 0 Register */
/* bit:  0.. 7  dum_req_dst_region  */
/* bit:  8..15  dum_req_src_region  */
/* bit: 16..19  grant_index_m2      */
/* bit: 20..23  grant_index_m1      */
/* bit:     24  tfr_req_m2          */
/* bit:     25  tfr_req_m1          */
/* bit:     26  granted_m2          */
/* bit:     27  granted_m1          */
/* bit:     28  dma_ctl_en          */
/* bit: 29..31  Reserved            */
#define HS_URB_PDMA2_DBG0_OFFSET     0x0A4
#define HS_URB_PDMA2_DBG0_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG0_MASK	     U(0x1FFFFFFF)

/*  HS_URB_PDMA2_DBG1: (R/  32) PDMA2 Debug 1 Register */
/* bit:  0.. 7  fifo_empty          */
/* bit:  8..15  fifo_half_full      */
/* bit: 16..23  fifo_ready_dst      */
/* bit: 24..31  fifo_ready_src      */
#define HS_URB_PDMA2_DBG1_OFFSET     0x0A8
#define HS_URB_PDMA2_DBG1_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG1_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG2: (R/  32) PDMA2 Debug 2 Register */
/* bit:  0..15  req_mi2             */
/* bit: 16..31  req_mi1             */
#define HS_URB_PDMA2_DBG2_OFFSET     0x0AC
#define HS_URB_PDMA2_DBG2_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG2_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG3: (R/  32) PDMA2 Debug 3 Register */
/* bit:  0.. 7  rd_rawdsttran       */
/* bit:  8..15  rd_rawsrctran       */
/* bit: 16..23  rd_rawblock         */
/* bit: 24..31  rd_rawtfr           */
#define HS_URB_PDMA2_DBG3_OFFSET     0x0B0
#define HS_URB_PDMA2_DBG3_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG3_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG4: (R/  32) PDMA2 Debug 4 Register */
/* bit:  0.. 7  rd_maskerr          */
/* bit:  8..15  rd_int_en           */
/* bit: 16..23  rd_rawerr           */
/* bit: 24..31  Reserved            */
#define HS_URB_PDMA2_DBG4_OFFSET     0x0B4
#define HS_URB_PDMA2_DBG4_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG4_MASK	     U(0x00FFFFFF)

/*  HS_URB_PDMA2_DBG5: (R/  32) PDMA2 Debug 5 Register */
/* bit:  0.. 7  rd_maskdsttran      */
/* bit:  8..15  rd_masksrctran      */
/* bit: 16..23  rd_maskblock        */
/* bit: 24..31  rd_masktfr          */
#define HS_URB_PDMA2_DBG5_OFFSET     0x0B8
#define HS_URB_PDMA2_DBG5_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG5_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG6: (R/  32) PDMA2 Debug 6 Register */
/* bit:  0..16  mask_lck_ch_m2      */
/* bit: 16..31  mask_lck_ch_m1      */
#define HS_URB_PDMA2_DBG6_OFFSET     0x0BC
#define HS_URB_PDMA2_DBG6_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG6_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_DBG7: (R/  32) PDMA2 Debug 7 Register */
/* bit:  0.. 7  en_dst_hs_sgl       */
/* bit:  8..15  en_src_hs_sgl       */
/* bit: 16..20  statusint_dmacore   */
/* bit: 21..30  length_m_i          */
/* bit:     31  Reserved            */
#define HS_URB_PDMA2_DBG7_OFFSET     0x0C0
#define HS_URB_PDMA2_DBG7_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG7_MASK	     U(0x7FFFFFFF)

/*  HS_URB_PDMA2_DBG8: (R/  32) PDMA2 Debug 8 Register */
/* bit:  0.. 7  ch_enable           */
/* bit:  8..15  ch_enable_reg       */
/* bit: 16..31  dma_data_req        */
#define HS_URB_PDMA2_DBG8_OFFSET     0x0C
#define HS_URB_PDMA2_DBG8_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_DBG8_MASK	     U(0xFFFFFFFF)

/*  HS_URB_PDMA2_BAR0: (R/W 32) Base Address 0 AXI PDMA2 */
/* bit:  0.. 7  addr                */
/* bit:  8..31  Reserved            */
#define HS_URB_PDMA2_BAR0_OFFSET     0x0C8
#define HS_URB_PDMA2_BAR0_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_BAR0_MASK	     U(0x000000FF)

/*  HS_URB_PDMA2_BAR1: (R/W 32) Base Address 1 AXI PDMA2 */
/* bit:  0.. 7  addr                */
/* bit:  8..31  Reserved            */
#define HS_URB_PDMA2_BAR1_OFFSET     0x0CC
#define HS_URB_PDMA2_BAR1_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_BAR1_MASK	     U(0x000000FF)

/*  HS_URB_PDMA2_BAR2: (R/W 32) Base Address 2 AXI PDMA2 */
/* bit:  0.. 7  addr                */
/* bit:  8..31  Reserved            */
#define HS_URB_PDMA2_BAR2_OFFSET     0x0D0
#define HS_URB_PDMA2_BAR2_RESETVALUE U(0x00000000)
#define HS_URB_PDMA2_BAR2_MASK	     U(0x000000FF)

/*  HS_URB_PDMA2_HMX_CTR: (R/W 32) HMX Signal Control Register */
/* bit:  0.. 3  xwdecerr            */
/* bit:  4.. 7  xwslverr            */
/* bit:  8..11  en_xwdecerr         */
/* bit: 12..15  en_xwslverr         */
/* bit: 16..19  xwerrclr            */
/* bit: 20..23  nopx                */
/* bit: 24..31  Reserved            */
#define HS_URB_PDMA2_HMX_CTR_OFFSET	0x0D4
#define HS_URB_PDMA2_HMX_CTR_RESETVALUE U(0x00F00000)
#define HS_URB_PDMA2_HMX_CTR_MASK	U(0x00FFFFFF)

/*  HS_URB_USB_CNTR_MISC: (R/W 32) USBn Controller Control Register */
/* bit:  0.. 3  bus_filter_bypass               */
/* bit:      4  host_port_power_control_present */
/* bit:  5..31  Reserved                        */
#define HS_URB_USB0_CNTR_MISC_OFFSET	0x0D8
#define HS_URB_USB1_CNTR_MISC_OFFSET	0x10C
#define HS_URB_USB_CNTR_MISC_RESETVALUE U(0x0000001F)
#define HS_URB_USB_CNTR_MISC_MASK	U(0x0000001F)

/*  HS_URB_USB_PHY_CTR: (R/W 32) USBn PHY Control Register */
/* bit:  0.. 8  ssc_ref_clk_sel */
/* bit:  9..11  ssc_range       */
/* bit:     12  ssc_en          */
/* bit:     13  ref_use_pad     */
/* bit:     14  ref_ssp_en      */
/* bit:     15  ref_clkdiv2     */
/* bit:     16  Reserved        */
/* bit: 17..23  mpll_multiplier */
/* bit: 24..29  fsel            */
/* bit: 30..31  Reserved        */
#define HS_URB_USB0_PHY_CTR_OFFSET    0x0DC
#define HS_URB_USB1_PHY_CTR_OFFSET    0x110
#define HS_URB_USB_PHY_CTR_RESETVALUE U(0x02C80000)
#define HS_URB_USB_PHY_CTR_MASK	      U(0x3FFEFFFF)

/*  HS_URB_USB_PHY_PARAMS0: (R/W 32) USBn PHY Parameter 0 Register */
/* bit:  0.. 2  los_bias            */
/* bit:  3.. 4  vdatreftune0        */
/* bit:  5.. 8  txvreftune0         */
/* bit:  9..10  txrisetune0         */
/* bit: 11..12  txrestune0          */
/* bit:     13  txpreemppulsetune0  */
/* bit: 14..15  txpreempamptune0    */
/* bit: 16..17  txhsxvtune0         */
/* bit: 18..21  txfslstune0         */
/* bit: 22..24  sqrxtune0           */
/* bit: 25..27  otgtune0            */
/* bit: 28..30  compdistune0        */
/* bit:     31  Reserved            */
#define HS_URB_USB0_PHY_PARAMS0_OFFSET	  0x0E0
#define HS_URB_USB1_PHY_PARAMS0_OFFSET	  0x114
#define HS_URB_USB_PHY_PARAMS0_RESETVALUE U(0x48CF0D0C)
#define HS_URB_USB_PHY_PARAMS0_MASK	  U(0x7FFFFFFF)

/*  HS_URB_USB_PHY_PARAMS1: (R/W 32) USBn PHY Parameter 1 Register */
/* bit:  0.. 9  pcs_rx_los_mask_val */
/* bit: 10..15  pcs_tx_deemph_3p5db */
/* bit: 16..21  pcs_tx_deemph_6db   */
/* bit: 22..28  pcs_tx_swing_full   */
/* bit: 29..30  tx_vboost_lvl       */
#define HS_URB_USB0_PHY_PARAMS1_OFFSET	  0x0E4
#define HS_URB_USB1_PHY_PARAMS1_OFFSET	  0x118
#define HS_URB_USB_PHY_PARAMS1_RESETVALUE U(0x9FDC7000)
#define HS_URB_USB_PHY_PARAMS1_MASK	  U(0xFFFFFFFF)

/* HS_URB_USB_PHY_CR: (R/W 32) USBn PHY Control Register */
/* bit:  0..15  data_io     */
/* bit:     16  cap_data    */
/* bit:     17  cap_addr    */
/* bit:     18  write       */
/* bit:     19  read        */
/* bit:     20  ack         */
/* bit: 21..31  Reserved    */
#define HS_URB_USB0_PHY_CR_OFFSET    0x0E8
#define HS_URB_USB1_PHY_CR_OFFSET    0x11C
#define HS_URB_USB_PHY_CR_RESETVALUE U(0x00000000)
#define HS_URB_USB_PHY_CR_MASK	     U(0x001FFFFF)

/* HS_URB_USB_PHY_TST: (R/W 32) USBn PHY Test Signal Register */
/* bit:      0  vatestenb           */
/* bit:      1  lane0_ext_pclk_req  */
/* bit:      2  lane0_tx2rx_loopbk  */
/* bit:      3  loopbackenb         */
/* bit:      4  atereset            */
/* bit:      5  test_powerdown_hsp  */
/* bit:      6  test_powerdown_ssp  */
/* bit:      7  idpullup            */
/* bit:  8..31  Reserved            */
#define HS_URB_USB0_PHY_TST_OFFSET    0x0EC
#define HS_URB_USB1_PHY_TST_OFFSET    0x120
#define HS_URB_USB_PHY_TST_RESETVALUE U(0x00000000)
#define HS_URB_USB_PHY_TST_MASK	      U(0x000000FF)

/* HS_URB_USB_PHY_TST_BYPASS: (R/W 32) USBn PHY Test Bypass Register */
/* bit:      0  dpdata          */
/* bit:      1  dmdata          */
/* bit:      2  dpen            */
/* bit:      3  dmen            */
/* bit:      4  sel             */
/* bit:      5  hssquelch       */
/* bit:      6  hsrxdat         */
/* bit:      7  hsxcvrextctl    */
/* bit:  8..31  Reserved        */
#define HS_URB_USB0_PHY_TST_BYPASS_OFFSET    0x0F0
#define HS_URB_USB1_PHY_TST_BYPASS_OFFSET    0x124
#define HS_URB_USB_PHY_TST_BYPASS_RESETVALUE U(0x00000000)
#define HS_URB_USB_PHY_TST_BYPASS_MASK	     U(0x000000FF)

/* HS_URB_USB_PHY_RTUNE: (R/W 32) USBn PHY Re-Tune Register */
/* bit:      0  req         */
/* bit:      1  ack         */
/* bit:  2..31  Reserved    */
#define HS_URB_USB0_PHY_RTUNE_OFFSET	0x0F4
#define HS_URB_USB1_PHY_RTUNE_OFFSET	0x128
#define HS_URB_USB_PHY_RTUNE_RESETVALUE U(0x00000000)
#define HS_URB_USB_PHY_RTUNE_MASK	U(0x00000003)

/* HS_URB_USB_DBG0: (R/  32) USBn Debug 0 Register */
/* bit:  0..31  logic_analyzer_trace_31_0   */
#define HS_URB_USB0_DBG0_OFFSET	   0x0F8
#define HS_URB_USB1_DBG0_OFFSET	   0x12C
#define HS_URB_USB_DBG0_RESETVALUE U(0x00000000)
#define HS_URB_USB_DBG0_MASK	   U(0xFFFFFFFF)

/* HS_URB_USB_DBG1: (R/  32) USBn Debug 1 Register */
/* bit:  0..31  logic_analyzer_trace_63_32  */
#define HS_URB_USB0_DBG1_OFFSET	   0x0FC
#define HS_URB_USB1_DBG1_OFFSET	   0x130
#define HS_URB_USB_DBG1_RESETVALUE U(0x00000000)
#define HS_URB_USB_DBG1_MASK	   U(0xFFFFFFFF)

/* HS_URB_USB_DBG2: (R/  32) USBn Debug 2 Register */
/* bit:  0..31  debug_31_0  */
#define HS_URB_USB0_DBG2_OFFSET	   0x100
#define HS_URB_USB1_DBG2_OFFSET	   0x134
#define HS_URB_USB_DBG2_RESETVALUE U(0x00000000)
#define HS_URB_USB_DBG2_MASK	   U(0xFFFFFFFF)

/* HS_URB_USB_DBG3: (R/  32) USBn Debug 3 Register */
/* bit:  0..31  debug_63_32 */
#define HS_URB_USB0_DBG3_OFFSET	   0x104
#define HS_URB_USB1_DBG3_OFFSET	   0x138
#define HS_URB_USB_DBG3_RESETVALUE U(0x00000000)
#define HS_URB_USB_DBG3_MASK	   U(0xFFFFFFFF)

/* HS_URB_USB_DBG4: (R/  32) USBn Debug 4 Register */
/* bit:  0.. 2  debug_66_64         */
/* bit:  3.. 4  operational_mode    */
/* bit:  5..16  host_current_belt   */
/* bit: 17..19  clk_gate_ctrl       */
/* bit: 20..31  Reserved            */
#define HS_URB_USB0_DBG4_OFFSET	   0x108
#define HS_URB_USB1_DBG4_OFFSET	   0x13C
#define HS_URB_USB_DBG4_RESETVALUE U(0x00000000)
#define HS_URB_USB_DBG4_MASK	   U(0x000FFFFF)

/* HS_URB_EMAC_PADCFG: (R/W 32) EMAC Pad Configuration Register */
/* bit:      0  v18         */
/* bit:  1..31  Reserved    */
#define HS_URB_EMAC_PADCFG_OFFSET     0x140
#define HS_URB_EMAC_PADCFG_RESETVALUE U(0x00000000)
#define HS_URB_EMAC_PADCFG_MASK	      U(0x00000001)

/* HS_URB_EMAC_PADCFG: (R/W 32) EMACn Pad RGMII Configuration Register */
/* bit:      0  en          */
/* bit:  1.. 2  Reserved    */
/* bit:      3  od          */
/* bit:  4..31  Reserved    */
#define HS_URB_EMAC0_PADCFG_OFFSET     0x144
#define HS_URB_EMAC1_PADCFG_OFFSET     0x164
#define HS_URB_EMACn_PADCFG_RESETVALUE U(0x00000000)
#define HS_URB_EMACn_PADCFG_MASK       U(0x00000009)

/* HS_URB_EMAC_TX_PADCFG: (R/W 32) EMACn Pad RGMII TXD & TXCTL Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..31  Reserved    */
#define HS_URB_EMAC0_TX_PADCFG_OFFSET	 0x148
#define HS_URB_EMAC1_TX_PADCFG_OFFSET	 0x168
#define HS_URB_EMAC_TX_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_EMAC_TX_PADCFG_MASK	 U(0x000007FF)

/* HS_URB_EMAC_TXC_PADCFG: (R/W 32) EMACn Pad RGMII TXC Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..31  Reserved    */
#define HS_URB_EMAC0_TXC_PADCFG_OFFSET	  0x14C
#define HS_URB_EMAC1_TXC_PADCFG_OFFSET	  0x16C
#define HS_URB_EMAC_TXC_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_EMAC_TXC_PADCFG_MASK	  U(0x000007FF)

/* HS_URB_EMAC_RX_PADCFG: (R/W 32) EMACn Pad RGMII RXD & RXCTL Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_EMAC0_RX_PADCFG_OFFSET	 0x150
#define HS_URB_EMAC1_RX_PADCFG_OFFSET	 0x170
#define HS_URB_EMAC_RX_PADCFG_RESETVALUE U(0x00000000)
#define HS_URB_EMAC_RX_PADCFG_MASK	 U(0x00008007)

/* HS_URB_EMAC_RXC_PADCFG: (R/W 32) EMACn Pad RGMII RXC Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_EMAC0_RXC_PADCFG_OFFSET	  0x154
#define HS_URB_EMAC1_RXC_PADCFG_OFFSET	  0x174
#define HS_URB_EMAC_RXC_PADCFG_RESETVALUE U(0x00000000)
#define HS_URB_EMAC_RXC_PADCFG_MASK	  U(0x00008007)

/* HS_URB_EMAC_MD_PADCFG: (R/W 32) EMACn Pad RGMII MDIO Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_EMAC0_MD_PADCFG_OFFSET	 0x158
#define HS_URB_EMAC1_MD_PADCFG_OFFSET	 0x178
#define HS_URB_EMAC_MD_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_EMAC_MD_PADCFG_MASK	 U(0x000087FF)

/* HS_URB_EMAC_MDC_PADCFG: (R/W 32) EMACn Pad RGMII MDC Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..31  Reserved    */
#define HS_URB_EMAC0_MDC_PADCFG_OFFSET	  0x15C
#define HS_URB_EMAC1_MDC_PADCFG_OFFSET	  0x17C
#define HS_URB_EMAC_MDC_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_EMAC_MDC_PADCFG_MASK	  U(0x000007FF)

/* HS_URB_EMAC_BAR: (R/W 32) EMACn Base Address AXI */
/* bit:  0.. 7  addr         */
/* bit:  8..31  Reserved    */
#define HS_URB_EMAC0_BAR_OFFSET	   0x160
#define HS_URB_EMAC1_BAR_OFFSET	   0x180
#define HS_URB_EMAC_BAR_RESETVALUE U(0x00000000)
#define HS_URB_EMAC_BAR_MASK	   U(0x000000FF)

/* HS_URB_NAND_PADCFG: (R/W 32) NAND Pad Configuration Register */
/* bit:      0  en              */
/* bit:      1  v18             */
/* bit:      2  cle             */
/* bit:      3  od              */
/* bit:      4  off_io_share    */
/* bit:  5..31  Reserved        */
#define HS_URB_NAND_PADCFG_OFFSET     0x184
#define HS_URB_NAND_PADCFG_RESETVALUE U(0x00000014)
#define HS_URB_NAND_PADCFG_MASK	      U(0x0000001F)

/* HS_URB_NAND_ALE_CLE_PADCFG: (R/W 32) NAND Pad NFC_ALE & NFC_CLE Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_ALE_CLE_PADCFG_OFFSET     0x188
#define HS_URB_NAND_ALE_CLE_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_ALE_CLE_PADCFG_MASK	      U(0x000087FF)

/* HS_URB_NAND_CEN_PADCFG: (R/W 32) NAND Pad NFC_CEN* Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_CEN_PADCFG_OFFSET	  0x18C
#define HS_URB_NAND_CEN_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_CEN_PADCFG_MASK	  U(0x000087FF)

/* HS_URB_NAND_REN_PADCFG: (R/W 32) NAND Pad NFC_REN Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_REN_PADCFG_OFFSET	  0x190
#define HS_URB_NAND_REN_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_REN_PADCFG_MASK	  U(0x000087FF)

/* HS_URB_NAND_WEN_PADCFG: (R/W 32) NAND Pad NFC_WEN Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_WEN_PADCFG_OFFSET	  0x194
#define HS_URB_NAND_WEN_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_WEN_PADCFG_MASK	  U(0x000087FF)

/* HS_URB_NAND_IO_PADCFG: (R/W 32) NAND Pad NFC_IO* Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_IO_PADCFG_OFFSET	 0x198
#define HS_URB_NAND_IO_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_IO_PADCFG_MASK	 U(0x000087FF)

/* HS_URB_NAND_DQS_PADCFG: (R/W 32) NAND Pad NFC_DQS Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_DQS_PADCFG_OFFSET	  0x19C
#define HS_URB_NAND_DQS_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_DQS_PADCFG_MASK	  U(0x000087FF)

/* HS_URB_NAND_RBN_PADCFG: (R/W 32) NAND Pad NFC_RBN* Configuration Register */
/* bit:      0  sus         */
/* bit:      1  pu          */
/* bit:      2  pd          */
/* bit:  3.. 4  sl          */
/* bit:  5..10  ctl         */
/* bit: 11..14  Reserved    */
/* bit:     15  st          */
/* bit: 16..31  Reserved    */
#define HS_URB_NAND_RBN_PADCFG_OFFSET	  0x1A0
#define HS_URB_NAND_RBN_PADCFG_RESETVALUE U(0x00000078)
#define HS_URB_NAND_RBN_PADCFG_MASK	  U(0x000087FF)

/* HS_URB_MISC_PADCFG: (R/W 32) NAND Pad MISC Configuration Register */
/* bit:  0.. 5  ctl             */
/* bit:      6  cle             */
/* bit:      7  v18             */
/* bit:      8  en              */
/* bit:      9  cdn_sus         */
/* bit:     10  cdn_pd          */
/* bit:     11  cdn_pu          */
/* bit:     12  wp_sus          */
/* bit:     13  wp_pd           */
/* bit:     14  wp_pu           */
/* bit:     15  usb_ocn_filt_en */
/* bit: 16..31  Reserved        */
#define HS_URB_MISC_PADCFG_OFFSET     0x1A4
#define HS_URB_MISC_PADCFG_RESETVALUE U(0x0000C843)

#define HS_URB_MISC_PADCFG_CTL_Pos 0
#define HS_URB_MISC_PADCFG_CTL_Msk (U(0x3F) << HS_URB_MISC_PADCFG_CTL_Pos)
#define HS_URB_MISC_PADCFG_CTL(value) \
	(HS_URB_MISC_PADCFG_CTL_Msk & ((value) << HS_URB_MISC_PADCFG_CTL_Pos))
#define HS_URB_MISC_PADCFG_CLE_Pos	   6
#define HS_URB_MISC_PADCFG_CLE		   (U(0x1) << HS_URB_MISC_PADCFG_CLE_Pos)
#define HS_URB_MISC_PADCFG_V18_Pos	   7
#define HS_URB_MISC_PADCFG_V18		   (U(0x1) << HS_URB_MISC_PADCFG_V18_Pos)
#define HS_URB_MISC_PADCFG_EN_Pos	   8
#define HS_URB_MISC_PADCFG_EN		   (U(0x1) << HS_URB_MISC_PADCFG_EN_Pos)
#define HS_URB_MISC_PADCFG_CDN_SUS_Pos	   9
#define HS_URB_MISC_PADCFG_CDN_SUS	   (U(0x1) << HS_URB_MISC_PADCFG_CDN_SUS_Pos)
#define HS_URB_MISC_PADCFG_CDN_PD_Pos	   10
#define HS_URB_MISC_PADCFG_CDN_PD	   (U(0x1) << HS_URB_MISC_PADCFG_CDN_PD_Pos)
#define HS_URB_MISC_PADCFG_CDN_PU_Pos	   11
#define HS_URB_MISC_PADCFG_CDN_PU	   (U(0x1) << HS_URB_MISC_PADCFG_CDN_PU_Pos)
#define HS_URB_MISC_PADCFG_WP_SUS_Pos	   12
#define HS_URB_MISC_PADCFG_WP_SUS	   (U(0x1) << HS_URB_MISC_PADCFG_WP_SUS_Pos)
#define HS_URB_MISC_PADCFG_WP_PD_Pos	   13
#define HS_URB_MISC_PADCFG_WP_PD	   (U(0x1) << HS_URB_MISC_PADCFG_WP_PD_Pos)
#define HS_URB_MISC_PADCFG_WP_PU_Pos	   14
#define HS_URB_MISC_PADCFG_WP_PU	   (U(0x1) << HS_URB_MISC_PADCFG_WP_PU_Pos)
#define HS_URB_MISC_PADCFG_USB_FILT_EN_Pos 15
#define HS_URB_MISC_PADCFG_USB_FILT_EN	   (U(0x1) << HS_URB_MISC_PADCFG_USB_FILT_EN_Pos)

#define HS_URB_MISC_PADCFG_MASK U(0x0000FFFF)

/* HS_URB_DBG_CTR: (R/W 32) Debug Register */
/* bit:      0  usb_attached            */
/* bit:      1  force_dbg_usb_en_to_0   */
/* bit:      2  force_dbg_usb_en_to_1   */
/* bit:  3..31  Reserved                */
#define HS_URB_DBG_CTR_OFFSET	  0x1A8
#define HS_URB_DBG_CTR_RESETVALUE U(0x00000000)
#define HS_URB_DBG_CTR_MASK	  U(0x00000007)

/* HS_URB_DMA_AXCACHE: (R/W 32) AXCACHE DMA Signal Configuration Register */
/* bit:  0.. 3  emac0       */
/* bit:  4.. 7  emac1       */
/* bit:  8..11  sdmmc0      */
/* bit: 12..15  sdmmc1      */
/* bit: 16..19  nand        */
/* bit: 20..23  pdma        */
/* bit: 24..27  dbg_dma     */
/* bit: 28..31  dbg_sysbuf  */
#define HS_URB_DMA_AXCACHE_OFFSET     0x1AC
#define HS_URB_DMA_AXCACHE_RESETVALUE U(0x00000000)
#define HS_URB_DMA_AXCACHE_MASK	      U(0x00000000)

/**
 * @brief The function returns a pointer
 *        to the URB register address
 *        of the HSPERIPH subsystem
 *
 * @return Pointer to the URB register address
 *         of the HSPERIPH subsystem
 */
hs_urb_regs_t *hs_periph_get_urb_registers(void);

#endif /* __HS_URB_H__ */
