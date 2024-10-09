// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __MCOM03_REGS_H__
#define __MCOM03_REGS_H__

#include <libs/utils-def.h>

/* Service Subs */
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

/* Service Subs TOP Clk Gate*/
#define SERVICE_TOP_CLK_GATE_SERVICE          BIT(0)
#define SERVICE_TOP_CLK_GATE_MEDIA            BIT(1)
#define SERVICE_TOP_CLK_GATE_CPU              BIT(2)
#define SERVICE_TOP_CLK_GATE_SDR              BIT(3)
#define SERVICE_TOP_CLK_GATE_HSP              BIT(4)
#define SERVICE_TOP_CLK_GATE_LSP0             BIT(5)
#define SERVICE_TOP_CLK_GATE_LSP1             BIT(6)
#define SERVICE_TOP_CLK_GATE_DDR              BIT(7)
#define SERVICE_TOP_CLK_GATE_TOP_INTERCONNECT BIT(8)
#define SERVICE_TOP_CLK_GATE_ALL_CH_MASK      GENMASK(8, 0)

/* Service Subs Debug Status*/
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

/* SDR */
#define BASE_ADDR_SDR_UCG0          0xa1900000
#define BASE_ADDR_SDR_UCG_PCIE0_REF 0xa1908000
#define BASE_ADDR_SDR_URB           0xa1910000
#define BASE_ADDR_SDR_RISC1_CSR     0xa1928000
#define BASE_ADDR_SDR_TIMERS        0xa1938000
#define BASE_ADDR_SDR_QLIC          0xa1940000
#define BASE_ADDR_SDR_UCG_PCIE1_REF 0xa1c00000
#define BASE_ADDR_SDR_UCG2          0xa1c08000
#define BASE_ADDR_SDR_SPINLOCK      0xa1c20000
#define BASE_ADDR_SDR_MAILBOX1      0xa1e00000

/* HS Periph */
#define BASE_ADDR_HSP_USB0      0xb0000000
#define BASE_ADDR_HSP_USB1      0xb0100000
#define BASE_ADDR_HSP_EMAC0     0xb0200000
#define BASE_ADDR_HSP_EMAC1     0xb0210000
#define BASE_ADDR_HSP_SDMMC0    0xb0220000
#define BASE_ADDR_HSP_SDMMC1    0xb0230000
#define BASE_ADDR_HSP_NAND      0xb0240000
#define BASE_ADDR_HSP_PDMA      0xb0250000
#define BASE_ADDR_HSP_QSPI1     0xb0260000
#define BASE_ADDR_HSP_URB       0xb0400000
#define BASE_ADDR_HSP_UCG0      0xb0410000
#define BASE_ADDR_HSP_UCG1      0xb0420000
#define BASE_ADDR_HSP_UCG2      0xb0430000
#define BASE_ADDR_HSP_UCG3      0xb0440000
#define BASE_ADDR_HSP_QSPI1_XIP 0x10000000

/* CPU Subs */
#define BASE_ADDR_CPU_URB            0xa1000000
#define BASE_ADDR_CPU_UCG            0xa1080000
#define BASE_ADDR_CPU_GIC500         0xa1100000
#define BASE_ADDR_CPU_GIC500_REDISTR 0xa1180000

/* TOP */
#define BASE_ADDR_TOP_URB_BASE        0xa1800000
#define BASE_ADDR_TOP_UCG0_BASE       0xa1801000
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

#define BASE_ADDR_TOP_UCG1_BASE        0xa1802000
#define TOP_UCG1_ALL_CH_MASK           (GENMASK(8, 4) | BIT(2) | BIT(0))
#define TOP_UCG1_SYNC_MASK             TOP_UCG1_ALL_CH_MASK
#define TOP_UCG1_CHANNEL_AXI_SLOW_COMM 0
#define TOP_UCG1_CHANNEL_AXI_FAST_COMM 2
#define TOP_UCG1_CHANNEL_DDR_SDR_DSP   4
#define TOP_UCG1_CHANNEL_DDR_SDR_PICE  5
#define TOP_UCG1_CHANNEL_DDR_LSP1      6
#define TOP_UCG1_CHANNEL_DDR_SERVICE   7
#define TOP_UCG1_CHANNEL_DDR_HSP       8

/* LSPeriph0 */
#define BASE_ADDR_LSP0_PDMA0_BASE 0xa1600000
#define BASE_ADDR_LSP0_GPIO0_BASE 0xa1610000
#define BASE_ADDR_LSP0_SPI0_BASE  0xa1620000
#define BASE_ADDR_LSP0_I2C0_BASE  0xa1630000
#define BASE_ADDR_LSP0_UART1_BASE 0xa1640000
#define BASE_ADDR_LSP0_UART2_BASE 0xa1650000
#define BASE_ADDR_LSP0_UART3_BASE 0xa1660000
#define BASE_ADDR_LSP0_URB_BASE   0xa1680000
#define BASE_ADDR_LSP0_UCG2_BASE  0xa1690000

/* LSPeriph1 */
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

/* DDR subs */
#define BASE_ADDR_DDR0_PHY_REG 0xa4000000
#define BASE_ADDR_DDR0_UMCTL2  0xa6000000

#define BASE_ADDR_DDR1_PHY_REG 0xa8000000
#define BASE_ADDR_DDR1_UMCTL2  0xaa000000

#define BASE_ADDR_DDR_SYS_URB  0xac000000
#define BASE_ADDR_DDR_SYS_UCG0 0xac010000
#define BASE_ADDR_DDR_SYS_UCG1 0xac020000

#define DDR_SYS_URB_CTRSECUREREGION           (BASE_ADDR_DDR_SYS_URB + 0x80)
#define DDR_SYS_URB_BASESECUREREGIONLOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x90 + (num * 0x10)))
#define DDR_SYS_URB_BASESECUREREGIONHIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x94 + (num * 0x10)))
#define DDR_SYS_URB_MASKSECUREREGIONLOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x98 + (num * 0x10)))
#define DDR_SYS_URB_MASKSECUREREGIONHIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x9C + (num * 0x10)))

/* Media system */
#define BASE_ADDR_MEDIA_SYS   0xa1200000
#define MEDIA_SYS_GPU_PPOLICY 0x121008
#define MEDIA_SYS_GPU_PSTATUS 0x12100c

#define MC_CSR_REG       BASE_ADDR_RISC0_CSR
#define MC_CSR_TR_CRAM   0x00000002
#define MC_CSR_TST_CACHE 0x00000800
#define MC_CSR_FLUSH_I   0x00001000
#define MC_CSR_FLUSH_D   0x00004000

/* CPU Subs UCG Channels */
#define CPU_UCG_ALL_CH_MASK      GENMASK(2, 0)
#define CPU_UCG_SYNC_MASK        CPU_UCG_ALL_CH_MASK
#define CPU_UCG_CHANNEL_CLK_CORE 1
#define CPU_UCG_CHANNEL_CLK_DBUS 2
#define CPU_UCG_CHANNEL_CLK_SYS  0

/* SERVICE UCG1 Channels */
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

/* SDR UCG0 Channels */
#define SDR_UCG0_CFG_CLK     0
#define SDR_UCG0_EXT_CLK     1
#define SDR_UCG0_INT_CLK     2
#define SDR_UCG0_PCI_CLK     3
#define SDR_UCG0_VCU_CLK     4
#define SDR_UCG0_ACC0_CLK    5
#define SDR_UCG0_ACC1_CLK    6
#define SDR_UCG0_ACC2_CLK    7
#define SDR_UCG0_AUX_PCI_CLK 8
#define SDR_UCG0_GNSS_CLK    9
#define SDR_UCG0_DFE_ALT_CLK 10
#define SDR_UCG0_VCU_TCLK    11
#define SDR_UCG0_LVDS_CLK    12

/* HS Periph UCG0-UCG3 Channels */
#define HSP_UCG0_CHANNEL_CLK_SYS    0
#define HSP_UCG0_CHANNEL_CLK_DMA    1
#define HSP_UCG0_CHANNEL_CLK_CTR    2
#define HSP_UCG0_CHANNEL_CLK_SPRAM0 3
#define HSP_UCG0_CHANNEL_CLK_EMAC0  4
#define HSP_UCG0_CHANNEL_CLK_EMAC1  5
#define HSP_UCG0_CHANNEL_CLK_USB0   6
#define HSP_UCG0_CHANNEL_CLK_USB1   7
#define HSP_UCG0_CHANNEL_CLK_NFC    8
#define HSP_UCG0_CHANNEL_CLK_PDMA2  9
#define HSP_UCG0_CHANNEL_CLK_SDMMC0 10
#define HSP_UCG0_CHANNEL_CLK_SDMMC1 11
#define HSP_UCG0_CHANNEL_CLK_QSPI1  12

#define HSP_UCG1_CHANNEL_XIN_CLK_SDMMC0 0
#define HSP_UCG1_CHANNEL_XIN_CLK_SDMMC1 1
#define HSP_UCG1_CHANNEL_CLK_NFC_FLASH  2
#define HSP_UCG1_CHANNEL_EXT_CLK_QSPI   3
#define HSP_UCG1_CHANNEL_CLK_DBG        4

#define HSP_UCG2_CHANNEL_CLK_EMAC0_1588      0
#define HSP_UCG2_CHANNEL_CLK_EMAC0_RGMII_TXC 1
#define HSP_UCG2_CHANNEL_CLK_EMAC1_1588      2
#define HSP_UCG2_CHANNEL_CLK_EMAC1_RGMII_TXC 3

#define HSP_UCG3_CHANNEL_CLK_USB0_REF_ALT 0
#define HSP_UCG3_CHANNEL_CLK_USB0_SUSPEND 1
#define HSP_UCG3_CHANNEL_CLK_USB1_REF_ALT 2
#define HSP_UCG3_CHANNEL_CLK_USB1_SUSPEND 3

/* LSPeriph0 UCG2 Channels */
#define LSP0_UCG2_CLK_SYS   0
#define LSP0_UCG2_CLK_UART3 1
#define LSP0_UCG2_CLK_UART1 2
#define LSP0_UCG2_CLK_UART2 3
#define LSP0_UCG2_CLK_SSI0  4
#define LSP0_UCG2_CLK_I2C0  5
#define LSP0_UCG2_CLK_GPIO0 6

/* LSPeriph1 UCG Channels */
#define LSP1_UCG_CLK_SYS    0
#define LSP1_UCG_CLK_I2C0   1
#define LSP1_UCG_CLK_I2C1   2
#define LSP1_UCG_CLK_I2C2   3
#define LSP1_UCG_CLK_GPIO1  4
#define LSP1_UCG_CLK_SPI1   5
#define LSP1_UCG_CLK_UART0  6
#define LSP1_UCG_CLK_TIMERS 7
#define LSP1_UCG_CLK_PWM    8
#define LSP1_UCG_CLK_WDT1   9

/* LSPeriph1 I2S UCG Channels */
#define LSP1_I2S_UCG_CLK_I2S0 0

/* LSPeriph1 subs */
#define LSP1_SUBS_UCG_ALL_CH_MASK GENMASK(9, 0)
#define LSP1_SUBS_UCG_SYNC_MASK   LSP1_SUBS_UCG_ALL_CH_MASK

#define LSP1_SUBS_GPIO1_PORTBN_PADCTR(i)  (BASE_ADDR_LSP1_URB_BASE + UL(0x40) + 0x4 * (i))
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_SUS BIT(0)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_SL  GENMASK(4, 3)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_CTL GENMASK(10, 5)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_E   BIT(12)

#endif // __MCOM03_REGS_H__
