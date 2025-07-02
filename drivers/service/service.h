// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <libs/utils-def.h>

#define BASE_ADDR_SERVICE_URB       0xbf000000
#define BASE_ADDR_SERVICE_UCG1      0xbf020000
#define BASE_ADDR_SERVICE_OTP       0xbf030000
#define BASE_ADDR_SERVICE_SPI_OTP   0xbf040000
#define BASE_ADDR_SERVICE_PVT_CTR   0xbf050000
#define BASE_ADDR_SERVICE_RTC       0xbf060000
#define BASE_ADDR_SERVICE_TRNG      0xbf070000
#define BASE_ADDR_SERVICE_WDT0      0xbf080000
#define BASE_ADDR_SERVICE_I2C4      0xbf090000
#define BASE_ADDR_SERVICE_MFBSP0    0xbf0a1300
#define BASE_ADDR_SERVICE_MFBSP1    0xbf0b1300
#define BASE_ADDR_SERVICE_CRAM      0xbfa00000
#define BASE_ADDR_SERVICE_IOMMU     0xbfd04000
#define BASE_ADDR_RISC0_CSR         0xbfd08000
#define BASE_ADDR_SERVICE_QLIC0     0xbfe00000
#define BASE_ADDR_SERVICE_QSPI0     0xbff00000
#define BASE_ADDR_SERVICE_QSPI0_XIP 0x00000000
#define BASE_ADDR_SERVICE_MAILBOX0  0xbefd0000

// SERVICE UCG1 Channels
#define SERVICE_UCG1_ALL_CH_MASK           GENMASK(15, 0)
#define SERVICE_UCG1_SYNC_MASK             GENMASK(11, 0)
#define SERVICE_UCG1_CHANNEL_CLK_APB       0
#define SERVICE_UCG1_CHANNEL_CLK_CORE      1
#define SERVICE_UCG1_CHANNEL_CLK_QSPI0     2
#define SERVICE_UCG1_CHANNEL_CLK_BPAM      3
#define SERVICE_UCG1_CHANNEL_CLK_RISC0     4
#define SERVICE_UCG1_CHANNEL_CLK_MFBSP0    5
#define SERVICE_UCG1_CHANNEL_CLK_MFBSP1    6
#define SERVICE_UCG1_CHANNEL_CLK_MAILBOX0  7
#define SERVICE_UCG1_CHANNEL_CLK_PVTCTR    8
#define SERVICE_UCG1_CHANNEL_CLK_I2C4      9
#define SERVICE_UCG1_CHANNEL_CLK_TRNG      10
#define SERVICE_UCG1_CHANNEL_CLK_SPIOTP    11
#define SERVICE_UCG1_CHANNEL_CLK_I2C4_EXT  12
#define SERVICE_UCG1_CHANNEL_CLK_QSPI0_EXT 13
#define SERVICE_UCG1_CHANNEL_CLKOUT_EXT    14
#define SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG 15

// Service Subs TOP Clk Gate
#define SERVICE_TOP_CLK_GATE_SERVICE          0
#define SERVICE_TOP_CLK_GATE_MEDIA            1
#define SERVICE_TOP_CLK_GATE_CPU              2
#define SERVICE_TOP_CLK_GATE_SDR              3
#define SERVICE_TOP_CLK_GATE_HSP              4
#define SERVICE_TOP_CLK_GATE_LSP0             5
#define SERVICE_TOP_CLK_GATE_LSP1             6
#define SERVICE_TOP_CLK_GATE_DDR              7
#define SERVICE_TOP_CLK_GATE_TOP_INTERCONNECT 8
#define SERVICE_TOP_CLK_GATE_ALL_CH_MASK      GENMASK(8, 0)

// Service Subs Debug Status
#define SERVICE_DBG_RISC0_DISABLE 0
#define SERVICE_DBG_RISC0_ENABLE  1

#define SERVICE_DBG_SDR_DISABLE  0
#define SERVICE_DBG_RISC1_ENABLE 1
#define SERVICE_DBG_DSP0_ENABLE  2
#define SERVICE_DBG_DSP1_ENABLE  4
#define SERVICE_DBG_ALL_ENABLE   GENMASK(2, 0)

#define SERVICE_DBG_CPU_SP_DISABLE     0
#define SERVICE_DBG_CPU_SPINDEN_ENABLE 1
#define SERVICE_DBG_CPU_SPIDENENABLE   2
#define SERVICE_DBG_CPU_SP_ALL_ENABLE  GENMASK(1, 0)

#define SERVICE_DBG_CPU_S_DISABLE      0
#define SERVICE_DBG_CPU_NIDEN_ENABLE   1
#define SERVICE_DBG_CPU_DBGEN_ENABLE   2
#define SERVICE_DBG_CPU_SUNIDEN_ENABLE 4
#define SERVICE_DBG_CPU_S_ALL_ENABLE   GENMASK(2, 0)

#define SERVICE_DBG_UST_DISABLE           0x00
#define SERVICE_DBG_UST_DDR_ENABLE        0x01
#define SERVICE_DBG_UST_SDR_DISABLE       0x02
#define SERVICE_DBG_UST_BPAM_RISC0_ENABLE 0x04
#define SERVICE_DBG_UST_BPAM_CPU_DISABLE  0x08
#define SERVICE_DBG_UST_ACOM_ENABLE       0x10
#define SERVICE_DBG_UST_USB_JTAG_ENABLE   0x20
#define SERVICE_DBG_UST_DMA_SMB_ENABLE    0x40
#define SERVICE_DBG_UST_VIRT_CONS_ENABLE  0x80
#define SERVICE_DBG_UST_ALL_ENABLE        GENMASK(7, 0)

typedef struct {
	volatile unsigned int cpu_ppolicy;
	volatile unsigned int cpu_pstatus;
	volatile unsigned int sdr_ppolicy;
	volatile unsigned int sdr_pstatus;
	volatile unsigned int media_ppolicy;
	volatile unsigned int media_pstatus;
	volatile unsigned int core_ppolicy;
	volatile unsigned int core_pstatus;
	volatile unsigned int hsperiph_subs_ppolicy;
	volatile unsigned int hsperiph_subs_pstatus;
	volatile unsigned int lsperiph0_subs_ppolicy;
	volatile unsigned int lsperiph0_subs_pstatus;
	volatile unsigned int lsperiph1_subs_ppolicy;
	volatile unsigned int lsperiph1_subs_pstatus;
	volatile unsigned int ddr_subs_ppolicy;
	volatile unsigned int ddr_subs_pstatus;
	volatile unsigned int top_subs_ppolicy;
	volatile unsigned int top_subs_pstatus;
	volatile unsigned int risc0_ppolicy;
	volatile unsigned int risc0_pstatus;
	volatile unsigned int reserved0[1004];
	volatile unsigned int service_subs_pllcnfg;
	volatile unsigned int service_subs_plldiag;
	volatile unsigned int top_clkgate;
	volatile unsigned int reserved1[1021];
	volatile unsigned int boot;
	volatile unsigned int qspi0_xip_en_req;
	volatile unsigned int qspi0_xip_en_out;
	volatile unsigned int qspi0_xip_cfg;
	volatile unsigned int axicore_dlock;
	volatile unsigned int axisys_dlock;
	volatile unsigned int tscount_high;
	volatile unsigned int tscount_low;
	volatile unsigned int extint_cfg;
	volatile unsigned int pads_ren;
	volatile unsigned int reserved2[1014];
	volatile unsigned int fuse0;
	volatile unsigned int fuse1;
	volatile unsigned int otp_mode;
	volatile unsigned int otp_flag;
	volatile unsigned int fuse0_ecc;
	volatile unsigned int fuse1_ecc;
	volatile unsigned int otp_ecc;
	volatile unsigned int reserved3[1];
	volatile unsigned int axicore_internal_priv;
	volatile unsigned int axisys_internal_priv;
	volatile unsigned int reserved4[2];
	volatile unsigned int tp_dbgen;
	volatile unsigned int sdr_dbgen;
	volatile unsigned int sp_dbgen;
	volatile unsigned int s_dbgen;
	volatile unsigned int ust_dbgen;
} service_urb_regs_t;

/**
 * @brief The function turns off RISC0 CPU
 */
void service_disable_risc0_cpu(void);

/**
 * @brief The function turns on CPU subsystem, waits for status register to
 *        report "on"
 */
void service_enable_arm_cpu(void);

/**
 * @brief The function turns off CPU subsystem, waits for status register to
 *        report "off"
 */
void service_disable_arm_cpu(void);

/**
 * @brief The function turns on LS-PERIPH1 subsystem, waits for status register to
 *        report "on"
 */
void service_enable_lsp1(void);

/**
 * @brief The function turns on LS-PERIPH0 subsystem, waits for status register to
 *        report "on"
 */
void service_enable_lsp0(void);

/**
 * @brief The function returns a pointer
 *        to the URB register address
 *        of the Service subsystem
 *
 * @return Pointer to the URB register address
 *         of the Service subsystem
 */
service_urb_regs_t *service_get_urb_registers(void);

/**
 * @brief The function gets frequency of UCG1 service subsystem APB channel
 *
 * @param apb_freq - The pointer to APB channel frequency value
 *
 * @return  0     - Success,
 *         -ENULL - apb_freq param is not provided (NULL pointer)
 */
int service_get_apb_clock(uint32_t *apb_freq);

/**
 * @brief The function gets frequency of UCG1 service subsystem CORE channel
 *
 * @param core_freq - The pointer to CORE channel frequency value
 *
 * @return  0     - Success,
 *         -ENULL - core_freq param is not provided (NULL pointer)
 */
int service_get_core_clock(uint32_t *core_freq);

/**
 * @brief The function sets clock for service subsystem
 *
 * @param ch_mask   - Service subsystem UCG channel mask
 * @param sync_mask - Service subsystem UCG synchronization mask
 *
 * @return  0             - Success,
 *         -ETIMEOUT      - Operation for PLL setup is timed out or
 *                          operation for set UCG channel dividers is timed out
 *         -EINVALIDPARAM - CORE ucg channel frequency is not divisible by
 *                          APB ucg channel frequency
 */
int service_set_clock(uint32_t ch_mask, uint32_t sync_mask);

/**
 * @brief The function checks if subsystem supports power domain functionality.
 *
 * @param id   - Subsystem ID
 *
 * @return  0             - Success,
 *         -ENOTSUPPORTED - Subsystem does not support power domain functionality.
 */
int service_subsystem_pm_check_support(uint32_t id);

/**
 * @brief The function turns on/off subsystem and waits for status register.
 *
 * @param id    - Subsystem ID
 * @param state - Value to PPOLICY register (PP_ON, PP_OFF or PP_WARM_RST)
 *
 * @return  0             - Success,
 *         -ENOTSUPPORTED - Subsystem does not support power domain functionality,
 *         -EINVALIDPARAM - Wrong state value,
 *         -ETIMEOUT      - Timeout of waiting PSTATUS register.
 */
int service_subsystem_set_power(uint32_t id, uint32_t state);
