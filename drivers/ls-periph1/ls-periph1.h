// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/utils-def.h>

#define BASE_ADDR_LSP1_PDMA1_BASE   0xa1700000
#define BASE_ADDR_LSP1_I2C0_BASE    0xa1710000
#define BASE_ADDR_LSP1_I2C1_BASE    0xa1720000
#define BASE_ADDR_LSP1_I2C2_BASE    0xa1730000
#define BASE_ADDR_LSP1_I2S0_BASE    0xa1740000
#define BASE_ADDR_LSP1_UART0_BASE   0xa1750000
#define BASE_ADDR_LSP1_SPI1_BASE    0xa1760000
#define BASE_ADDR_LSP1_PWM_BASE     0xa1770000
#define BASE_ADDR_LSP1_GPIO1_BASE   0xa1780000
#define BASE_ADDR_LSP1_TIMERS_BASE  0xa1790000
#define BASE_ADDR_LSP1_WDT1_BASE    0xa17a0000
#define BASE_ADDR_LSP1_UCG_BASE     0xa17c0000
#define BASE_ADDR_LSP1_I2S_UCG_BASE 0xa17d0000
#define BASE_ADDR_LSP1_URB_BASE     0xa17e0000

// LSPeriph1 UCG Channels
#define LSP1_UCG_ALL_CH_MASK GENMASK(9, 0)
#define LSP1_UCG_SYNC_MASK   LSP1_UCG_ALL_CH_MASK
#define LSP1_UCG_CLK_SYS     0
#define LSP1_UCG_CLK_I2C0    1
#define LSP1_UCG_CLK_I2C1    2
#define LSP1_UCG_CLK_I2C2    3
#define LSP1_UCG_CLK_GPIO1   4
#define LSP1_UCG_CLK_SPI1    5
#define LSP1_UCG_CLK_UART0   6
#define LSP1_UCG_CLK_TIMERS  7
#define LSP1_UCG_CLK_PWM     8
#define LSP1_UCG_CLK_WDT1    9

// LSPeriph1 I2S UCG Channels
#define LSP1_I2S_UCG_CLK_I2S0 0

#define LSP1_PADCTR_SUS_MASK BIT(0)
#define LSP1_PADCTR_SL_MASK  GENMASK(4, 3)
#define LSP1_PADCTR_CTL_MASK GENMASK(10, 5)
#define LSP1_PADCTR_E_MASK   BIT(12)

typedef struct {
	volatile unsigned int pll_cfg;
	volatile unsigned int pll_diag;
	volatile unsigned int i2s_ucg_rstn_ppolicy;
	volatile unsigned int i2s_ucg_rstn_pstatus;
	volatile unsigned int i2s_ucg_ref_clk;
	volatile unsigned int i2c_highspeed_pull_enable;
	volatile unsigned int reserved0[2];
	volatile unsigned int gpio1_port_a_pad_ctr[8];
	volatile unsigned int gpio1_port_b_pad_ctr[8];
	volatile unsigned int gpio1_port_c_pad_ctr[8];
	volatile unsigned int gpio1_port_d_pad_ctr[8];
	volatile unsigned int gpio1_v18;
} lsp1_urb_regs_t;

enum pad_driver_strength {
	PAD_DRIVER_STREGTH_2_MILLI_A = 0x1,
	PAD_DRIVER_STREGTH_4_MILLI_A = 0x3,
	PAD_DRIVER_STREGTH_6_MILLI_A = 0x7,
	PAD_DRIVER_STREGTH_8_MILLI_A = 0xF,
	PAD_DRIVER_STREGTH_10_MILLI_A = 0x1F,
	PAD_DRIVER_STREGTH_12_MILLI_A = 0x3F
};

lsp1_urb_regs_t *lsp1_get_urb_registers(void);

int lsp1_pad_cfg(unsigned int port, unsigned int pin, unsigned int value);
