// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __HS_PERIPH_H__
#define __HS_PERIPH_H__

#include <libs/utils-def.h>

#define HS_URB_DBG_CTR_MASK GENMASK(2, 0)

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
} hs_urb_regs_t;

hs_urb_regs_t *hs_periph_get_urb_registers(void);

void hsp_refclk_setup(void);

#endif /* __HS_PERIPH_H__ */
