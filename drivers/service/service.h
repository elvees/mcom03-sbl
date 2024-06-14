// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __SERVICE_URB_H__
#define __SERVICE_URB_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <mcom03.h>
#include <mcom03-errors.h>

// clang-format off
/**
 * @brief Struct of URB Registers Service Subsystem
 *
 */
typedef struct
{
    volatile unsigned int cpu_ppolicy;            /**< \brief offset: 0x0000 (R/W 32)  State Setting Register for cpu_subs and cpu power domain */
    volatile unsigned int cpu_pstatus;            /**< \brief offset: 0x0004 (R/ 32)  Status Register of cpu_subsys and cpu power domain */
    volatile unsigned int sdr_ppolicy;            /**< \brief offset: 0x0008 (R/W 32)  State Setting Register for sdr_subs and sdr power domain */
    volatile unsigned int sdr_pstatus;            /**< \brief offset: 0x000C (R/ 32)  Status Register of sdr_subs and sdr power domain */
    volatile unsigned int media_ppolicy;          /**< \brief offset: 0x0010 (R/W 32)  State Setting Register for media_subs and media power domain */
    volatile unsigned int media_pstatus;          /**< \brief offset: 0x0014 (R/ 32)  Status Register of media_subs and media power domain */
    volatile unsigned int core_ppolicy;           /**< \brief offset: 0x0018 (R/W 32)  State Setting Register for core power domain */
    volatile unsigned int core_pstatus;           /**< \brief offset: 0x001C (R/ 32)  Status Register of core power domain */
    volatile unsigned int hsperiph_subs_ppolicy;  /**< \brief offset: 0x0020 (R/W 32)  State Setting Register for hsperiph_subs */
    volatile unsigned int hsperiph_subs_pstatus;  /**< \brief offset: 0x0024 (R/ 32)  Status Register of hsperiph_subs */
    volatile unsigned int lsperiph0_subs_ppolicy; /**< \brief offset: 0x0028 (R/W 32)  State Setting Register for lsperiph0_subs */
    volatile unsigned int lsperiph0_subs_pstatus; /**< \brief offset: 0x002C (R/ 32)  Status Register of lsperiph0_subs */
    volatile unsigned int lsperiph1_subs_ppolicy; /**< \brief offset: 0x0030 (R/W 32)  State Setting Register for lsperiph1_subs */
    volatile unsigned int lsperiph1_subs_pstatus; /**< \brief offset: 0x0034 (R/ 32)  Status Register of lsperiph1_subs */
    volatile unsigned int ddr_subs_ppolicy;       /**< \brief offset: 0x0038 (R/W 32)  State Setting Register for ddr_subs */
    volatile unsigned int ddr_subs_pstatus;       /**< \brief offset: 0x003C (R/ 32)  Status Register of ddr_subs */
    volatile unsigned int top_subs_ppolicy;       /**< \brief offset: 0x0040 (R/W 32)  State Setting Register for high level top_subs */
    volatile unsigned int top_subs_pstatus;       /**< \brief offset: 0x0044 (R/ 32)  Status Register of high level top_subs */
    volatile unsigned int risc0_ppolicy;          /**< \brief offset: 0x0048 (R/W 32)  State Setting Register for risc0 */
    volatile unsigned int risc0_pstatus;          /**< \brief offset: 0x004C (R/ 32)  Status Register of risc0 */
    volatile unsigned int reserved0[1004];
    volatile unsigned int service_subs_pllcnfg;   /**< \brief offset: 0x1000 (R/W 32)  Control Register PLL subsystem */
    volatile unsigned int service_subs_plldiag;   /**< \brief offset: 0x1004 (R/W 32)  Diagnostics Register PLL subsystem */
    volatile unsigned int top_clkgate;            /**< \brief offset: 0x1008 (R/W 32)  Register for enabling subsystem reference frequencies */
    volatile unsigned int reserved1[1021];
    volatile unsigned int boot;                   /**< \brief offset: 0x2000 (R/ 32)  Boot Setup Register */
    volatile unsigned int qspi0_xip_en_req;       /**< \brief offset: 0x2004 (R/W 32)  XIP Enable Register for QSPI0 */
    volatile unsigned int qspi0_xip_en_out;       /**< \brief offset: 0x2008 (R/ 32)  XIP State Register for QSPI0 */
    volatile unsigned int qspi0_xip_cfg;          /**< \brief offset: 0x200C (R/W 32)  XIP Setup Register for QSPI0 */
    volatile unsigned int axicore_dlock;          /**< \brief offset: 0x2010 (R/ 32)  Debug Register for axicore switch */
    volatile unsigned int axisys_dlock;           /**< \brief offset: 0x2014 (R/ 32)  Debug Register for axisys switch */
    volatile unsigned int tscount_high;           /**< \brief offset: 0x2018 (R/ 32)  Timestamp Register of SoC counter high part */
    volatile unsigned int tscount_low;            /**< \brief offset: 0x201C (R/ 32)  Timestamp Register of SoC counter low part */
    volatile unsigned int extint_cfg;             /**< \brief offset: 0x2020 (R/W 32)  EXTINT Output Setting Register */
    volatile unsigned int pads_ren;               /**< \brief offset: 0x2024 (R/W 32)  Pull-up resistor enable register for subsystem pins */
    volatile unsigned int reserved2[1014];
    volatile unsigned int fuse0;                  /**< \brief offset: 0x3000 (R/ 32)  Fuse 0 Register */
    volatile unsigned int fuse1;                  /**< \brief offset: 0x3004 (R/ 32)  Fuse 1 Register */
    volatile unsigned int otp_mode;               /**< \brief offset: 0x3008 (R/W 32)  OTP Mode Register */
    volatile unsigned int otp_flag;               /**< \brief offset: 0x300C (R/ 32)  OTP Flag Register */
    volatile unsigned int fuse0_ecc;              /**< \brief offset: 0x3010 (R/ 32)  ECC Fuse 0 Register */
    volatile unsigned int fuse1_ecc;              /**< \brief offset: 0x3014 (R/ 32)  ECC Fuse 1 Register */
    volatile unsigned int otp_ecc;                /**< \brief offset: 0x0318 (R/ 32)  ECC OTP Read Data Register */
    volatile unsigned int reserved3[1];
    volatile unsigned int axicore_internal_priv;  /**< \brief offset: 0x3020 (R/W 32)  Access Control (Privilege) Register for internal subsystem components at axicore switch level */
    volatile unsigned int axisys_internal_priv;   /**< \brief offset: 0x3024 (R/W 32)  Access Control (Privilege) Register for internal subsystem components at axisys switch level */
    volatile unsigned int reserved4[2];
    volatile unsigned int tp_dbgen;               /**< \brief offset: 0x3030 (R/W 32)  Debug Access Control Register for DPM_TP */
    volatile unsigned int sdr_dbgen;              /**< \brief offset: 0x3034 (R/W 32)  Debug Access Control Register for DPM_SDR */
    volatile unsigned int sp_dbgen;               /**< \brief offset: 0x3038 (R/W 32)  Debug Access Control Register for DPM_SP */
    volatile unsigned int s_dbgen;                /**< \brief offset: 0x303C (R/W 32)  Debug Access Control Register for DPM_SP, DPM_SU, DPM_NSP, DPM_NSU */
    volatile unsigned int ust_dbgen;              /**< \brief offset: 0x3040 (R/W 32)  Debug Access Control Register for UltraSoC */
} service_urb_regs_t;
// clang-format on

/*register cpu_pstatus       offset     reset */
/* bits: 31:9              -            0   */
/* bit:  8                 memretspt    0   */
/* bits: 7:6               -            0   */
/* bit:  5                 emulated     0   */
/* bits: 4:0               pstatus      0   */
#define SERVICE_PSTATUS(VAL) ((VAL)&0x13F)

/*register service_subs_plldiag      offset:0x1000   reset   */
/* bits: 31:5             reserve                */
/* bit: 4:4               fbslip           0     */
/* bit: 3:3               rfslip           0     */
/* bit: 2:2               fasten           0     */
/* bit: 1:1               ensat            1     */
/* bit: 0:0               test             0     */
#define SERVICE_PLLDIAG_OFFSET     0x1000UL
#define SERVICE_PLLDIAG_MASK       0x1FUL
#define SERVICE_PLLDIAG_RESETVALUE 0x2UL

#define SERVICE_PLLDIAG_TEST_POS      0UL
#define SERVICE_PLLDIAG_TEST_MASK     (0x1UL << SERVICE_PLLDIAG_TEST_POS)
#define SERVICE_PLLDIAG_TEST(val)     (((val)&0x1UL) << SERVICE_PLLDIAG_TEST_POS)
#define SERVICE_PLLDIAG_TEST_GET(reg) (((reg) >> SERVICE_PLLDIAG_TEST_POS) & 0x1UL)

#define SERVICE_PLLDIAG_ENSAT_POS      1UL
#define SERVICE_PLLDIAG_ENSAT_MASK     (0x1UL << SERVICE_PLLDIAG_ENSAT_POS)
#define SERVICE_PLLDIAG_ENSAT(val)     (((val)&0x1UL) << SERVICE_PLLDIAG_ENSAT_POS)
#define SERVICE_PLLDIAG_ENSAT_GET(reg) (((reg) >> SERVICE_PLLDIAG_ENSAT_POS) & 0x1UL)

#define SERVICE_PLLDIAG_FASTEN_POS      2UL
#define SERVICE_PLLDIAG_FASTEN_MASK     (0x1UL << SERVICE_PLLDIAG_FASTEN_POS)
#define SERVICE_PLLDIAG_FASTEN(val)     (((val)&0x1UL) << SERVICE_PLLDIAG_FASTEN_POS)
#define SERVICE_PLLDIAG_FASTEN_GET(reg) (((reg) >> SERVICE_PLLDIAG_FASTEN_POS) & 0x1UL)

#define SERVICE_PLLDIAG_RFSLIP_POS      3UL
#define SERVICE_PLLDIAG_RFSLIP_MASK     (0x1UL << SERVICE_PLLDIAG_RFSLIP_POS)
#define SERVICE_PLLDIAG_RFSLIP(val)     (((val)&0x1UL) << SERVICE_PLLDIAG_RFSLIP_POS)
#define SERVICE_PLLDIAG_RFSLIP_GET(reg) (((reg) >> SERVICE_PLLDIAG_RFSLIP_POS) & 0x1UL)

#define SERVICE_PLLDIAG_FBSLIP_POS      4UL
#define SERVICE_PLLDIAG_FBSLIP_MASK     (0x1UL << SERVICE_PLLDIAG_FBSLIP_POS)
#define SERVICE_PLLDIAG_FBSLIP(val)     (((val)&0x1UL) << SERVICE_PLLDIAG_FBSLIP_POS)
#define SERVICE_PLLDIAG_FBSLIP_GET(reg) (((reg) >> SERVICE_PLLDIAG_FBSLIP_POS) & 0x1UL)

/* register top_clkgate  offset 0x1008 (R/W 32)  Register for enabling subsystem reference frequencies */
/* 31:9    reserve                  */
/* 8       top interconnect   1     */
/* 7       ddr                0     */
/* 6       lsperiph1          0     */
/* 5       lsperiph0          0     */
/* 4       hsperiph           1     */
/* 3       sdr                0     */
/* 2       cpu                0     */
/* 1       media              0     */
/* 0       service            1     */
#define SERVICE_TOPCLKGATE_OFFSET     0x1008UL
#define SERVICE_TOPCLKGATE_MASK       0x1FFUL
#define SERVICE_TOPCLKGATE_RESETVALUE 0x111UL

#define SERVICE_TOPCLKGATE_SERVICE_POS      0UL
#define SERVICE_TOPCLKGATE_SERVICE_MASK     (0x1UL << SERVICE_TOPCLKGATE_SERVICE_POS)
#define SERVICE_TOPCLKGATE_SERVICE(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_SERVICE_POS)
#define SERVICE_TOPCLKGATE_SERVICE_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_SERVICE_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_MEDIA_POS      1UL
#define SERVICE_TOPCLKGATE_MEDIA_MASK     (0x1UL << SERVICE_TOPCLKGATE_MEDIA_POS)
#define SERVICE_TOPCLKGATE_MEDIA(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_MEDIA_POS)
#define SERVICE_TOPCLKGATE_MEDIA_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_MEDIA_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_CPU_POS      2UL
#define SERVICE_TOPCLKGATE_CPU_MASK     (0x1UL << SERVICE_TOPCLKGATE_CPU_POS)
#define SERVICE_TOPCLKGATE_CPU(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_CPU_POS)
#define SERVICE_TOPCLKGATE_CPU_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_CPU_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_SDR_POS      3UL
#define SERVICE_TOPCLKGATE_SDR_MASK     (0x1UL << SERVICE_TOPCLKGATE_SDR_POS)
#define SERVICE_TOPCLKGATE_SDR(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_SDR_POS)
#define SERVICE_TOPCLKGATE_SDR_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_SDR_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_HSPERIPH_POS      4UL
#define SERVICE_TOPCLKGATE_HSPERIPH_MASK     (0x1UL << SERVICE_TOPCLKGATE_HSPERIPH_POS)
#define SERVICE_TOPCLKGATE_HSPERIPH(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_HSPERIPH_POS)
#define SERVICE_TOPCLKGATE_HSPERIPH_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_HSPERIPH_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_LSPERIPH0_POS      5UL
#define SERVICE_TOPCLKGATE_LSPERIPH0_MASK     (0x1UL << SERVICE_TOPCLKGATE_LSPERIPH0_POS)
#define SERVICE_TOPCLKGATE_LSPERIPH0(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_LSPERIPH0_POS)
#define SERVICE_TOPCLKGATE_LSPERIPH0_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_LSPERIPH0_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_LSPERIPH1_POS      6UL
#define SERVICE_TOPCLKGATE_LSPERIPH1_MASK     (0x1UL << SERVICE_TOPCLKGATE_LSPERIPH1_POS)
#define SERVICE_TOPCLKGATE_LSPERIPH1(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_LSPERIPH1_POS)
#define SERVICE_TOPCLKGATE_LSPERIPH1_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_LSPERIPH1_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_DDR_POS      7UL
#define SERVICE_TOPCLKGATE_DDR_MASK     (0x1UL << SERVICE_TOPCLKGATE_DDR_POS)
#define SERVICE_TOPCLKGATE_DDR(val)     (((val)&0x1UL) << SERVICE_TOPCLKGATE_DDR_POS)
#define SERVICE_TOPCLKGATE_DDR_GET(reg) (((reg) >> SERVICE_TOPCLKGATE_DDR_POS) & 0x1UL)

#define SERVICE_TOPCLKGATE_INTERCONNECT_POS  8UL
#define SERVICE_TOPCLKGATE_INTERCONNECT_MASK (0x1UL << SERVICE_TOPCLKGATE_INTERCONNECT_POS)
#define SERVICE_TOPCLKGATE_INTERCONNECT(val) (((val)&0x1UL) << SERVICE_TOPCLKGATE_INTERCONNECT_POS)
#define SERVICE_TOPCLKGATE_INTERCONNECT_GET(reg) \
	(((reg) >> SERVICE_TOPCLKGATE_INTERCONNECT_POS) & 0x1UL)

/*register   boot                     offset 0x2000 (R/ 32)  Boot Setup Register */
/* bits: 31:4              -               0         */
/* bits: 3                 bs_en           0         */
/* bits: 2:0               boot            0         */
#define SERVICE_BOOT_OFFSET     0x2000UL
#define SERVICE_BOOT_MASK       0x7UL
#define SERVICE_BOOT_RESETVALUE 0UL

#define SERVICE_BOOT_BOOT_POS      0UL
#define SERVICE_BOOT_BOOT_MASK     (3 << SERVICE_BOOT_BOOT_POS)
#define SERVICE_BOOT_BOOT_GET(reg) (((reg) >> SERVICE_BOOT_BOOT_POS) & 3)

#define SERVICE_BOOT_BS_EN_POS      3UL
#define SERVICE_BOOT_BS_EN_MASK     (0x1UL << SERVICE_BOOT_BS_EN_POS)
#define SERVICE_BOOT_BS_EN_GET(reg) (((reg) >> SERVICE_BOOT_BS_EN_POS) & 0x1UL)

/* register xip_en_req         offset 0x2004 (R/W 32)  XIP Enable Register for QSPI0 */
/* bits: 31:1            reserve            0           */
/* bits: 0               en                 1           */ // To enable XIP mode, set the bit to 1
#define SERVICE_XIP_EN_OFFSET     0x2004UL
#define SERVICE_XIP_EN_MASK       1UL
#define SERVICE_XIP_EN_RESETVALUE 1UL

#define SERVICE_XIP_EN_REQ_POS      0UL
#define SERVICE_XIP_EN_REQ_MASK     (0x1UL << SERVICE_XIP_EN_REQ_POS)
#define SERVICE_XIP_EN_REQ(val)     (((val)&0x1UL) << SERVICE_XIP_EN_REQ_POS)
#define SERVICE_XIP_EN_REQ_GET(reg) (((reg) >> SERVICE_XIP_EN_REQ_POS) & 0x1UL)

// register xip_en_out                offset 0x2008 (R/ 32)  XIP State Register for QSPI0 */
/* bits: 31:1              reserve                  0    */
/* bits: 0                 status                   0    */ //  XIP mode status, 0 - mode off, 1 - mode on
#define SERVICE_XIP_EN_OUT_OFFSET     0x2008UL
#define SERVICE_XIP_EN_OUT_MASK       1UL
#define SERVICE_XIP_EN_OUT_RESETVALUE 0UL

#define SERVICE_XIP_EN_OUT_STATUS_POS      0UL
#define SERVICE_XIP_EN_OUT_STATUS_MASK     (0x1UL << SERVICE_XIP_EN_OUT_STATUS_POS)
#define SERVICE_XIP_EN_OUT_STATUS_GET(reg) (((reg) >> SERVICE_XIP_EN_OUT_STATUS_POS) & 0x1UL)

/* register xip_cfg                      offset 0x200C (R/W 32)  XIP Setup Register for QSPI0 */
/* bits 31                -                     0       */
/* bits 30                hpen                  0       */
/* bits 29                le32                  1       */
/* bits 28                addr4                 0       */
/* bits 27                cpol                  0       */
/* bits 26                cpha                  0       */
/* bits 25:24             hp_end_dummy          0       */
/* bits 23:20             dummy_cycles          0       */
/* bits 19:16             ssen                  1       */
/* bits 15:8              hp_mode               0       */
/* bits 7:0               cmd                   8'h03   */
#define SERVICE_XIP_CFG_OFFSET     0x200CUL
#define SERVICE_XIP_CFG_MASK       0x7fffffffUL
#define SERVICE_XIP_CFG_RESETVALUE 0x20010008UL

#define SERVICE_XIP_CFG_CMD_POS      0UL
#define SERVICE_XIP_CFG_CMD_MASK     (0xffUL << SERVICE_XIP_CFG_CMD_POS)
#define SERVICE_XIP_CFG_CMD(val)     (((val)&0xffUL) << SERVICE_XIP_CFG_CMD_POS)
#define SERVICE_XIP_CFG_CMD_GET(reg) (((reg) >> SERVICE_XIP_CFG_CMD_POS) & 0xffUL)

#define SERVICE_XIP_CFG_HP_MODE_POS      8UL
#define SERVICE_XIP_CFG_HP_MODE_MASK     (0xffUL << SERVICE_XIP_CFG_HP_MODE_POS)
#define SERVICE_XIP_CFG_HP_MODE(val)     (((val)&0xffUL) << SERVICE_XIP_CFG_HP_MODE_POS)
#define SERVICE_XIP_CFG_HP_MODE_GET(reg) (((reg) >> SERVICE_XIP_CFG_HP_MODE_POS) & 0xffUL)

#define SERVICE_XIP_CFG_SSEN_POS      16UL
#define SERVICE_XIP_CFG_SSEN_MASK     (0xfUL << SERVICE_XIP_CFG_SSEN_POS)
#define SERVICE_XIP_CFG_SSEN(val)     (((val)&0xfUL) << SERVICE_XIP_CFG_SSEN_POS)
#define SERVICE_XIP_CFG_SSEN_GET(reg) (((reg) >> SERVICE_XIP_CFG_SSEN_POS) & 0xfUL)

#define SERVICE_XIP_CFG_DUMMY_CYCLES_POS      20UL
#define SERVICE_XIP_CFG_DUMMY_CYCLES_MASK     (0x3UL << SERVICE_XIP_CFG_DUMMY_CYCLES_POS)
#define SERVICE_XIP_CFG_DUMMY_CYCLES(val)     (((val)&0x3UL) << SERVICE_XIP_CFG_DUMMY_CYCLES_POS)
#define SERVICE_XIP_CFG_DUMMY_CYCLES_GET(reg) (((reg) >> SERVICE_XIP_CFG_DUMMY_CYCLES_POS) & 0x3UL)

#define SERVICE_XIP_CFG_HP_END_DUMMY_POS      24UL
#define SERVICE_XIP_CFG_HP_END_DUMMY_MASK     (0x3UL << SERVICE_XIP_CFG_HP_END_DUMMY_POS)
#define SERVICE_XIP_CFG_HP_END_DUMMY(val)     (((val)&0x3UL) << SERVICE_XIP_CFG_HP_END_DUMMY_POS)
#define SERVICE_XIP_CFG_HP_END_DUMMY_GET(reg) (((reg) >> SERVICE_XIP_CFG_HP_END_DUMMY_POS) & 0x3UL)

#define SERVICE_XIP_CFG_CPHA_POS      26UL
#define SERVICE_XIP_CFG_CPHA_MASK     (0x1UL << SERVICE_XIP_CFG_CPHA_POS)
#define SERVICE_XIP_CFG_CPHA(val)     (((val)&0x1UL) << SERVICE_XIP_CFG_CPHA_POS)
#define SERVICE_XIP_CFG_CPHA_GET(reg) (((reg) >> SERVICE_XIP_CFG_CPHA_POS) & 0x1UL)

#define SERVICE_XIP_CFG_CPOL_POS      27UL
#define SERVICE_XIP_CFG_CPOL_MASK     (0x1UL << SERVICE_XIP_CFG_CPOL_POS)
#define SERVICE_XIP_CFG_CPOL(val)     (((val)&0x1UL) << SERVICE_XIP_CFG_CPOL_POS)
#define SERVICE_XIP_CFG_CPOL_GET(reg) (((reg) >> SERVICE_XIP_CFG_CPOL_POS) & 0x1UL)

#define SERVICE_XIP_CFG_ADDR4_POS      28UL
#define SERVICE_XIP_CFG_ADDR4_MASK     (0x1UL << SERVICE_XIP_CFG_ADDR4_POS)
#define SERVICE_XIP_CFG_ADDR4(val)     (((val)&0x1UL) << SERVICE_XIP_CFG_ADDR4_POS)
#define SERVICE_XIP_CFG_ADDR4_GET(reg) (((reg) >> SERVICE_XIP_CFG_ADDR4_POS) & 0x1UL)

#define SERVICE_XIP_CFG_LE32_POS      29UL
#define SERVICE_XIP_CFG_LE32_MASK     (0x1UL << SERVICE_XIP_CFG_LE32_POS)
#define SERVICE_XIP_CFG_LE32(val)     (((val)&0x1UL) << SERVICE_XIP_CFG_LE32_POS)
#define SERVICE_XIP_CFG_LE32_GET(reg) (((reg) >> SERVICE_XIP_CFG_LE32_POS) & 0x1UL)

#define SERVICE_XIP_CFG_HPEN_POS      30UL
#define SERVICE_XIP_CFG_HPEN_MASK     (0x1UL << SERVICE_XIP_CFG_HPEN_POS)
#define SERVICE_XIP_CFG_HPEN(val)     (((val)&0x1UL) << SERVICE_XIP_CFG_HPEN_POS)
#define SERVICE_XIP_CFG_HPEN_GET(reg) (((reg) >> SERVICE_XIP_CFG_HPEN_POS) & 0x1UL)

/* register axicore_dlock     offset 0x2010 (R/ 32)  Debug Register for axicore switch */
/* bits 31:7              -                 0     */
/* bits 6                 dlock_id          0     */ // Shows the id for the transaction that caused the switch to hang
/* bits 5:4               dlock_mst         0     */ // Shows the master number for the transaction that caused the switch to hang
/* bits 3:1               dlock_slv         0     */ // Shows the slave number for the transaction that caused the switch to hang
/* bits 0                 dlock_wr          0     */ // Shows writing or reading caused the switch to hang
#define SERVICE_AXICORE_OFFSET     0x2010UL
#define SERVICE_AXICORE_MASK       0x7FUL
#define SERVICE_AXICORE_RESETVALUE 0UL

#define SERVICE_AXICORE_DLOCK_ID_POS      6UL
#define SERVICE_AXICORE_DLOCK_ID_MASK     (0x1UL << SERVICE_AXICORE_DLOCK_ID_POS)
#define SERVICE_AXICORE_DLOCK_ID(val)     (((val)&0x1UL) << SERVICE_AXICORE_DLOCK_ID_POS)
#define SERVICE_AXICORE_DLOCK_ID_GET(reg) (((reg) >> SERVICE_AXICORE_DLOCK_ID_POS) & 0x1UL)

#define SERVICE_AXICORE_DLOCK_MST_POS      4UL
#define SERVICE_AXICORE_DLOCK_MST_MASK     (0x3UL << SERVICE_AXICORE_DLOCK_MST_POS)
#define SERVICE_AXICORE_DLOCK_MST(val)     (((val)&0x3UL) << SERVICE_AXICORE_DLOCK_MST_POS)
#define SERVICE_AXICORE_DLOCK_MST_GET(reg) (((reg) >> SERVICE_AXICORE_DLOCK_MST_POS) & 0x3UL)

#define SERVICE_AXICORE_DLOCK_SLV_POS      1UL
#define SERVICE_AXICORE_DLOCK_SLV_MASK     (0x7UL << SERVICE_AXICORE_DLOCK_SLV_POS)
#define SERVICE_AXICORE_DLOCK_SLV(val)     (((val)&0x7UL) << SERVICE_AXICORE_DLOCK_SLV_POS)
#define SERVICE_AXICORE_DLOCK_SLV_GET(reg) (((reg) >> SERVICE_AXICORE_DLOCK_SLV_POS) & 0x7UL)

#define SERVICE_AXICORE_DLOCK_WR_POS      0UL
#define SERVICE_AXICORE_DLOCK_WR_MASK     (0x1UL << SERVICE_AXICORE_DLOCK_WR_POS)
#define SERVICE_AXICORE_DLOCK_WR(val)     (((val)&0x1UL) << SERVICE_AXICORE_DLOCK_WR_POS)
#define SERVICE_AXICORE_DLOCK_WR_GET(reg) (((reg) >> SERVICE_AXICORE_DLOCK_WR_POS) & 0x1UL)

/* register axisys_dlock             offset 0x2014 (R/ 32)  Debug Register for axisys switch */
/* bits 31:8              -                  0   */
/* bits 7:5               dlock_id           0   */ //Shows the id for the transaction that caused the switch to hang
/* bits 4                 dlock_mst          0   */ //Shows the master number for the transaction that caused the switch to hang
/* bits 3:1               dlock_slv          0   */ //Shows the slave number for the transaction that caused the switch to hang
/* bits 0                 dlock_wr           0   */ //Shows writing or reading caused the switch to hang
#define SERVICE_AXISYS_OFFSET     0x2014UL
#define SERVICE_AXISYS_MASK       0xFFUL
#define SERVICE_AXISYS_RESETVALUE 0UL

#define SERVICE_AXISYS_DLOCK_ID_POS      5UL
#define SERVICE_AXISYS_DLOCK_ID_MASK     (0x7UL << SERVICE_AXISYS_DLOCK_ID_POS)
#define SERVICE_AXISYS_DLOCK_ID(val)     (((val)&0x7UL) << SERVICE_AXISYS_DLOCK_ID_POS)
#define SERVICE_AXISYS_DLOCK_ID_GET(reg) (((reg) >> SERVICE_AXISYS_DLOCK_ID_POS) & 0x7UL)

#define SERVICE_AXISYS_DLOCK_MST_POS      4UL
#define SERVICE_AXISYS_DLOCK_MST_MASK     (0x1UL << SERVICE_AXISYS_DLOCK_MST_POS)
#define SERVICE_AXISYS_DLOCK_MST(val)     (((val)&0x1UL) << SERVICE_AXISYS_DLOCK_MST_POS)
#define SERVICE_AXISYS_DLOCK_MST_GET(reg) (((reg) >> SERVICE_AXISYS_DLOCK_MST_POS) & 0x1UL)

#define SERVICE_AXISYS_DLOCK_SLV_POS      1UL
#define SERVICE_AXISYS_DLOCK_SLV_MASK     (0x7UL << SERVICE_AXISYS_DLOCK_SLV_POS)
#define SERVICE_AXISYS_DLOCK_SLV(val)     (((val)&0x7UL) << SERVICE_AXISYS_DLOCK_SLV_POS)
#define SERVICE_AXISYS_DLOCK_SLV_GET(reg) (((reg) >> SERVICE_AXISYS_DLOCK_SLV_POS) & 0x7UL)

#define SERVICE_AXISYS_DLOCK_WR_POS      0UL
#define SERVICE_AXISYS_DLOCK_WR_MASK     (0x1UL << SERVICE_AXISYS_DLOCK_WR_POS)
#define SERVICE_AXISYS_DLOCK_WR(val)     (((val)&0x1UL) << SERVICE_AXISYS_DLOCK_WR_POS)
#define SERVICE_AXISYS_DLOCK_WR_GET(reg) (((reg) >> SERVICE_AXISYS_DLOCK_WR_POS) & 0x1UL)

/* register extint_cfg    offset: 0x2020 (R/W 32)  EXTINT Output Setting Register */
/* bits: 31:2                res0             ?  */
/* bits: 1:0                 outen            0  */
// The field bits control the direction of the corresponding EXTINT [1: 0] pins of the microcircuit, 0 - the pin operates in the input mode, 1 - in the output mode
#define SERVICE_EXTINT_CFG_OFFSET     0x2020UL
#define SERVICE_EXTINT_CFG_MASK       0x3UL
#define SERVICE_EXTINT_CFG_RESETVALUE 0UL

#define SERVICE_EXTINT_CFG_OUTEN_POS      0UL
#define SERVICE_EXTINT_CFG_OUTEN_MASK     (0x3UL << SERVICE_EXTINT_CFG_OUTEN_POS)
#define SERVICE_EXTINT_CFG_OUTEN(val)     (((val)&0x3UL) << SERVICE_EXTINT_CFG_OUTEN_POS)
#define SERVICE_EXTINT_CFG_OUTEN_GET(reg) (((reg) >> SERVICE_EXTINT_CFG_OUTEN_POS) & 0x3UL)

/* register pads_ren     offset: 0x2024 (R/W 32)  Pull-up resistor enable register for subsystem pins */
/* bits 31:5                reserve              0  */ //    1'b0 - pull-up is on; 1'b1 - pull-up is off
/* bits 4                   mfbsp0_ren           0  */ //    1'b0 - pull-up is on; 1'b1 - pull-up is off
/* bits 3                   mfbsp1_ren           0  */ //    1'b0 - pull-up is on; 1'b1 - pull-up is off
/* bits 2                   i2c4_ren             0  */ //    1'b0 - pull-up is on; 1'b1 - pull-up is off
/* bits 1                   qspi0_ren            0  */ //    1'b0 - pull-up is on; 1'b1 - pull-up is off
/* bits 0                   sysio_ren            0  */
#define SERVICE_PADS_REN_OFFSET     0x2024UL
#define SERVICE_PADS_REN_MASK       0x1FUL
#define SERVICE_PADS_REN_RESETVALUE 0UL

#define SERVICE_PADS_REN_MFBSP0_REN_POS      4UL
#define SERVICE_PADS_REN_MFBSP0_REN_MASK     (0x1UL << SERVICE_PADS_REN_MFBSP0_REN_POS)
#define SERVICE_PADS_REN_MFBSP0_REN(val)     (((val)&0x1UL) << SERVICE_PADS_REN_MFBSP0_REN_POS)
#define SERVICE_PADS_REN_MFBSP0_REN_GET(reg) (((reg) >> SERVICE_PADS_REN_MFBSP0_REN_POS) & 0x1UL)

#define SERVICE_PADS_REN_MFBSP1_REN_POS      3UL
#define SERVICE_PADS_REN_MFBSP1_REN_MASK     (0x1UL << SERVICE_PADS_REN_MFBSP1_REN_POS)
#define SERVICE_PADS_REN_MFBSP1_REN(val)     (((val)&0x1UL) << SERVICE_PADS_REN_MFBSP1_REN_POS)
#define SERVICE_PADS_REN_MFBSP1_REN_GET(reg) (((reg) >> SERVICE_PADS_REN_MFBSP1_REN_POS) & 0x1UL)

#define SERVICE_PADS_REN_I2C4_REN_POS      2UL
#define SERVICE_PADS_REN_I2C4_REN_MASK     (0x1UL << SERVICE_PADS_REN_I2C4_REN_POS)
#define SERVICE_PADS_REN_I2C4_REN(val)     (((val)&0x1UL) << SERVICE_PADS_REN_I2C4_REN_POS)
#define SERVICE_PADS_REN_I2C4_REN_GET(reg) (((reg) >> SERVICE_PADS_REN_I2C4_REN_POS) & 0x1UL)

#define SERVICE_PADS_REN_QSPI0_REN_POS      1UL
#define SERVICE_PADS_REN_QSPI0_REN_MASK     (0x1UL << SERVICE_PADS_REN_QSPI0_REN_POS)
#define SERVICE_PADS_REN_QSPI0_REN(val)     (((val)&0x1UL) << SERVICE_PADS_REN_QSPI0_REN_POS)
#define SERVICE_PADS_REN_QSPI0_REN_GET(reg) (((reg) >> SERVICE_PADS_REN_QSPI0_REN_POS) & 0x1UL)

#define SERVICE_PADS_REN_SYSIO_REN_POS      0UL
#define SERVICE_PADS_REN_SYSIO_REN_MASK     (0x1UL << SERVICE_PADS_REN_SYSIO_REN_POS)
#define SERVICE_PADS_REN_SYSIO_REN(val)     (((val)&0x1UL) << SERVICE_PADS_REN_SYSIO_REN_POS)
#define SERVICE_PADS_REN_SYSIO_REN_GET(reg) (((reg) >> SERVICE_PADS_REN_SYSIO_REN_POS) & 0x1UL)

/* register fuse0                 offset 0x3000 (R/ 32)  Fuse 0 Register */
/* bit 31           lock                       0     */ // Setting the bit disables overwriting fuse0
/* bit 30           -                          0     */ // reserve
/* bit 29           -                          0     */ // reserve
/* bit 28           -                          0     */ // reserve
/* bit 27           top_subs_disable           0     */ // Disables upper level switches
/* bit 26           ddr_subs_disable           0     */ // Disables the ddr subsystem
/* bit 25           ls1_subs_disable           0     */ // Disables the lsperiph1 subsystem
/* bit 24           ls0_subs_disable           0     */ // Disables the lsperiph0 subsystem
/* bit 23           hs_subs_disable            0     */ // Disables the hsperiph subsystem
/* bit 22           media_subs_disable         0     */ // Disables the media subsystem
/* bit 21           sdr_subs_disable           0     */ // Disables sdr subsystem
/* bit 20           cpu_subs_disable           0     */ // Disables the cpu subsystem
/* bit 19           scan_test_disable          0     */ // Disables DFT scan testing
/* bit 18           mbist_test_disable         0     */ // Disables MBIST testing
/* bit 17           bsr_test_disable           0     */ // Disables BSR testing
/* bit 16           ust_debug_disable          0     */ // Disables the ability to debug via UltraSoC
/* bit 15           bringupdbg_disable         0     */ // Setting to 1 disables RISC0 JTAG debugging
/* bit 14           sdrtosecure_disable        0     */ // Setting to 1 prevents changing access rights from sdr to secure. After burning this bit, the securable flag becomes inapplicable to areas with trusted access (sdr)
/* bit 13           trustedtosecure_disable    0     */ // Setting to 1 prohibits changing access rights from trusted to secure. After burning this bit, the securable flag becomes inapplicable to areas with trusted access
/* bit 12           dpm_enable                 0     */ // Setting to 1 enables checking mechanism for all debug DPMs when dpm_lock == 1
/* bit 11           -                          0     */ // reserve
/* bit 10           dpm_lock_secure            0     */ // Setting to 1 disables the unlock operation for DPM_SP debug access protection mechanisms
/* bit 9            dpm_lock_sdr               0     */ // Setting to 1 disables unlock operation for DPM_SDR debug access protection mechanisms
/* bit 8            dpm_lock_trusted           0     */ // Setting to 1 disables unlock operation for DPM_TP debug access protection mechanisms
/* bit 7            bs_en_padoverride          0     */ // Setting to 1 sets vs_en to be set from the vs_en bit of the register, not from the external BS_EN pin of the microcircuit.
/* bit 6            bs_en                      0     */ // Setting to 1 specifies the need to verify the boot loader image at boot time
/* bit 5            vs_en_padoverride          0     */ // Setting to 1 sets the vs_en setting from the vs_en register bit, not from the external VS_EN pin of the microcircuit
/* bit 4            vs_en                      0     */ // Setting to 1 equalizes non_secure, secure, trusted and sdr access levels
/* bit 3            boot_padsoverride          0     */ // Setting to 1 sets the boot settings from the boot0-2 bits, and not from the external BOOT pins of the microcircuit
/* bit 2            boot2                      0     */ // Specifies the source for initial loading of the microcircuit
/* bit 1            boot1                      0     */ // Specifies the source for initial loading of the microcircuit
/* bit 0            boot0                      0     */ // Specifies the source for initial loading of the microcircuit
#define SERVICE_FUSE0_OFFSET     0x3000UL
#define SERVICE_FUSE0_MASK       0x8FFF3FFFUL
#define SERVICE_FUSE0_RESETVALUE 0UL

#define SERVICE_FUSE0_LOCK_POS      31UL
#define SERVICE_FUSE0_LOCK_MASK     (0x1UL << SERVICE_FUSE0_LOCK_POS)
#define SERVICE_FUSE0_LOCK(val)     (((val)&0x1UL) << SERVICE_FUSE0_LOCK_POS)
#define SERVICE_FUSE0_LOCK_GET(reg) (((reg) >> SERVICE_FUSE0_LOCK_POS) & 0x1UL)

#define SERVICE_FUSE0_TOP_SUBS_DISABLE_POS  27UL
#define SERVICE_FUSE0_TOP_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_TOP_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_TOP_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_TOP_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_TOP_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_TOP_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_DDR_SUBS_DISABLE_POS  26UL
#define SERVICE_FUSE0_DDR_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_DDR_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_DDR_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_DDR_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_DDR_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_DDR_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_LS1_SUBS_DISABLE_POS  25UL
#define SERVICE_FUSE0_LS1_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_LS1_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_LS1_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_LS1_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_LS1_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_LS1_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_LS0_SUBS_DISABLE_POS  24UL
#define SERVICE_FUSE0_LS0_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_LS0_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_LS0_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_LS0_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_LS0_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_LS0_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_HS_SUBS_DISABLE_POS  23UL
#define SERVICE_FUSE0_HS_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_HS_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_HS_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_HS_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_HS_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_HS_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_MEDIA_SUBS_DISABLE_POS  22UL
#define SERVICE_FUSE0_MEDIA_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_MEDIA_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_MEDIA_SUBS_DISABLE(val) \
	(((val)&0x1UL) << SERVICE_FUSE0_MEDIA_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_MEDIA_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_MEDIA_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_SDR_SUBS_DISABLE_POS  21UL
#define SERVICE_FUSE0_SDR_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_SDR_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_SDR_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_SDR_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_SDR_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_SDR_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_CPU_SUBS_DISABLE_POS  20UL
#define SERVICE_FUSE0_CPU_SUBS_DISABLE_MASK (0x1UL << SERVICE_FUSE0_CPU_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_CPU_SUBS_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_CPU_SUBS_DISABLE_POS)
#define SERVICE_FUSE0_CPU_SUBS_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_CPU_SUBS_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_SCAN_TEST_DISABLE_POS  19UL
#define SERVICE_FUSE0_SCAN_TEST_DISABLE_MASK (0x1UL << SERVICE_FUSE0_SCAN_TEST_DISABLE_POS)
#define SERVICE_FUSE0_SCAN_TEST_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_SCAN_TEST_DISABLE_POS)
#define SERVICE_FUSE0_SCAN_TEST_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_SCAN_TEST_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_MBIST_TEST_DISABLE_POS  18UL
#define SERVICE_FUSE0_MBIST_TEST_DISABLE_MASK (0x1UL << SERVICE_FUSE0_MBIST_TEST_DISABLE_POS)
#define SERVICE_FUSE0_MBIST_TEST_DISABLE(val) \
	(((val)&0x1UL) << SERVICE_FUSE0_MBIST_TEST_DISABLE_POS)
#define SERVICE_FUSE0_MBIST_TEST_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_MBIST_TEST_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_BSR_TEST_DISABLE_POS  17UL
#define SERVICE_FUSE0_BSR_TEST_DISABLE_MASK (0x1UL << SERVICE_FUSE0_BSR_TEST_DISABLE_POS)
#define SERVICE_FUSE0_BSR_TEST_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_BSR_TEST_DISABLE_POS)
#define SERVICE_FUSE0_BSR_TEST_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_BSR_TEST_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_UST_DEBUG_DISABLE_POS  16UL
#define SERVICE_FUSE0_UST_DEBUG_DISABLE_MASK (0x1UL << SERVICE_FUSE0_UST_DEBUG_DISABLE_POS)
#define SERVICE_FUSE0_UST_DEBUG_DISABLE(val) (((val)&0x1UL) << SERVICE_FUSE0_UST_DEBUG_DISABLE_POS)
#define SERVICE_FUSE0_UST_DEBUG_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_UST_DEBUG_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_BRINGUPDBG_DISABLE_POS  15UL
#define SERVICE_FUSE0_BRINGUPDBG_DISABLE_MASK (0x1UL << SERVICE_FUSE0_BRINGUPDBG_DISABLE_POS)
#define SERVICE_FUSE0_BRINGUPDBG_DISABLE(val) \
	(((val)&0x1UL) << SERVICE_FUSE0_BRINGUPDBG_DISABLE_POS)
#define SERVICE_FUSE0_BRINGUPDBG_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_BRINGUPDBG_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_SDRTOSECURE_DISABLE_POS  14UL
#define SERVICE_FUSE0_SDRTOSECURE_DISABLE_MASK (0x1UL << SERVICE_FUSE0_SDRTOSECURE_DISABLE_POS)
#define SERVICE_FUSE0_SDRTOSECURE_DISABLE(val) \
	(((val)&0x1UL) << SERVICE_FUSE0_SDRTOSECURE_DISABLE_POS)
#define SERVICE_FUSE0_SDRTOSECURE_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_SDRTOSECURE_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_POS 13UL
#define SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_MASK \
	(0x1UL << SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_POS)
#define SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE(val) \
	(((val)&0x1UL) << SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_POS)
#define SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_TRUSTEDTOSECURE_DISABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_DPM_ENABLE_POS      12UL
#define SERVICE_FUSE0_DPM_ENABLE_MASK     (0x1UL << SERVICE_FUSE0_DPM_ENABLE_POS)
#define SERVICE_FUSE0_DPM_ENABLE(val)     (((val)&0x1UL) << SERVICE_FUSE0_DPM_ENABLE_POS)
#define SERVICE_FUSE0_DPM_ENABLE_GET(reg) (((reg) >> SERVICE_FUSE0_DPM_ENABLE_POS) & 0x1UL)

#define SERVICE_FUSE0_RESERVED_POS      11UL
#define SERVICE_FUSE0_RESERVED_MASK     (0x1UL << SERVICE_FUSE0_RESERVED_POS)
#define SERVICE_FUSE0_RESERVED(val)     (((val)&0x1UL) << SERVICE_FUSE0_RESERVED_POS)
#define SERVICE_FUSE0_RESERVED_GET(reg) (((reg) >> SERVICE_FUSE0_RESERVED_POS) & 0x1UL)

#define SERVICE_FUSE0_DPM_LOCK_SECURE_POS  10UL
#define SERVICE_FUSE0_DPM_LOCK_SECURE_MASK (0x1UL << SERVICE_FUSE0_DPM_LOCK_SECURE_POS)
#define SERVICE_FUSE0_DPM_LOCK_SECURE(val) (((val)&0x1UL) << SERVICE_FUSE0_DPM_LOCK_SECURE_POS)
#define SERVICE_FUSE0_DPM_LOCK_SECURE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_DPM_LOCK_SECURE_POS) & 0x1UL)

#define SERVICE_FUSE0_DPM_LOCK_SDR_POS      9UL
#define SERVICE_FUSE0_DPM_LOCK_SDR_MASK     (0x1UL << SERVICE_FUSE0_DPM_LOCK_SDR_POS)
#define SERVICE_FUSE0_DPM_LOCK_SDR(val)     (((val)&0x1UL) << SERVICE_FUSE0_DPM_LOCK_SDR_POS)
#define SERVICE_FUSE0_DPM_LOCK_SDR_GET(reg) (((reg) >> SERVICE_FUSE0_DPM_LOCK_SDR_POS) & 0x1UL)

#define SERVICE_FUSE0_DPM_LOCK_TRUSTED_POS  8UL
#define SERVICE_FUSE0_DPM_LOCK_TRUSTED_MASK (0x1UL << SERVICE_FUSE0_DPM_LOCK_TRUSTED_POS)
#define SERVICE_FUSE0_DPM_LOCK_TRUSTED(val) (((val)&0x1UL) << SERVICE_FUSE0_DPM_LOCK_TRUSTED_POS)
#define SERVICE_FUSE0_DPM_LOCK_TRUSTED_GET(reg) \
	(((reg) >> SERVICE_FUSE0_DPM_LOCK_TRUSTED_POS) & 0x1UL)

#define SERVICE_FUSE0_BS_EN_PADOVERRIDE_POS  7UL
#define SERVICE_FUSE0_BS_EN_PADOVERRIDE_MASK (0x1UL << SERVICE_FUSE0_BS_EN_PADOVERRIDE_POS)
#define SERVICE_FUSE0_BS_EN_PADOVERRIDE(val) (((val)&0x1UL) << SERVICE_FUSE0_BS_EN_PADOVERRIDE_POS)
#define SERVICE_FUSE0_BS_EN_PADOVERRIDE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_BS_EN_PADOVERRIDE_POS) & 0x1UL)

#define SERVICE_FUSE0_BS_EN_POS      6UL
#define SERVICE_FUSE0_BS_EN_MASK     (0x1UL << SERVICE_FUSE0_BS_EN_POS)
#define SERVICE_FUSE0_BS_EN(val)     (((val)&0x1UL) << SERVICE_FUSE0_BS_EN_POS)
#define SERVICE_FUSE0_BS_EN_GET(reg) (((reg) >> SERVICE_FUSE0_BS_EN_POS) & 0x1UL)

#define SERVICE_FUSE0_VS_EN_PADOVERRIDE_POS  5UL
#define SERVICE_FUSE0_VS_EN_PADOVERRIDE_MASK (0x1UL << SERVICE_FUSE0_VS_EN_PADOVERRIDE_POS)
#define SERVICE_FUSE0_VS_EN_PADOVERRIDE(val) (((val)&0x1UL) << SERVICE_FUSE0_VS_EN_PADOVERRIDE_POS)
#define SERVICE_FUSE0_VS_EN_PADOVERRIDE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_VS_EN_PADOVERRIDE_POS) & 0x1UL)

#define SERVICE_FUSE0_VS_EN_POS      4UL
#define SERVICE_FUSE0_VS_EN_MASK     (0x1UL << SERVICE_FUSE0_VS_EN_POS)
#define SERVICE_FUSE0_VS_EN(val)     (((val)&0x1UL) << SERVICE_FUSE0_VS_EN_POS)
#define SERVICE_FUSE0_VS_EN_GET(reg) (((reg) >> SERVICE_FUSE0_VS_EN_POS) & 0x1UL)

#define SERVICE_FUSE0_BOOT_PADSOVERRIDE_POS  3UL
#define SERVICE_FUSE0_BOOT_PADSOVERRIDE_MASK (0x1UL << SERVICE_FUSE0_BOOT_PADSOVERRIDE_POS)
#define SERVICE_FUSE0_BOOT_PADSOVERRIDE(val) (((val)&0x1UL) << SERVICE_FUSE0_BOOT_PADSOVERRIDE_POS)
#define SERVICE_FUSE0_BOOT_PADSOVERRIDE_GET(reg) \
	(((reg) >> SERVICE_FUSE0_BOOT_PADSOVERRIDE_POS) & 0x1UL)

#define SERVICE_FUSE0_BOOT_POS      0UL
#define SERVICE_FUSE0_BOOT_MASK     (0x7UL << SERVICE_FUSE0_BOOT_POS)
#define SERVICE_FUSE0_BOOT(val)     (((val)&0x7UL) << SERVICE_FUSE0_BOOT_POS)
#define SERVICE_FUSE0_BOOT_GET(reg) (((reg) >> SERVICE_FUSE0_BOOT_POS) & 0x7UL)

// register fuse1                    offset 0x3004 (R/ 32)  Fuse 1 Register */
/* bits: 31:31                lock                         */ // Setting the bit disables overwriting fuse1
/* bits: 30:30                lock_fw                      */ // Setting the bit blocks the 0xBC-FF OTP memory area from being overwritten
/* bits: 29:29                lock_bootrom                 */ // Setting the bit blocks the area 0x08 - 0x43 OTP memory from being overwritten
/* bits: 28:0                 user                   0     */
#define SERVICE_FUSE1_OFFSET     0x3004UL
#define SERVICE_FUSE1_MASK       0xFFFFFFFFUL
#define SERVICE_FUSE1_RESETVALUE 0UL

#define SERVICE_FUSE1_LOCK_POS      31UL
#define SERVICE_FUSE1_LOCK_MASK     (0x1UL << SERVICE_FUSE1_LOCK_POS)
#define SERVICE_FUSE1_LOCK(val)     (((val)&0x1UL) << SERVICE_FUSE1_LOCK_POS)
#define SERVICE_FUSE1_LOCK_GET(reg) (((reg) >> SERVICE_FUSE1_LOCK_POS) & 0x1UL)

#define SERVICE_FUSE1_LOCK_FW_POS      30UL
#define SERVICE_FUSE1_LOCK_FW_MASK     (0x1UL << SERVICE_FUSE1_LOCK_FW_POS)
#define SERVICE_FUSE1_LOCK_FW(val)     (((val)&0x1UL) << SERVICE_FUSE1_LOCK_FW_POS)
#define SERVICE_FUSE1_LOCK_FW_GET(reg) (((reg) >> SERVICE_FUSE1_LOCK_FW_POS) & 0x1UL)

#define SERVICE_FUSE1_LOCK_BOOTROM_POS      29UL
#define SERVICE_FUSE1_LOCK_BOOTROM_MASK     (0x1UL << SERVICE_FUSE1_LOCK_BOOTROM_POS)
#define SERVICE_FUSE1_LOCK_BOOTROM(val)     (((val)&0x1UL) << SERVICE_FUSE1_LOCK_BOOTROM_POS)
#define SERVICE_FUSE1_LOCK_BOOTROM_GET(reg) (((reg) >> SERVICE_FUSE1_LOCK_BOOTROM_POS) & 0x1UL)

#define SERVICE_FUSE1_USER_POS      0UL
#define SERVICE_FUSE1_USER_MASK     (0x1fffffffUL << SERVICE_FUSE1_USER_POS)
#define SERVICE_FUSE1_USER(val)     (((val)&0x1fffffffUL) << SERVICE_FUSE1_USER_POS)
#define SERVICE_FUSE1_USER_GET(reg) (((reg) >> SERVICE_FUSE1_USER_POS) & 0x1fffffffUL)

// register otp_mode           offset 0x3008 (R/W 32)  OTP Mode Register */
/* bits: 31:3              -                 0  */
/* bits: 2                 pd                1  */ // Cut off the otp power line needed to read it
/* bits: 1                 dctrl             0  */ // Bit allows direct reading from otp via apb bus. The bit value must be changed when spi_otp cs == 0, or when spi_otp cs == 1 and NOP on the data bus
/* bits: 0                 sp                0  */ // Operation mode otp c spi_otp 1 - sequential; 0 - parallel
#define SERVICE_OTP_MODE_OFFSET     0x3008UL
#define SERVICE_OTP_MODE_MASK       0x7UL
#define SERVICE_OTP_MODE_RESETVALUE 0x4UL

#define SERVICE_OTP_MODE_PD_POS      2UL
#define SERVICE_OTP_MODE_PD_MASK     (0x1UL << SERVICE_OTP_MODE_PD_POS)
#define SERVICE_OTP_MODE_PD(val)     (((val)&0x1UL) << SERVICE_OTP_MODE_PD_POS)
#define SERVICE_OTP_MODE_PD_GET(reg) (((reg) >> SERVICE_OTP_MODE_PD_POS) & 0x1UL)

#define SERVICE_OTP_MODE_DCTRL_POS      1UL
#define SERVICE_OTP_MODE_DCTRL_MASK     (0x1UL << SERVICE_OTP_MODE_DCTRL_POS)
#define SERVICE_OTP_MODE_DCTRL(val)     (((val)&0x1UL) << SERVICE_OTP_MODE_DCTRL_POS)
#define SERVICE_OTP_MODE_DCTRL_GET(reg) (((reg) >> SERVICE_OTP_MODE_DCTRL_POS) & 0x1UL)

#define SERVICE_OTP_MODE_SP_POS      0UL
#define SERVICE_OTP_MODE_SP_MASK     (0x1UL << SERVICE_OTP_MODE_SP_POS)
#define SERVICE_OTP_MODE_SP(val)     (((val)&0x1UL) << SERVICE_OTP_MODE_SP_POS)
#define SERVICE_OTP_MODE_SP_GET(reg) (((reg) >> SERVICE_OTP_MODE_SP_POS) & 0x1UL)

// register otp_flag                   offset 0x300C (R/ 32)  OTP Flag Register */
/* bits: 31:2              -                           0        */
/* bits: 1                 boot_done                   0 -> 1   */ // Reflects the status of completion of the process of initialization of the otp memory when resetting and the validity of the fuse0 and fuse1 registers
/* bits: 0                 flag                        0        */ // Reflects the states of the FLAG otp pin
#define SERVICE_OTP_FLAG_OFFSET     0x300CUL
#define SERVICE_OTP_FLAG_MASK       0x3UL
#define SERVICE_OTP_FLAG_RESETVALUE 0UL

#define SERVICE_OTP_FLAG_BOOT_DONE_POS      1UL
#define SERVICE_OTP_FLAG_BOOT_DONE_MASK     (0x1UL << SERVICE_OTP_FLAG_BOOT_DONE_POS)
#define SERVICE_OTP_FLAG_BOOT_DONE(val)     (((val)&0x1UL) << SERVICE_OTP_FLAG_BOOT_DONE_POS)
#define SERVICE_OTP_FLAG_BOOT_DONE_GET(reg) (((reg) >> SERVICE_OTP_FLAG_BOOT_DONE_POS) & 0x1UL)

#define SERVICE_OTP_FLAG_FLAG_POS      0UL
#define SERVICE_OTP_FLAG_FLAG_MASK     (0x1UL << SERVICE_OTP_FLAG_FLAG_POS)
#define SERVICE_OTP_FLAG_FLAG(val)     (((val)&0x1UL) << SERVICE_OTP_FLAG_FLAG_POS)
#define SERVICE_OTP_FLAG_FLAG_GET(reg) (((reg) >> SERVICE_OTP_FLAG_FLAG_POS) & 0x1UL)

//----------------------------------------------------------------------

// register fuse0_ecc            offset 0x3010 (R/ 32)  ECC Fuse 0 Register */
/* bits 31:8              reserve               0     */ //    reserve
/* bits 7:6               brp                   0     */ //    BRP Bits (Bit Repair by Polarity)
/* bits 5:0               ecc                   0     */ //    ECC validation bits
#define SERVICE_FUSE0_ECC_OFFSET     0x3019UL
#define SERVICE_FUSE0_ECC_MASK       0xFFUL
#define SERVICE_FUSE0_ECC_RESETVALUE 0UL

#define SERVICE_FUSE0_ECC_BRP_POS      6UL
#define SERVICE_FUSE0_ECC_BRP_MASK     (0x3UL << SERVICE_FUSE0_ECC_BRP_POS)
#define SERVICE_FUSE0_ECC_BRP(val)     (((val)&0x3UL) << SERVICE_FUSE0_ECC_BRP_POS)
#define SERVICE_FUSE0_ECC_BRP_GET(reg) (((reg) >> SERVICE_FUSE0_ECC_BRP_POS) & 0x3UL)

#define SERVICE_FUSE0_ECC_ECC_POS      0UL
#define SERVICE_FUSE0_ECC_ECC_MASK     (0x3fUL << SERVICE_FUSE0_ECC_ECC_POS)
#define SERVICE_FUSE0_ECC_ECC(val)     (((val)&0x3fUL) << SERVICE_FUSE0_ECC_ECC_POS)
#define SERVICE_FUSE0_ECC_ECC_GET(reg) (((reg) >> SERVICE_FUSE0_ECC_ECC_POS) & 0x3fUL)

// register fuse1_ecc  offset 0x3014 (R/ 32)  ECC Fuse 1 Register */
/* bits 31:8              reserve   0    */ //  reserve
/* bits 7:6               brp       0    */ //  BRP Bits (Bit Repair by Polarity)
/* bits 5:0               ecc       0    */ //  ECC validation bits
#define SERVICE_FUSE1_ECC_OFFSET     0x3014UL
#define SERVICE_FUSE1_ECC_MASK       0xFFUL
#define SERVICE_FUSE1_ECC_RESETVALUE 0UL

#define SERVICE_FUSE1_ECC_BRP_POS      6UL
#define SERVICE_FUSE1_ECC_BRP_MASK     (0x3UL << SERVICE_FUSE1_ECC_BRP_POS)
#define SERVICE_FUSE1_ECC_BRP(val)     (((val)&0x3UL) << SERVICE_FUSE1_ECC_BRP_POS)
#define SERVICE_FUSE1_ECC_BRP_GET(reg) (((reg) >> SERVICE_FUSE1_ECC_BRP_POS) & 0x3UL)

#define SERVICE_FUSE1_ECC_ECC_POS      0UL
#define SERVICE_FUSE1_ECC_ECC_MASK     (0x3fUL << SERVICE_FUSE1_ECC_ECC_POS)
#define SERVICE_FUSE1_ECC_ECC(val)     (((val)&0x3fUL) << SERVICE_FUSE1_ECC_ECC_POS)
#define SERVICE_FUSE1_ECC_ECC_GET(reg) (((reg) >> SERVICE_FUSE1_ECC_ECC_POS) & 0x3fUL)

// register otp_ecc  offset: 0x3018 (R/ 32)  ECC OTP Read Data Register */
/* bits 31:8              reserve     0         reserve                                */
/* bits 7:6               brp                   BRP Bits (Bit Repair by Polarity)      */
/* bits 5:0               ecc         0         ECC validation bits                    */
#define SERVICE_OTP_ECC_OFFSET     0x3018UL
#define SERVICE_OTP_ECC_MASK       0UL
#define SERVICE_OTP_ECC_RESETVALUE 0UL

#define SERVICE_OTP_ECC_BRP_POS      6UL
#define SERVICE_OTP_ECC_BRP_MASK     (0x3UL << SERVICE_OTP_ECC_BRP_POS)
#define SERVICE_OTP_ECC_BRP(val)     (((val)&0x3UL) << SERVICE_OTP_ECC_BRP_POS)
#define SERVICE_OTP_ECC_BRP_GET(reg) (((reg) >> SERVICE_OTP_ECC_BRP_POS) & 0x3UL)

#define SERVICE_OTP_ECC_ECC_POS      0UL
#define SERVICE_OTP_ECC_ECC_MASK     (0x3fUL << SERVICE_OTP_ECC_ECC_POS)
#define SERVICE_OTP_ECC_ECC(val)     (((val)&0x3fUL) << SERVICE_OTP_ECC_ECC_POS)
#define SERVICE_OTP_ECC_ECC_GET(reg) (((reg) >> SERVICE_OTP_ECC_ECC_POS) & 0x3fUL)

// register axicore_internal_priv offset: 0x3020 (R/W 32)  Access Control (Privilege) Register for internal subsystem components at axicore switch level */
/* bits 31:5              res0                 ?             */
/* bits 4:1               privileged           0             */ // If the bit of the field is set to 1, calls to the slave with the bit number are allowed only for secure transactions Default 'b1100
/* bit 0                  reserve              0             */ //
#define SERVICE_AXICORE_INTERNAL_PRIV_OFFSET     0x3020UL
#define SERVICE_AXICORE_INTERNAL_PRIV_MASK       0x1FUL
#define SERVICE_AXICORE_INTERNAL_PRIV_RESETVALUE 0UL

#define SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_POS 1UL
#define SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_MASK \
	(0xfUL << SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_POS)
#define SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED(val) \
	(((val)&0xfUL) << SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_POS)
#define SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_GET(reg) \
	(((reg) >> SERVICE_AXICORE_INTERNAL_PRIV_PRIVILEGED_POS) & 0xfUL)

// register axisys_internal_priv       offset: 0x3024 (R/W 32)  Access Control (Privilege) Register for internal subsystem components at axisys switch level */
/* bits 31:6              res0               ?               */
/* bits 5:1               privileged         0               */ // If the field bit is set to 1, calls to
/* bits 0                 reserve            0               */ // slave with bit number are allowed only for secure transactions Default - 'b11111
#define SERVICE_AXISYS_INTERNAL_PRIV_OFFSET     0x3024UL
#define SERVICE_AXISYS_INTERNAL_PRIV_MASK       0x3FUL
#define SERVICE_AXISYS_INTERNAL_PRIV_RESETVALUE 0UL

#define SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_POS 1UL
#define SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_MASK \
	(0x1fUL << SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_POS)
#define SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED(val) \
	(((val)&0x1fUL) << SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_POS)
#define SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_GET(reg) \
	(((reg) >> SERVICE_AXISYS_INTERNAL_PRIV_PRIVILEGED_POS) & 0x1fUL)

// register tp_dbgen     offset: 0x3030 (R/W 32)  Debug Access Control Register for DPM_TP */
/* bits 31:1              -                      0      */ //     reserve
/* bits 0                 risc0_dbgen            0      */ //     Allows UltraSoC debugging via RISC0
#define SERVICE_TP_DBGEN_OFFSET     0x3030UL
#define SERVICE_TP_DBGEN_MASK       1UL
#define SERVICE_TP_DBGEN_RESETVALUE 0UL

#define SERVICE_TP_DBGEN_RISC0_DBGEN_POS      0UL
#define SERVICE_TP_DBGEN_RISC0_DBGEN_MASK     (0x1UL << SERVICE_TP_DBGEN_RISC0_DBGEN_POS)
#define SERVICE_TP_DBGEN_RISC0_DBGEN(val)     (((val)&0x1UL) << SERVICE_TP_DBGEN_RISC0_DBGEN_POS)
#define SERVICE_TP_DBGEN_RISC0_DBGEN_GET(reg) (((reg) >> SERVICE_TP_DBGEN_RISC0_DBGEN_POS) & 0x1UL)

// register sdr_dbgen   offset: 0x3034 (R/W 32)  Debug Access Control Register for DPM_SDR */
/* bits 31:3                   reserve                    0   */ //  reserve
/* bits 2                      dsp1_dbgen                 0   */ //  Allows UltraSoC DSP1 debugging
/* bits 1                      dsp0_dbgen                 0   */ //  Allows UltraSoC DSP0 debugging
/* bits 0                      risc1_dbgen                0   */ //  Allows UltraSoC RISC1 debugging
#define SERVICE_SDR_DBGEN_OFFSET     0x3034UL
#define SERVICE_SDR_DBGEN_MASK       0x7UL
#define SERVICE_SDR_DBGEN_RESETVALUE 0UL

#define SERVICE_SDR_DBGEN_DSP1_DBGEN_POS      2UL
#define SERVICE_SDR_DBGEN_DSP1_DBGEN_MASK     (0x1UL << SERVICE_SDR_DBGEN_DSP1_DBGEN_POS)
#define SERVICE_SDR_DBGEN_DSP1_DBGEN(val)     (((val)&0x1UL) << SERVICE_SDR_DBGEN_DSP1_DBGEN_POS)
#define SERVICE_SDR_DBGEN_DSP1_DBGEN_GET(reg) (((reg) >> SERVICE_SDR_DBGEN_DSP1_DBGEN_POS) & 0x1UL)

#define SERVICE_SDR_DBGEN_DSP0_DBGEN_POS      1UL
#define SERVICE_SDR_DBGEN_DSP0_DBGEN_MASK     (0x1UL << SERVICE_SDR_DBGEN_DSP0_DBGEN_POS)
#define SERVICE_SDR_DBGEN_DSP0_DBGEN(val)     (((val)&0x1UL) << SERVICE_SDR_DBGEN_DSP0_DBGEN_POS)
#define SERVICE_SDR_DBGEN_DSP0_DBGEN_GET(reg) (((reg) >> SERVICE_SDR_DBGEN_DSP0_DBGEN_POS) & 0x1UL)

#define SERVICE_SDR_DBGEN_RISC1_DBGEN_POS  0UL
#define SERVICE_SDR_DBGEN_RISC1_DBGEN_MASK (0x1UL << SERVICE_SDR_DBGEN_RISC1_DBGEN_POS)
#define SERVICE_SDR_DBGEN_RISC1_DBGEN(val) (((val)&0x1UL) << SERVICE_SDR_DBGEN_RISC1_DBGEN_POS)
#define SERVICE_SDR_DBGEN_RISC1_DBGEN_GET(reg) \
	(((reg) >> SERVICE_SDR_DBGEN_RISC1_DBGEN_POS) & 0x1UL)

// register sp_dbgen      offset: 0x3038 (R/W 32)  Debug Access Control Register for DPM_SP */
/* bits 31:3                   -                            0     */ //          reserve
/* bits 2                      res0                         ?     */ //
/* bits 1                      cpu_spiden                   0     */ //          Monitors the SPIDEN CPU input
/* bits 0                      cpu_spinden                  0     */ //          Monitors the SPINDEN CPU input
#define SERVICE_SP_DBGEN_OFFSET     0x3038UL
#define SERVICE_SP_DBGEN_MASK       0x7UL
#define SERVICE_SP_DBGEN_RESETVALUE 0UL

#define SERVICE_SP_DBGEN_CPU_SPIDEN_POS      1UL
#define SERVICE_SP_DBGEN_CPU_SPIDEN_MASK     (0x1UL << SERVICE_SP_DBGEN_CPU_SPIDEN_POS)
#define SERVICE_SP_DBGEN_CPU_SPIDEN(val)     (((val)&0x1UL) << SERVICE_SP_DBGEN_CPU_SPIDEN_POS)
#define SERVICE_SP_DBGEN_CPU_SPIDEN_GET(reg) (((reg) >> SERVICE_SP_DBGEN_CPU_SPIDEN_POS) & 0x1UL)

#define SERVICE_SP_DBGEN_CPU_SPINDEN_POS      0UL
#define SERVICE_SP_DBGEN_CPU_SPINDEN_MASK     (0x1UL << SERVICE_SP_DBGEN_CPU_SPINDEN_POS)
#define SERVICE_SP_DBGEN_CPU_SPINDEN(val)     (((val)&0x1UL) << SERVICE_SP_DBGEN_CPU_SPINDEN_POS)
#define SERVICE_SP_DBGEN_CPU_SPINDEN_GET(reg) (((reg) >> SERVICE_SP_DBGEN_CPU_SPINDEN_POS) & 0x1UL)

// register s_dbgen     offset: 0x303C (R/W 32)  Debug Access Control Register for DPM_SP, DPM_SU, DPM_NSP, DPM_NSU */
/* bits 31:3                   -                             0      */ //     reserve
/* bits 2                      cpu_suniden                   0      */ //     Monitors the SUNIDEN CPU input
/* bits 1                      cpu_dbgen                     0      */ //     Monitors the DBGEN CPU input
/* bits 0                      cpu_niden                     0      */ //     Monitors the NIDEN CPU input
#define SERVICE_S_DBGEN_OFFSET     0x303CUL
#define SERVICE_S_DBGEN_MASK       0x7UL
#define SERVICE_S_DBGEN_RESETVALUE 0UL

#define SERVICE_S_DBGEN_CPU_SUNIDEN_POS      2UL
#define SERVICE_S_DBGEN_CPU_SUNIDEN_MASK     (0x1UL << SERVICE_S_DBGEN_CPU_SUNIDEN_POS)
#define SERVICE_S_DBGEN_CPU_SUNIDEN(val)     (((val)&0x1) << SERVICE_S_DBGEN_CPU_SUNIDEN_POS)
#define SERVICE_S_DBGEN_CPU_SUNIDEN_GET(reg) (((reg) >> SERVICE_S_DBGEN_CPU_SUNIDEN_POS) & 0x1)

#define SERVICE_S_DBGEN_CPU_DBGEN_POS      1UL
#define SERVICE_S_DBGEN_CPU_DBGEN_MASK     (0x1UL << SERVICE_S_DBGEN_CPU_DBGEN_POS)
#define SERVICE_S_DBGEN_CPU_DBGEN(val)     (((val)&0x1UL) << SERVICE_S_DBGEN_CPU_DBGEN_POS)
#define SERVICE_S_DBGEN_CPU_DBGEN_GET(reg) (((reg) >> SERVICE_S_DBGEN_CPU_DBGEN_POS) & 0x1UL)

#define SERVICE_S_DBGEN_CPU_NIDEN_POS      0UL
#define SERVICE_S_DBGEN_CPU_NIDEN_MASK     (0x1UL << SERVICE_S_DBGEN_CPU_NIDEN_POS)
#define SERVICE_S_DBGEN_CPU_NIDEN(val)     (((val)&0x1UL) << SERVICE_S_DBGEN_CPU_NIDEN_POS)
#define SERVICE_S_DBGEN_CPU_NIDEN_GET(reg) (((reg) >> SERVICE_S_DBGEN_CPU_NIDEN_POS) & 0x1UL)

/* register ust_dbgen  offset:  0x3040 (R/W 32)  Debug Access Control Register for UltraSoC */
/* bits 31:8           -                             0    */ //
/* bits 7              ustc_virt_cons_en             0    */ // Allowing work for the VIRT CONS UltraSoC component (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 6              ustc_dma_smb_en               0    */ // Allowing work for DMA and SMB UltraSoC components (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 5              ustc_usb_jtag_en              0    */ // Allowing work for USB COMM and JTAG COMM UltraSoC components (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 4              ustc_acom_en                  0    */ // Enable operation for the AXI COMM UltraSoC component (with vs_en == 1, this bit is ignored and operation is allowed)
/* bits 3              ustc_bpam_cpu_en              0    */ // Enabling work for the BPAM CPU component (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 2              ustc_bpam_risc0_en            0    */ // Permission of work for the BPAM RISC0 component (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 1              ustc_sdr_en                   0    */ // Allowing work for UltraSoC components as part of the sdr subsystem (with vs_en == 1, this bit is ignored and work is allowed)
/* bits 0              ustc_ddr_en                   0    */ // Allowing work for UltraSoC components as part of the ddr subsystem (with vs_en == 1, this bit is ignored and work is allowed)
#define SERVICE_UST_DBGEN_OFFSET     0x3040UL
#define SERVICE_UST_DBGEN_MASK       0xFFUL
#define SERVICE_UST_DBGEN_RESETVALUE 0

#define SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_POS  7UL
#define SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_POS)
#define SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN(val) \
	(((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_POS)
#define SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_VIRT_CONS_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_POS  6UL
#define SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_POS)
#define SERVICE_UST_DBGEN_USTC_DMA_SMB_EN(val) \
	(((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_POS)
#define SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_DMA_SMB_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_POS  5UL
#define SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_POS)
#define SERVICE_UST_DBGEN_USTC_USB_JTAG_EN(val) \
	(((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_POS)
#define SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_USB_JTAG_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_ACOM_EN_POS  4UL
#define SERVICE_UST_DBGEN_USTC_ACOM_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_ACOM_EN_POS)
#define SERVICE_UST_DBGEN_USTC_ACOM_EN(val) (((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_ACOM_EN_POS)
#define SERVICE_UST_DBGEN_USTC_ACOM_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_ACOM_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_POS  3UL
#define SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_POS)
#define SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN(val) \
	(((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_POS)
#define SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_BPAM_CPU_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_POS 2UL
#define SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_MASK \
	(0x1UL << SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_POS)
#define SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN(val) \
	(((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_POS)
#define SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_BPAM_RISC0_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_SDR_EN_POS  1UL
#define SERVICE_UST_DBGEN_USTC_SDR_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_SDR_EN_POS)
#define SERVICE_UST_DBGEN_USTC_SDR_EN(val) (((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_SDR_EN_POS)
#define SERVICE_UST_DBGEN_USTC_SDR_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_SDR_EN_POS) & 0x1UL)

#define SERVICE_UST_DBGEN_USTC_DDR_EN_POS  0UL
#define SERVICE_UST_DBGEN_USTC_DDR_EN_MASK (0x1UL << SERVICE_UST_DBGEN_USTC_DDR_EN_POS)
#define SERVICE_UST_DBGEN_USTC_DDR_EN(val) (((val)&0x1UL) << SERVICE_UST_DBGEN_USTC_DDR_EN_POS)
#define SERVICE_UST_DBGEN_USTC_DDR_EN_GET(reg) \
	(((reg) >> SERVICE_UST_DBGEN_USTC_DDR_EN_POS) & 0x1UL)

/**
 * @brief The function returns a pointer
 *        to the URB register address
 *        of the Service subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the Service subsystem
 */
service_urb_regs_t *service_get_urb_registers(void);

/**
 * @brief Function for setting the top_clkgate register values
 *        of the Service subsystem
 *
 * @param urb The pointer to urb registers service subs
 * @param top_clkgate value of top_clkgate register.
 *
 * @return Error value
 */
mcom_err_t service_set_top_clkgate(service_urb_regs_t *urb, uint32_t top_clkgate);

/**
 * @brief Function for getting the values of the top_clkgate register
 *        of the Service subsystem
 *
 * @param urb The pointer to urb registers service subs subs
 * @param top_clkgate Value of top_clkgate register
 *
 * @return Error value
 */
mcom_err_t service_get_top_clkgate(service_urb_regs_t *urb, uint32_t *top_clkgate);

/**
 * @brief The function clocks and releases the ls_periph 0 subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph0_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal ls_periph 0
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph0_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout);

/**
 * @brief The function clocks and releases the ls_periph 1 subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph1_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal ls_periph 1
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph1_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout);

/**
 * @brief The function clocks and releases the hs_periph subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_hs_periph_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal hs_periph
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_hs_periph_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                          uint32_t timeout);

/**
 * @brief The function return state of boot register.
 *
 * @param urb The pointer to urb registers
 * @param boot Value boot register
 *
 * @return Error value
 */
mcom_err_t service_get_boot_setup(service_urb_regs_t *urb, uint32_t *boot);

/**
 * @brief Function for getting the timestamp value from the counter register
 *
 * @param urb The pointer to urb registers
 * @param timestamp Value timestamp
 *
 * @return Error value
 */
mcom_err_t service_get_timestamp(service_urb_regs_t *urb, uint64_t *timestamp);

/**
 * @brief The function set state of register tp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_tp_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register tp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable) *
 *
 * @return The state of tp_dbgen register.
 */
mcom_err_t service_get_tp_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register sdr_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_sdr_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register sdr_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_sdr_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register sp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_sp_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register sp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_sp_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register s_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_s_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register s_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_s_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register ust_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_ust_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register ust_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_ust_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

#endif /* __SERVICE_URB_H__ */
