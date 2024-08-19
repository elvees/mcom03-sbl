// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __LS_PERIPH1_H__
#define __LS_PERIPH1_H__

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
} ls1_urb_regs_t;

enum pad_driver_strength {
	PAD_DRIVER_STREGTH_2mA = 0x1,
	PAD_DRIVER_STREGTH_4mA = 0x3,
	PAD_DRIVER_STREGTH_6mA = 0x7,
	PAD_DRIVER_STREGTH_8mA = 0xF,
	PAD_DRIVER_STREGTH_10mA = 0x1F,
	PAD_DRIVER_STREGTH_12mA = 0x3F
};

ls1_urb_regs_t *ls1_periph_get_urb_registers(void);

#endif /* __LS_PERIPH1_H__ */
