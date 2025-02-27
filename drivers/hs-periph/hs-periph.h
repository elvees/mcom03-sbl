// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/utils-def.h>

#define BASE_ADDR_HSP_USB0      0xb0000000
#define BASE_ADDR_HSP_USB1      0xb0100000
#define BASE_ADDR_HSP_EMAC0     0xb0200000
#define BASE_ADDR_HSP_EMAC1     0xb0210000
#define BASE_ADDR_HSP_SDMMC0    0xb0220000
#define BASE_ADDR_HSP_SDMMC1    0xb0230000
#define BASE_ADDR_HSP_NAND      0xb0240000
#define BASE_ADDR_HSP_PDMA      0xb0250000
#define BASE_ADDR_HSP_QSPI1     0xb0260000
#define BASE_ADDR_HSP_URB       0xb0400000
#define BASE_ADDR_HSP_UCG0      0xb0410000
#define BASE_ADDR_HSP_UCG1      0xb0420000
#define BASE_ADDR_HSP_UCG2      0xb0430000
#define BASE_ADDR_HSP_UCG3      0xb0440000
#define BASE_ADDR_HSP_QSPI1_XIP 0x10000000

#define HSP_URB_DBG_CTR_MASK GENMASK(2, 0)

// HS Periph UCG0-UCG3 Channels
#define HSP_UCG0_CHANNEL_CLK_SYS    0
#define HSP_UCG0_CHANNEL_CLK_DMA    1
#define HSP_UCG0_CHANNEL_CLK_CTR    2
#define HSP_UCG0_CHANNEL_CLK_SPRAM0 3
#define HSP_UCG0_CHANNEL_CLK_EMAC0  4
#define HSP_UCG0_CHANNEL_CLK_EMAC1  5
#define HSP_UCG0_CHANNEL_CLK_USB0   6
#define HSP_UCG0_CHANNEL_CLK_USB1   7
#define HSP_UCG0_CHANNEL_CLK_NFC    8
#define HSP_UCG0_CHANNEL_CLK_PDMA2  9
#define HSP_UCG0_CHANNEL_CLK_SDMMC0 10
#define HSP_UCG0_CHANNEL_CLK_SDMMC1 11
#define HSP_UCG0_CHANNEL_CLK_QSPI1  12

#define HSP_UCG1_CHANNEL_XIN_CLK_SDMMC0 0
#define HSP_UCG1_CHANNEL_XIN_CLK_SDMMC1 1
#define HSP_UCG1_CHANNEL_CLK_NFC_FLASH  2
#define HSP_UCG1_CHANNEL_EXT_CLK_QSPI   3
#define HSP_UCG1_CHANNEL_CLK_DBG        4

#define HSP_UCG2_CHANNEL_CLK_EMAC0_1588      0
#define HSP_UCG2_CHANNEL_CLK_EMAC0_RGMII_TXC 1
#define HSP_UCG2_CHANNEL_CLK_EMAC1_1588      2
#define HSP_UCG2_CHANNEL_CLK_EMAC1_RGMII_TXC 3

#define HSP_UCG3_CHANNEL_CLK_USB0_REF_ALT 0
#define HSP_UCG3_CHANNEL_CLK_USB0_SUSPEND 1
#define HSP_UCG3_CHANNEL_CLK_USB1_REF_ALT 2
#define HSP_UCG3_CHANNEL_CLK_USB1_SUSPEND 3

typedef struct {
	volatile unsigned int pll_cfg;
	volatile unsigned int pll_diag;
	volatile unsigned int rst;
	volatile unsigned int refclk;
	volatile unsigned int qspi1_xip_en_req;
	volatile unsigned int qspi1_xip_en_out;
	volatile unsigned int qspi1_xip_cfg;
	volatile unsigned int qspi1_padcfg;
	volatile unsigned int qspi1_ss_padcfg;
	volatile unsigned int qspi1_siso_padcfg;
	volatile unsigned int qspi1_sclk_padcfg;
	struct {
		volatile unsigned int padcfg;
		volatile unsigned int clk_padcfg;
		volatile unsigned int cmd_padcfg;
		volatile unsigned int dat_padcfg;
		volatile unsigned int corecfg_0;
		volatile unsigned int corecfg_1;
		volatile unsigned int corecfg_2;
		volatile unsigned int corecfg_3;
		volatile unsigned int corecfg_4;
		volatile unsigned int corecfg_5;
		volatile unsigned int corecfg_6;
		volatile unsigned int corecfg_7;
		volatile unsigned int sdhc_dbg0;
		volatile unsigned int sdhc_dbg1;
		volatile unsigned int sdhc_dbg2;
	} sdmmc[2];
	volatile unsigned int pdma2_dbg0;
	volatile unsigned int pdma2_dbg1;
	volatile unsigned int pdma2_dbg2;
	volatile unsigned int pdma2_dbg3;
	volatile unsigned int pdma2_dbg4;
	volatile unsigned int pdma2_dbg5;
	volatile unsigned int pdma2_dbg6;
	volatile unsigned int pdma2_dbg7;
	volatile unsigned int pdma2_dbg8;
	volatile unsigned int pdma2_bar0;
	volatile unsigned int pdma2_bar1;
	volatile unsigned int pdma2_bar2;
	volatile unsigned int pdma2_hmx_ctr;
	struct {
		volatile unsigned int cntr_misc;
		volatile unsigned int phy_ctr;
		volatile unsigned int phy_params0;
		volatile unsigned int phy_params1;
		volatile unsigned int phy_cr;
		volatile unsigned int phy_tst;
		volatile unsigned int phy_tst_bypass;
		volatile unsigned int phy_rtune;
		volatile unsigned int dbg0;
		volatile unsigned int dbg1;
		volatile unsigned int dbg2;
		volatile unsigned int dbg3;
		volatile unsigned int dbg4;
	} usb[2];
	volatile unsigned int emac_padcfg;
	struct {
		volatile unsigned int padcfg;
		volatile unsigned int tx_padcfg;
		volatile unsigned int txc_padcfg;
		volatile unsigned int rx_padcfg;
		volatile unsigned int rxc_padcfg;
		volatile unsigned int md_padcfg;
		volatile unsigned int mdc_padcfg;
		volatile unsigned int bar;
	} emac[2];
	volatile unsigned int nand_padcfg;
	volatile unsigned int nand_ale_cle_padcfg;
	volatile unsigned int nand_cen_padcfg;
	volatile unsigned int nand_ren_padcfg;
	volatile unsigned int nand_wen_padcfg;
	volatile unsigned int nand_io_padcfg;
	volatile unsigned int nand_dqs_padcfg;
	volatile unsigned int nand_rbn_padcfg;
	volatile unsigned int misc_padcfg;
	volatile unsigned int dbg_ctr;
	volatile unsigned int dma_axcache;
} hsp_urb_regs_t;

hsp_urb_regs_t *hsp_get_urb_registers(void);

void hsp_refclk_setup(void);
