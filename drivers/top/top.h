// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __TOP_H__
#define __TOP_H__

#include <libs/utils-def.h>

// @brief Bit position in NONESECURETOSECURE Register
#define TOP_HSPERIPH_SUBS_SYS_SECURE_POS  BIT(0)
#define TOP_CPU_SUBS_GIC500_SECURE_POS    BIT(1)
#define TOP_MEDIA_SUBS_SYS_SECURE_POS     BIT(2)
#define TOP_MEDIA_SUBS_DP0_SECURE_POS     BIT(3)
#define TOP_MEDIA_SUBS_DP1_SECURE_POS     BIT(4)
#define TOP_LSPERIPH0_SUBS_SYS_SECURE_POS BIT(5)
#define TOP_LSPERIPH1_SUBS_SYS_SECURE_POS BIT(6)
#define TOP_SDR_SUBS_PCIE_LOW_SECURE_POS  BIT(7)
#define TOP_NONSECURETOSECURE_MASK        GENMASK(7, 0)

// @brief Bit position in SDRTOSECURE Register
#define TOP_SDR_SUBS_SYS0_SDRTRUSTED_POS        BIT(0)
#define TOP_SDR_SUBS_PHYPCIE0_SDRTRUSTED_POS    BIT(1)
#define TOP_SDR_SUBS_SYS1_SDRTRUSTED_POS        BIT(2)
#define TOP_SDR_SUBS_PHYPCIE1_SDRTRUSTED_POS    BIT(3)
#define TOP_SDR_SUBS_MAILBOX1_SF_SDRTRUSTED_POS BIT(4)
#define TOP_SDR_SUBS_SYS2_SDRTRUSTED_POS        BIT(5)
#define TOP_SDR_SUBS_CTRPCIE0_SDRTRUSTED_POS    BIT(6)
#define TOP_SDR_SUBS_CTRPCIE1_SDRTRUSTED_POS    BIT(7)
#define TOP_SDR_SUBS_MEM_SDRTRUSTED_POS         BIT(8)
#define TOP_SDRTOSECURE_MASK                    GENMASK(8, 0)

// @brief Bit position in TRUSTEDTOSECURE Register
#define TOP_CPU_SUBS_SYS_TRUSTED_POS             BIT(0)
#define TOP_CPU_SUBS_DEBUG_TRUSTED_POS           BIT(1)
#define TOP_MEDIA_SUBS_SYS_TRUSTED_POS           BIT(2)
#define TOP_HSPERIPH_SUBS_SYS_TRUSTED_POS        BIT(3)
#define TOP_LSPERIPH0_SUBS_SYS_TRUSTED_POS       BIT(4)
#define TOP_LSPERIPH1_SUBS_SYS_TRUSTED_POS       BIT(5)
#define TOP_TOP_TRUSTED_POS                      BIT(6)
#define TOP_DDR_SUBS_DDR0_TRUSTED_POS            BIT(7)
#define TOP_DDR_SUBS_DDR1_TRUSTED_POS            BIT(8)
#define TOP_DDR_SUBS_SYS_TRUSTED_POS             BIT(9)
#define TOP_SERVICE_SUBS_MAILBOX0_SF_TRUSTED_POS BIT(10)
#define TOP_SERVICE_SUBS_SYS_TRUSTED_POS         BIT(11)
#define TOP_SDR_SUBS_PCIE_MID_TRUSTED_POS        BIT(13)
#define TOP_TRUSTEDTOSECURE_MASK                 (GENMASK(11, 0) | BIT(13))

// @brief Bit position in DDR_LOW_RANGE_SECURE_CTR Register
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_0_POS BIT(0)
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_1_POS BIT(1)
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_0_POS     BIT(2)
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_1_POS     BIT(3)
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

#endif /* __TOP_H__ */
