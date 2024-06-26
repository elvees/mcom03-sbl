// SPDX-License-Identifier: MIT
// Copyright 2020-2021 RnD Center "ELVEES", JSC

#ifndef __MCOM03_H__
#define __MCOM03_H__

#define TOP_URB_BASE                   0xa1800000
#define TOP_UCG0_BASE                  0xa1801000
#define TOP_UCG0_ALL_CH_MASK           0xff
#define TOP_UCG0_SYNC_MASK             TOP_UCG0_ALL_CH_MASK
#define TOP_UCG0_CHANNEL_DDR_DP        0
#define TOP_UCG0_CHANNEL_DDR_VPU       1
#define TOP_UCG0_CHANNEL_DDR_GPU       2
#define TOP_UCG0_CHANNEL_DDR_ISP       3
#define TOP_UCG0_CHANNEL_DDR_CPU       4
#define TOP_UCG0_CHANNEL_CPU_ACP       5
#define TOP_UCG0_CHANNEL_DDR_LSPERIPH0 6
#define TOP_UCG0_CHANNEL_AXI_COH_COMM  7
#define TOP_UCG1_BASE                  0xa1802000
#define TOP_UCG1_ALL_CH_MASK           0x1f5
#define TOP_UCG1_SYNC_MASK             TOP_UCG1_ALL_CH_MASK
#define TOP_UCG1_CHANNEL_AXI_SLOW_COMM 0
#define TOP_UCG1_CHANNEL_AXI_FAST_COMM 2
#define TOP_UCG1_CHANNEL_DDR_SDR_DSP   4
#define TOP_UCG1_CHANNEL_DDR_SDR_PICE  5
#define TOP_UCG1_CHANNEL_DDR_LSPERIPH1 6
#define TOP_UCG1_CHANNEL_DDR_SERVICE   7
#define TOP_UCG1_CHANNEL_DDR_HSPERIPH  8

#define SERV_URB_BASE                   0xbf000000
#define SERV_UCG1_BASE                  0xbf020000
#define SERV_UCG1_ALL_CH_MASK           0xffff
#define SERV_UCG1_SYNC_MASK             0xfff
#define SERV_UCG1_CHANNEL_CLK_APB       0
#define SERV_UCG1_CHANNEL_CLK_CORE      1
#define SERV_UCG1_CHANNEL_CLK_QSPI0     2
#define SERV_UCG1_CHANNEL_CLK_BPAM      3
#define SERV_UCG1_CHANNEL_CLK_RISC0     4
#define SERV_UCG1_CHANNEL_CLK_MFBSP0    5
#define SERV_UCG1_CHANNEL_CLK_MFBSP1    6
#define SERV_UCG1_CHANNEL_CLK_MAILBOX0  7
#define SERV_UCG1_CHANNEL_CLK_PVTCTR    8
#define SERV_UCG1_CHANNEL_CLK_I2C4      9
#define SERV_UCG1_CHANNEL_CLK_TRNG      10
#define SERV_UCG1_CHANNEL_CLK_SPIOTP    11
#define SERV_UCG1_CHANNEL_CLK_I2C4_EXT  12
#define SERV_UCG1_CHANNEL_CLK_QSPI0_EXT 13
#define SERV_UCG1_CHANNEL_CLKOUT_EXT    14
#define SERV_UCG1_CHANNEL_RISC0_TCK_UCG 15

#define SERV_TOP_CLK_GATE_URB_BASE         0xBF001008
#define SERV_TOP_CLK_GATE_ALL_CH_MASK      0x1ff
#define SERV_TOP_CLK_GATE_SERVICE          0
#define SERV_TOP_CLK_GATE_MEDIA            1
#define SERV_TOP_CLK_GATE_CPU              2
#define SERV_TOP_CLK_GATE_SDR              3
#define SERV_TOP_CLK_GATE_HSPERIPH         4
#define SERV_TOP_CLK_GATE_LSPERIPH0        5
#define SERV_TOP_CLK_GATE_LSPERIPH1        6
#define SERV_TOP_CLK_GATE_DDR              7
#define SERV_TOP_CLK_GATE_TOP_INTERCONNECT 8

#define CPU_URB_BASE             0xa1000000
#define CPU_UCG_BASE             0xa1080000
#define CPU_UCG_ALL_CH_MASK      0x7
#define CPU_UCG_SYNC_MASK        CPU_UCG_ALL_CH_MASK
#define CPU_UCG_CHANNEL_CLK_SYS  0
#define CPU_UCG_CHANNEL_CLK_CORE 1
#define CPU_UCG_CHANNEL_CLK_DBUS 2

#define TOP_PLL_ADDR      (TOP_URB_BASE)
#define SERV_PLL_ADDR     (SERV_URB_BASE + 0x1000)
#define SERV_CPU_PPOLICY  (SERV_URB_BASE)
#define SERV_LSP1_PPOLICY (SERV_URB_BASE + 0x30)
#define CPU_CPU0_PPOLICY  (CPU_URB_BASE)
#define CPU_SYS_PPOLICY   (CPU_URB_BASE + 0x40)
#define CPU_PLL_ADDR      (CPU_URB_BASE + 0x50)
#define CPU_RVBADDR(i)    (CPU_URB_BASE + 0x118 + (i)*8)

#define SERV_WDT0_BASE          (SERV_URB_BASE + 0x80000)
#define SERV_WDT_CR             0x0
#define SERV_WDT_TORR           0x4
#define SERV_WDT_CRR            0xC
#define SERV_WDT_CRR_KICK_VALUE 0x76
#define SERV_WDT_EN             BIT(0)

#define HSP_SUBS_URB_BASE   0xb0400000
#define HSP_SUBS_REFCLK_REG (HSP_SUBS_URB_BASE + 0xc)

#define LSP1_SUBS_GPIO1_BASE      0xa1780000
#define LSP1_SUBS_TIMERS_BASE     0xa1790000
#define LSP1_SUBS_UCG_BASE        0xa17c0000
#define LSP1_SUBS_URB_BASE        0xa17e0000
#define LSP1_SUBS_UCG_ALL_CH_MASK 0x3ff
#define LSP1_SUBS_UCG_SYNC_MASK   LSP1_SUBS_UCG_ALL_CH_MASK
#define LSP1_SUBS_UCG_CLK_SYS     0
#define LSP1_SUBS_UCG_CLK_I2C0    1
#define LSP1_SUBS_UCG_CLK_I2C1    2
#define LSP1_SUBS_UCG_CLK_I2C2    3
#define LSP1_SUBS_UCG_CLK_GPIO1   4
#define LSP1_SUBS_UCG_CLK_SPI1    5
#define LSP1_SUBS_UCG_CLK_UART0   6
#define LSP1_SUBS_UCG_CLK_TIMERS  7
#define LSP1_SUBS_UCG_CLK_PWM     8
#define LSP1_SUBS_UCG_CLK_WDT1    9

#define LSP1_SUBS_I2S_UCG1_RSTN_PPOLICY_REG (LSP1_SUBS_URB_BASE + 0x8)

#define LSP1_SUBS_GPIO1_SWPORTB_DDR (LSP1_SUBS_GPIO1_BASE + 0x10)
#define LSP1_SUBS_GPIO1_SWPORTB_CTL (LSP1_SUBS_GPIO1_BASE + 0x14)

#define LSP1_SUBS_GPIO1_PORTBN_PADCTR(i)  (LSP1_SUBS_URB_BASE + 0x40 + (i)*0x4)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_SUS BIT(0)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_PU  BIT(1)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_PD  BIT(2)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_SL  GENMASK(4, 3)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_CTL GENMASK(10, 5)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_E   BIT(12)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_CLE BIT(13)
#define LSP1_SUBS_GPIO1_PORTBN_PADCTR_OD  BIT(14)

enum pad_driver_strength {
	PAD_DRIVER_STREGTH_2mA = 0x1,
	PAD_DRIVER_STREGTH_4mA = 0x3,
	PAD_DRIVER_STREGTH_6mA = 0x7,
	PAD_DRIVER_STREGTH_8mA = 0xf,
	PAD_DRIVER_STREGTH_10mA = 0x1f,
	PAD_DRIVER_STREGTH_12mA = 0x3f
};

#define UART_BASE       0xa1750000
#define UART_THR        UART_BASE
#define UART_DLL        UART_BASE
#define UART_IER        (UART_BASE + 0x4)
#define UART_DLH        (UART_BASE + 0x4)
#define UART_FCR        (UART_BASE + 0x8)
#define UART_FCR_FIFOE  BIT(0)
#define UART_FCR_RFIFOR BIT(1)
#define UART_FCR_XFIFOR BIT(2)
#define UART_LCR        (UART_BASE + 0xC)
#define UART_LCR_DLAB   BIT(7)
#define UART_MCR        (UART_BASE + 0x10)
#define UART_MCR_DTR    BIT(0)
#define UART_MCR_RTS    BIT(1)
#define UART_LSR        (UART_BASE + 0x14)
#define UART_LSR_THRE   BIT(5)

#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7

#define GPIO_DIR_INPUT  0
#define GPIO_DIR_OUTPUT 1

#define DW_APB_TIMER(i)         (LSP1_SUBS_TIMERS_BASE + (i)*0x14)
#define DW_APB_LOAD_COUNT(i)    (DW_APB_TIMER(i) + 0x0)
#define DW_APB_CURRENT_VALUE(i) (DW_APB_TIMER(i) + 0x4)
#define DW_APB_CTRL(i)          (DW_APB_TIMER(i) + 0x8)

#endif /* __MCOM03_H__ */
