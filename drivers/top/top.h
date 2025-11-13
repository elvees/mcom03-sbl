// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/utils-def.h>

#define BASE_ADDR_TOP_URB_BASE  0xa1800000
#define BASE_ADDR_TOP_UCG0_BASE 0xa1801000
#define BASE_ADDR_TOP_UCG1_BASE 0xa1802000

// TOP UCG Channels
#define TOP_UCG0_ALL_CH_MASK          GENMASK(7, 0)
#define TOP_UCG0_SYNC_MASK            TOP_UCG0_ALL_CH_MASK
#define TOP_UCG0_CHANNEL_DDR_DP       0
#define TOP_UCG0_CHANNEL_DDR_VPU      1
#define TOP_UCG0_CHANNEL_DDR_GPU      2
#define TOP_UCG0_CHANNEL_DDR_ISP      3
#define TOP_UCG0_CHANNEL_DDR_CPU      4
#define TOP_UCG0_CHANNEL_CPU_ACP      5
#define TOP_UCG0_CHANNEL_DDR_LSP0     6
#define TOP_UCG0_CHANNEL_AXI_COH_COMM 7

#define TOP_UCG1_ALL_CH_MASK           (GENMASK(8, 4) | BIT(2) | BIT(0))
#define TOP_UCG1_SYNC_MASK             TOP_UCG1_ALL_CH_MASK
#define TOP_UCG1_CHANNEL_AXI_SLOW_COMM 0
#define TOP_UCG1_CHANNEL_AXI_FAST_COMM 2
#define TOP_UCG1_CHANNEL_DDR_SDR_DSP   4
#define TOP_UCG1_CHANNEL_DDR_SDR_PCIE  5
#define TOP_UCG1_CHANNEL_DDR_LSP1      6
#define TOP_UCG1_CHANNEL_DDR_SERVICE   7
#define TOP_UCG1_CHANNEL_DDR_HSP       8

// Bit position in NONESECURETOSECURE Register
#define TOP_HSP_SUBS_SYS_SECURE_POS      0
#define TOP_CPU_SUBS_GIC500_SECURE_POS   1
#define TOP_MEDIA_SUBS_SYS_SECURE_POS    2
#define TOP_MEDIA_SUBS_DP0_SECURE_POS    3
#define TOP_MEDIA_SUBS_DP1_SECURE_POS    4
#define TOP_LSP0_SUBS_SYS_SECURE_POS     5
#define TOP_LSP1_SUBS_SYS_SECURE_POS     6
#define TOP_SDR_SUBS_PCIE_LOW_SECURE_POS 7
#define TOP_NONSECURETOSECURE_MASK       GENMASK(7, 0)

// Bit position in SDRTOSECURE Register
#define TOP_SDR_SUBS_SYS0_SDRTRUSTED_POS        0
#define TOP_SDR_SUBS_PHYPCIE0_SDRTRUSTED_POS    1
#define TOP_SDR_SUBS_SYS1_SDRTRUSTED_POS        2
#define TOP_SDR_SUBS_PHYPCIE1_SDRTRUSTED_POS    3
#define TOP_SDR_SUBS_MAILBOX1_SF_SDRTRUSTED_POS 4
#define TOP_SDR_SUBS_SYS2_SDRTRUSTED_POS        5
#define TOP_SDR_SUBS_CTRPCIE0_SDRTRUSTED_POS    6
#define TOP_SDR_SUBS_CTRPCIE1_SDRTRUSTED_POS    7
#define TOP_SDR_SUBS_MEM_SDRTRUSTED_POS         8
#define TOP_SDRTOSECURE_MASK                    GENMASK(8, 0)

// Bit position in TRUSTEDTOSECURE Register
#define TOP_CPU_SUBS_SYS_TRUSTED_POS             0
#define TOP_CPU_SUBS_DEBUG_TRUSTED_POS           1
#define TOP_MEDIA_SUBS_SYS_TRUSTED_POS           2
#define TOP_HSP_SUBS_SYS_TRUSTED_POS             3
#define TOP_LSP0_SUBS_SYS_TRUSTED_POS            4
#define TOP_LSP1_SUBS_SYS_TRUSTED_POS            5
#define TOP_TOP_TRUSTED_POS                      6
#define TOP_DDR_SUBS_DDR0_TRUSTED_POS            7
#define TOP_DDR_SUBS_DDR1_TRUSTED_POS            8
#define TOP_DDR_SUBS_SYS_TRUSTED_POS             9
#define TOP_SERVICE_SUBS_MAILBOX0_SF_TRUSTED_POS 10
#define TOP_SERVICE_SUBS_SYS_TRUSTED_POS         11
#define TOP_SDR_SUBS_PCIE_MID_TRUSTED_POS        13
#define TOP_TRUSTEDTOSECURE_MASK                 (GENMASK(11, 0) | BIT(13))

// Bit position in DDR_LOW_RANGE_SECURE_CTR Register
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_0_POS 0
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_1_POS 1
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_0_POS     2
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_1_POS     3
#define TOP_DDR_LOW_RANGE_SECURE_CTR_MASK         GENMASK(3, 0)

typedef struct {
	volatile unsigned int pllcfg;
	volatile unsigned int plldiag;
	volatile unsigned int nonsecuretosecure;
	volatile unsigned int sdrtosecure;
	volatile unsigned int trustedtosecure;
	volatile unsigned int ddr_low_range_secure_ctr;
	volatile unsigned int reserved0[2];
	volatile unsigned int trusted_id_0;
	volatile unsigned int trusted_id_1;
	volatile unsigned int trusted_id_2;
	volatile unsigned int trusted_id_3;
	volatile unsigned int trusted_id_4;
	volatile unsigned int trusted_id_5;
	volatile unsigned int trusted_id_6;
	volatile unsigned int trusted_id_7;
	volatile unsigned int sdr_id_0;
	volatile unsigned int sdr_id_1;
	volatile unsigned int sdr_id_2;
	volatile unsigned int sdr_id_3;
	volatile unsigned int sdr_id_4;
	volatile unsigned int sdr_id_5;
	volatile unsigned int sdr_id_6;
	volatile unsigned int sdr_id_7;
	volatile unsigned int slow_axi_dlock;
	volatile unsigned int fast_axi_dlock;
	volatile unsigned int coh_axi_dlock;
	volatile unsigned int coh_remap;
	volatile unsigned int mcom03_version;
	volatile unsigned int cpu_subs_version;
	volatile unsigned int ddr_subs_version;
	volatile unsigned int interconnect_subs_version;
	volatile unsigned int sdr_subs_version;
	volatile unsigned int media_subs_version;
	volatile unsigned int service_subs_version;
	volatile unsigned int hsperiph_subs_version;
	volatile unsigned int lsperiph0_subs_version;
	volatile unsigned int lsperiph1_subs_version;
} top_urb_regs_t;

top_urb_regs_t *top_get_urb_registers(void);

int top_reset_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask);
int top_set_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask);
int top_reset_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask);
int top_set_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask);
int top_reset_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask);
int top_set_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask);
int top_set_clock(void);
