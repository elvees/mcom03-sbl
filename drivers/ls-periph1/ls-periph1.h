// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __LS1_URB_H__
#define __LS1_URB_H__

typedef struct {
	volatile unsigned int pll_cfg;
	volatile unsigned int pll_diag;
	volatile unsigned int i2s_ucg_rstn_ppolicy;
	volatile unsigned int i2s_ucg_rstn_pstatus;
	volatile unsigned int i2s_ucg_ref_clk;
	volatile unsigned int i2c_highspeed_pull_enable;
	volatile unsigned int gpio1_port_a_pad_ctr[8];
	volatile unsigned int gpio1_port_b_pad_ctr[8];
	volatile unsigned int gpio1_port_c_pad_ctr[8];
	volatile unsigned int gpio1_port_d_pad_ctr[8];
	volatile unsigned int gpio1_v18;
} ls1_urb_regs_t;

ls1_urb_regs_t *ls1_periph_get_urb_registers(void);

#endif /* __LS1_URB_H__ */
