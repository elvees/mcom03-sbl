// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

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

void lsp1_i2s_ucg1_rstn(void);
