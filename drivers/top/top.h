// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __TOP_H__
#define __TOP_H__

#include <mcom03.h>

#define TOP_ADDR_URB_OFFSET		     (0x00UL)
#define TOP_ADDR_NONSECURETOSECURE_OFFSET    (0x08UL)
#define TOP_ADDR_SDRTOSECURE_OFFSET	     (0x0CUL)
#define TOP_ADDR_TRUSTEDTOSECURE_OFFSET	     (0x10UL)
#define TOP_ADDR_LOW_RANGE_SECURE_CTR_OFFSET (0x14UL)

#define TOP_ADDR_UCG0_OFFSET (0x01000UL)
#define TOP_ADDR_UCG1_OFFSET (0x02000UL)

/**
 * @brief Bit position in NONESECURETOSECURE Register
 *
 */
#define TOP_HSPERIPH_SUBS_SYS_SECURE_POS  (1UL << 0)
#define TOP_CPU_SUBS_GIC500_SECURE_POS	  (1UL << 1)
#define TOP_MEDIA_SUBS_SYS_SECURE_POS	  (1UL << 2)
#define TOP_MEDIA_SUBS_DP0_SECURE_POS	  (1UL << 3)
#define TOP_MEDIA_SUBS_DP1_SECURE_POS	  (1UL << 4)
#define TOP_LSPERIPH0_SUBS_SYS_SECURE_POS (1UL << 5)
#define TOP_LSPERIPH1_SUBS_SYS_SECURE_POS (1UL << 6)
#define TOP_SDR_SUBS_PCIE_LOW_SECURE_POS  (1UL << 7)
#define TOP_NONSECURETOSECURE_MASK	  (0xFFUL)

/**
 * @brief Bit position in SDRTOSECURE Register
 *
 */
#define TOP_SDR_SUBS_SYS0_SDRTRUSTED_POS	(1UL << 0)
#define TOP_SDR_SUBS_PHYPCIE0_SDRTRUSTED_POS	(1UL << 1)
#define TOP_SDR_SUBS_SYS1_SDRTRUSTED_POS	(1UL << 2)
#define TOP_SDR_SUBS_PHYPCIE1_SDRTRUSTED_POS	(1UL << 3)
#define TOP_SDR_SUBS_MAILBOX1_SF_SDRTRUSTED_POS (1UL << 4)
#define TOP_SDR_SUBS_SYS2_SDRTRUSTED_POS	(1UL << 5)
#define TOP_SDR_SUBS_CTRPCIE0_SDRTRUSTED_POS	(1UL << 6)
#define TOP_SDR_SUBS_CTRPCIE1_SDRTRUSTED_POS	(1UL << 7)
#define TOP_SDR_SUBS_MEM_SDRTRUSTED_POS		(1UL << 8)
#define TOP_SDRTOSECURE_MASK			(0x1FFUL)

/**
 * @brief Bit position in TRUSTEDTOSECURE Register
 *
 */
#define TOP_CPU_SUBS_SYS_TRUSTED_POS		 (1UL << 0)
#define TOP_CPU_SUBS_DEBUG_TRUSTED_POS		 (1UL << 1)
#define TOP_MEDIA_SUBS_SYS_TRUSTED_POS		 (1UL << 2)
#define TOP_HSPERIPH_SUBS_SYS_TRUSTED_POS	 (1UL << 3)
#define TOP_LSPERIPH0_SUBS_SYS_TRUSTED_POS	 (1UL << 4)
#define TOP_LSPERIPH1_SUBS_SYS_TRUSTED_POS	 (1UL << 5)
#define TOP_TOP_TRUSTED_POS			 (1UL << 6)
#define TOP_DDR_SUBS_DDR0_TRUSTED_POS		 (1UL << 7)
#define TOP_DDR_SUBS_DDR1_TRUSTED_POS		 (1UL << 8)
#define TOP_DDR_SUBS_SYS_TRUSTED_POS		 (1UL << 9)
#define TOP_SERVICE_SUBS_MAILBOX0_SF_TRUSTED_POS (1UL << 10)
#define TOP_SERVICE_SUBS_SYS_TRUSTED_POS	 (1UL << 11)
#define TOP_SDR_SUBS_PCIE_MID_TRUSTED_POS	 (1UL << 13)
#define TOP_TRUSTEDTOSECURE_MASK		 (0x2FFFUL)

/**
 * @brief Bit position in DDR_LOW_RANGE_SECURE_CTR Register
 *
 */
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_0_POS (1UL << 0)
#define TOP_DDR_LOW_TRUSTED_TO_SECURE_RANGE_1_POS (1UL << 1)
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_0_POS	  (1UL << 2)
#define TOP_DDR_LOW_SDR_TO_SECURE_RANGE_1_POS	  (1UL << 3)
#define TOP_DDR_LOW_RANGE_SECURE_CTR_MASK	  (0x0F)

/**
 * @brief Struct of registers TOP Subsystem
 *
 */
typedef struct {
	volatile unsigned int PLLCFG; /**< 0x00 PLL Configuration Register                */
	volatile unsigned int PLLDIAG; /**< 0x04 PLL Diagnostic Register                   */
	volatile unsigned int
		NONSECURETOSECURE; /**< 0x08 Change Mode Register non-secure to secure */
	volatile unsigned int SDRTOSECURE; /**< 0x0C Change Mode Register sdr to secure        */
	volatile unsigned int TRUSTEDTOSECURE; /**< 0x10 Change Mode Register trusted to secure    */
	volatile unsigned int
		DDR_LOW_RANGE_SECURE_CTR; /**< 0x14 Control Register Secure Mode DDR Low      */
	volatile unsigned int RESERVED0[2]; /**< 0x18, 0x1C Reserved                            */
	volatile unsigned int TRUSTED_ID_0; /**< 0x20 Trusted ID0 Transaction Register          */
	volatile unsigned int TRUSTED_ID_1; /**< 0x24 Trusted ID1 Transaction Register          */
	volatile unsigned int TRUSTED_ID_2; /**< 0x28 Trusted ID2 Transaction Register          */
	volatile unsigned int TRUSTED_ID_3; /**< 0x2C Trusted ID3 Transaction Register          */
	volatile unsigned int TRUSTED_ID_4; /**< 0x30 Trusted ID4 Transaction Register          */
	volatile unsigned int TRUSTED_ID_5; /**< 0x34 Trusted ID5 Transaction Register          */
	volatile unsigned int TRUSTED_ID_6; /**< 0x38 Trusted ID6 Transaction Register          */
	volatile unsigned int TRUSTED_ID_7; /**< 0x3C Trusted ID7 Transaction Register          */
	volatile unsigned int SDR_ID_0; /**< 0x40 SDR ID0 Transaction Register              */
	volatile unsigned int SDR_ID_1; /**< 0x44 SDR ID1 Transaction Register              */
	volatile unsigned int SDR_ID_2; /**< 0x48 SDR ID2 Transaction Register              */
	volatile unsigned int SDR_ID_3; /**< 0x4C SDR ID3 Transaction Register              */
	volatile unsigned int SDR_ID_4; /**< 0x50 SDR ID4 Transaction Register              */
	volatile unsigned int SDR_ID_5; /**< 0x54 SDR ID5 Transaction Register              */
	volatile unsigned int SDR_ID_6; /**< 0x58 SDR ID6 Transaction Register              */
	volatile unsigned int SDR_ID_7; /**< 0x5C SDR ID7 Transaction Register              */
	volatile unsigned int SLOW_AXI_DLOCK; /**< 0x60 IRQ Status Slow AXI DLOCK Register        */
	volatile unsigned int FAST_AXI_DLOCK; /**< 0x64 IRQ Status Fast AXI DLOCK Register        */
	volatile unsigned int COH_AXI_DLOCK; /**< 0x68 IRQ Status Coh AXI DLOCK Register         */
	volatile unsigned int COH_REMAP; /**< 0x6C Coherent Remap Register                   */
	volatile unsigned int MCOM03_VERSION; /**< 0x70 TOP SubSys MCOM03 Version Register        */
	volatile unsigned int CPU_SUBS_VERSION; /**< 0x74 CPU SubSys Version Register               */
	volatile unsigned int DDR_SUBS_VERSION; /**< 0x78 DDR SubSys Version Register               */
	volatile unsigned int
		INTERCONNECT_SUBS_VERSION; /**< 0x7C Interconnect SubSys Version Register      */
	volatile unsigned int SDR_SUBS_VERSION; /**< 0x80 SDR SubSys Version Register               */
	volatile unsigned int
		MEDIA_SUBS_VERSION; /**< 0x84 Media SubSys Version Register             */
	volatile unsigned int
		SERVICE_SUBS_VERSION; /**< 0x88 Service SubSys Version Register           */
	volatile unsigned int
		HSPERIPH_SUBS_VERSION; /**< 0x8C High Speed Periph SubSys Version Register */
	volatile unsigned int
		LSPERIPH0_SUBS_VERSION; /**< 0x90 Low Speed Periph 0 SubSys Version Register*/
	volatile unsigned int
		LSPERIPH1_SUBS_VERSION; /**< 0x94 Low Speed Periph 1 SubSys Version Register*/
} top_urb_regs_t;

top_urb_regs_t *top_get_urb_registers(void);

void top_reset_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask);
void top_set_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask);
void top_reset_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask);
void top_set_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask);
void top_reset_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask);
void top_set_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask);
void top_set_sdr_to_secure(top_urb_regs_t *urb, unsigned int mask);
void top_reset_sdr_to_secure(top_urb_regs_t *urb, unsigned int mask);

#endif /* __TOP_H__ */
