// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __QLIC_H__
#define __QLIC_H__

#define QLIC0 0
#define QLIC1 1

#define QLIC_TARG_MAX 18

#define QLIC_TARG_IP0_OFFSET 8
#define QLIC_TARG_IP1_OFFSET 9
#define QLIC_TARG_IP2_OFFSET 10
#define QLIC_TARG_IP3_OFFSET 11
#define QLIC_TARG_IP4_OFFSET 12
#define QLIC_TARG_IP5_OFFSET 13
#define QLIC_TARG_IP6_OFFSET 14
#define QLIC_TARG_IP7_OFFSET 15
#define QLIC_TARG_IP8_OFFSET 16

#define QLIC0_TARG_SERVICE_RISC0_IP2	    0
#define QLIC0_TARG_SERVICE_RISC0_IP3	    1
#define QLIC0_TARG_SERVICE_RISC0_IP4	    2
#define QLIC0_TARG_SERVICE_RISC0_IP5	    3
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU0 4
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU1 5
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU2 6
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU3 7
#define QLIC0_TARG_SERVICE_RISC0_QLIC1	    8
#define QLIC0_TARG_SERVICE_RISC0_EXTINT0    9
#define QLIC0_TARG_SERVICE_RISC0_EXTINT1    10
#define QLIC0_TARG_SERVICE_RISC0_EXTINT2    11

#define QLIC1_TARG_SDR_RISC1_IP0	    0
#define QLIC1_TARG_SDR_RISC1_IP1	    1
#define QLIC1_TARG_SDR_RISC1_IP2	    2
#define QLIC1_TARG_SDR_RISC1_IP3	    3
#define QLIC1_TARG_SDR_RISC1_IP4	    4
#define QLIC1_TARG_SDR_RISC1_IP5	    5
#define QLIC1_TARG_SDR_RISC1_IP6	    6
#define QLIC1_TARG_SDR_RISC1_IP7	    7
#define QLIC1_TARG_SDR_RISC1_IP8	    8
#define QLIC1_TARG_SDR_RISC1_DSP0_IRQIN	    9
#define QLIC1_TARG_SDR_RISC1_DSP0_IRQIN_DBG 10
#define QLIC1_TARG_SDR_RISC1_DSP1_IRQIN	    11
#define QLIC1_TARG_SDR_RISC1_DSP1_IRQIN_DBG 12
#define QLIC1_TARG_SDR_RISC1_PPI31_CPU0	    13
#define QLIC1_TARG_SDR_RISC1_PPI31_CPU1	    14
#define QLIC1_TARG_SDR_RISC1_PPI31_CPU2	    15
#define QLIC1_TARG_SDR_RISC1_PPI31_CPU3	    16
#define QLIC1_TARG_SDR_RISC1_QLIC0	    17

//TODO: Add check target to max possible value for QLIC0 and QLIC1

#define QLIC_INTERRUPTS_COUNT 128
#define QLIC_ID0_INVALID      0 /* Invalid interrupt vector  */

//QLIC0
#define QLIC0_ID_RISC0_IRQ_DMA			      1
#define QLIC0_ID_RISC0_IOMMU_IRQ		      2
#define QLIC0_ID_DSP0_EVENT			      3
#define QLIC0_ID_DSP1_EVENT			      4
#define QLIC0_ID_CPU_EVENT			      5
#define QLIC0_ID_RISC1_EVENT			      6
#define QLIC0_ID_AXICORE_AXISYs_DLOCK_IRQ	      7
#define QLIC0_ID_MB0_RRQ_RISC0			      8
#define QLIC0_ID_MB0_RRQ_RISC1			      9
#define QLIC0_ID_MB0_RRQ_DSP0			      10
#define QLIC0_ID_MB0_RRQ_DSP1			      11
#define QLIC0_ID_MB0_RRQ_CPU0			      12
#define QLIC0_ID_MB0_RRQ_CPU1			      13
#define QLIC0_ID_MB0_RRQ_CPU2			      14
#define QLIC0_ID_MB0_RRQ_CPU3			      15
#define QLIC0_ID_MFBSP0_RXIRQ			      16
#define QLIC0_ID_MFBSP0_TXIRQ			      17
#define QLIC0_ID_MFBSP0_SRQ			      18
#define QLIC0_ID_MFBSP1_RXIRQ			      19
#define QLIC0_ID_MFBSP1_TXIRQ			      20
#define QLIC0_ID_MFBSP1_SRQ			      21
#define QLIC0_ID_MFBSP0_DMAIRQ0			      22
#define QLIC0_ID_MFBSP0_DMAIRQ1			      23
#define QLIC0_ID_MFBSP1_DMAIRQ0			      24
#define QLIC0_ID_MFBSP1_DMAIRQ1			      25
#define QLIC0_ID_MFBSP0_CANINT0			      26
#define QLIC0_ID_MFBSP0_CANINT1			      27
#define QLIC0_ID_MFBSP1_CANINT0			      28
#define QLIC0_ID_MFBSP1_CANINT1			      29
#define QLIC0_ID_PVT_TS_IRQ			      30
#define QLIC0_ID_PVT_VM_IRQ			      31
#define QLIC0_ID_RTC_IT_EVENT			      32
#define QLIC0_ID_RTC_ALARM_EVENT		      33
#define QLIC0_ID_OTP_SPI_INTR			      34
#define QLIC0_ID_TRNG_IRQ			      35
#define QLIC0_ID_WDT0_INTR			      36
#define QLIC0_ID_I2C4_IC_INTR			      37
#define QLIC0_ID_QSPI0_INTREQ			      38
#define QLIC0_ID_EXTINT0			      39
#define QLIC0_ID_EXTINT1			      40
#define QLIC0_ID_USB0_COMB_IRQ			      41
#define QLIC0_ID_USB1_COMB_IRQ			      42
#define QLIC0_ID_QLIC1_IRQREQ			      43
#define QLIC0_ID_ISP_IRQ			      44
#define QLIC0_ID_GPU_IRQ			      45
#define QLIC0_ID_VPU_IRQ			      46
#define QLIC0_ID_DISP_DE_IRQ			      47
#define QLIC0_ID_DISP_SE_IRQ			      48
#define QLIC0_ID_DSI_IRQ			      49
#define QLIC0_ID_DDR0_DWC_DDRPHY_INT_N		      50
#define QLIC0_ID_DDR0_DFI_ALERT_ERR_INTR	      51
#define QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR	      52
#define QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR_FAULT    53
#define QLIC0_ID_DDR1_DWC_DDRPHY_INT_N		      54
#define QLIC0_ID_DDR1_DFI_ALERT_ERR_INTR	      55
#define QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR	      56
#define QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR_FAULT    57
#define QLIC0_ID_DDR0_ECC_CORRECTED_ERR_INTR	      58
#define QLIC0_ID_DDR0_ECC_CORRECTE_D_ERR_INTR_FAULT   59
#define QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR	      60
#define QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR_FAULT 61
#define QLIC0_ID_DDR0_ECC_AP_ERR_INTR		      62
#define QLIC0_ID_DDR0_ECC_AP_ERR_INTR_FAULT	      63
#define QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR	      64
#define QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR_FAULT    65
#define QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR	      66
#define QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR_FAULT  67
#define QLIC0_ID_DDR1_ECC_AP_ERR_INTR		      68
#define QLIC0_ID_DDR1_ECC_AP_ERR_INTR_FAULT	      69
#define QLIC0_ID_DDR_POISON_INTR		      70
#define QLIC0_ID_IRQ_EMAC0_COMB			      71
#define QLIC0_ID_IRQ_EMAC0_WKUP			      72
#define QLIC0_ID_IRQ_EMAC1_COMB			      73
#define QLIC0_ID_IRQ_EMAC1_WKUP			      74
#define QLIC0_ID_IRQ_NFC_COMB			      75
#define QLIC0_ID_IRQ_PDMA2_COMB			      76
#define QLIC0_ID_IRQ_QSPI_COMB			      77
#define QLIC0_ID_IRQ_SDMMC0_COMB		      78
#define QLIC0_ID_IRQ_SDMMC0_WKUP		      79
#define QLIC0_ID_IRQ_SDMMC1_COMB		      80
#define QLIC0_ID_IRQ_SDMMC1_WKUP		      81
#define QLIC0_ID_UST_DBG_COMB			      82
#define QLIC0_ID_INTERCONNECT_DLK_EVENT		      83
#define QLIC0_ID_PDMA0_INT_COMB			      84
#define QLIC0_ID_UART3_INTR			      85
#define QLIC0_ID_UART1_INTR			      86
#define QLIC0_ID_UART2_INTR			      87
#define QLIC0_ID_SSI0_INTR			      88
#define QLIC0_ID_I2C0_INTR			      89
#define QLIC0_ID_LS1_GPIO0_INT			      90
#define QLIC0_ID_LS1_GPIO1_INT			      91
#define QLIC0_ID_LS1_GPIO2_INT			      92
#define QLIC0_ID_LS1_GPIO3_INT			      93
#define QLIC0_ID_LS1_GPIO4_INT			      94
#define QLIC0_ID_LS1_GPIO5_INT			      95
#define QLIC0_ID_LS1_GPIO6_INT			      96
#define QLIC0_ID_LS1_GPIO7_INT			      97
#define QLIC0_ID_TIMERS0_INT			      98
#define QLIC0_ID_TIMERS1_INT			      99
#define QLIC0_ID_TIMERS2_INT			      100
#define QLIC0_ID_TIMERS3_INT			      101
#define QLIC0_ID_TIMERS4_INT			      102
#define QLIC0_ID_TIMERS5_INT			      103
#define QLIC0_ID_TIMERS6_INT			      104
#define QLIC0_ID_TIMERS7_INT			      105
#define QLIC0_ID_I2C1_IC_INTR			      106
#define QLIC0_ID_I2C2_IC_INTR			      107
#define QLIC0_ID_I2C3_IC_INTR			      108
#define QLIC0_ID_SSI1_SSI_INTR			      109
#define QLIC0_ID_I2S0_INTR			      110
#define QLIC0_ID_UART0_INTR			      111
#define QLIC0_ID_PWM_INT			      112
#define QLIC0_ID_PMWTU_INT			      113
#define QLIC0_ID_PDMA1_INT_COMB			      114
#define QLIC0_ID_WDT1_INTR			      115
#define QLIC0_ID_PCIE0_SYSTEM_IRQ		      116
#define QLIC0_ID_PCIE0_ERROR_IRQ		      117
#define QLIC0_ID_PCIE0_PME_IRQ			      118
#define QLIC0_ID_PCIE0_EDMA_IRQ			      119
#define QLIC0_ID_PCIE0_INTX_IRQ			      120
#define QLIC0_ID_PCIE0_MSI_IRQ			      121
#define QLIC0_ID_PCIE1_SYSTEM_IRQ		      122
#define QLIC0_ID_PCIE1_ERROR_IRQ		      123
#define QLIC0_ID_PCIE1_PME_IRQ			      124
#define QLIC0_ID_PCIE1_EDMA_IRQ			      125
#define QLIC0_ID_PCIE1_INTX_IRQ			      126
#define QLIC0_ID_PCIE1_MSI_IRQ			      127

//QLIC1
#define QLIC1_ID_RISC1_IRQ_DMA	    1
#define QLIC1_ID_RISC1_IRQ_IOMMU    2
#define QLIC1_ID_SDR_IRQ_QLIC0	    3
#define QLIC1_ID_UNUSED4	    4
#define QLIC1_ID_CPU_EVENT	    5
#define QLIC1_ID_RISC0_EVENT	    6
#define QLIC1_ID_UNUSED7	    7
#define QLIC1_ID_MB1_RRQ_RISC0	    8
#define QLIC1_ID_MB1_RRQ_RISC1	    9
#define QLIC1_ID_MB1_RRQ_DSP0	    10
#define QLIC1_ID_MB1_RRQ_DSP1	    11
#define QLIC1_ID_MB1_RRQ_CPU0	    12
#define QLIC1_ID_MB1_RRQ_CPU1	    13
#define QLIC1_ID_MB1_RRQ_CPU2	    14
#define QLIC1_ID_MB1_RRQ_CPU3	    15
#define QLIC1_ID_DSP0_IRQ_TLB_DSPW  16
#define QLIC1_ID_DSP0_IRQ_TLB_DSPR  16
#define QLIC1_ID_DSP0_IRQ_TLB_DMAW  16
#define QLIC1_ID_DSP0_IRQ_TLB_DMAR  16
#define QLIC1_ID_DSP0_IRQ_TLB	    16
#define QLIC1_ID_DSP0_IRQ_DMA0	    17
#define QLIC1_ID_DSP0_IRQ_DMA1	    17
#define QLIC1_ID_DSP0_IRQ_DMA2	    17
#define QLIC1_ID_DSP0_IRQ_DMA3	    17
#define QLIC1_ID_DSP0_IRQ_DMA4	    17
#define QLIC1_ID_DSP0_IRQ_DMA	    17
#define QLIC1_ID_DSP0_IRQ_PI	    18
#define QLIC1_ID_DSP0_IRQ_STP	    18
#define QLIC1_ID_DSP0_IRQ_SYSCALL   18
#define QLIC1_ID_DSP0_IRQ_TAP	    18
#define QLIC1_ID_DSP0_IRQ_SE	    18
#define QLIC1_ID_DSP0_IRQ_DBBRK	    19
#define QLIC1_ID_DSP0_IRQ_BAD	    20
#define QLIC1_ID_SDR_TIMER0_INT	    21
#define QLIC1_ID_SDR_TIMER1_INT	    22
#define QLIC1_ID_DSP1_IRQ_TLB_DSPW  23
#define QLIC1_ID_DSP1_IRQ_TLB_DSPR  23
#define QLIC1_ID_DSP1_IRQ_TLB_DMAW  23
#define QLIC1_ID_DSP1_IRQ_TLAB_DMAR 23
#define QLIC1_ID_DSP1_IRQ_DMA0	    24
#define QLIC1_ID_DSP1_IRQ_DMA1	    24
#define QLIC1_ID_DSP1_IRQ_DMA2	    24
#define QLIC1_ID_DSP1_IRQ_DMA3	    24
#define QLIC1_ID_DSP1_IRQ_PI	    25
#define QLIC1_ID_DSP1_IRQ_STP	    25
#define QLIC1_ID_DSP1_IRQ_SYSCALL   25
#define QLIC1_ID_DSP1_IRQ_TAP	    25
#define QLIC1_ID_DSP1_IRQ_SE	    25
#define QLIC1_ID_DSP1_IRQ_DBBRK	    26
#define QLIC1_ID_DSP1_IRQ_BAD	    27
#define QLIC1_ID_SDR_TIMER2_INT	    28
#define QLIC1_ID_SDR_TIMER3_INT	    29
#define QLIC1_ID_ADFE_INT	    30
#define QLIC1_ID_ADFE_RX_DMA0_IRQ   31
#define QLIC1_ID_ADFE_RX_DMA1_IRQ   32
#define QLIC1_ID_ADFE_RX_DMA2_IRQ   33
#define QLIC1_ID_ADFE_RX_DMA3_IRQ   34
#define QLIC1_ID_ADFE_MIX_DMA0_IRQ  35
#define QLIC1_ID_ADFE_MIX_DMA1_IRQ  36
#define QLIC1_ID_ADFE_MIX_DMA2_IRQ  37
#define QLIC1_ID_ADFE_MIX_DMA3_IRQ  38
#define QLIC1_ID_NDFE_DDC0_DMA0_IRQ 39
#define QLIC1_ID_NDFE_DDC0_DMA1_IRQ 40
#define QLIC1_ID_NDFE_DDC0_DMA2_IRQ 41
#define QLIC1_ID_NDFE_DDC0_DMA3_IRQ 42
#define QLIC1_ID_NDFE_DDC1_DMA0_IRQ 43
#define QLIC1_ID_NDFE_DDC1_DMA1_IRQ 44
#define QLIC1_ID_NDFE_DDC1_DMA2_IRQ 45
#define QLIC1_ID_NDFE_DDC1_DMA3_IRQ 46
#define QLIC1_ID_NDFE_DDC2_DMA0_IRQ 47
#define QLIC1_ID_NDFE_DDC2_DMA1_IRQ 48
#define QLIC1_ID_NDFE_DDC2_DMA2_IRQ 49
#define QLIC1_ID_NDFE_DDC2_DMA3_IRQ 50
#define QLIC1_ID_NDFE_DDC3_DMA0_IRQ 51
#define QLIC1_ID_NDFE_DDC3_DMA1_IRQ 52
#define QLIC1_ID_NDFE_DDC3_DMA2_IRQ 53
#define QLIC1_ID_NDFE_DDC3_DMA3_IRQ 54
#define QLIC1_ID_NDFE_ADCBP_DMA_IRQ 55
#define QLIC1_ID_NDFE_DUC_DMA0_IRQ  56
#define QLIC1_ID_NDFE_DUC_DMA1_IRQ  57
#define QLIC1_ID_NDFE_DUC_DMA2_IRQ  58
#define QLIC1_ID_NDFE_DUC_DMA3_IRQ  59
#define QLIC1_ID_NDFE_CMD_DMA0_IRQ  60
#define QLIC1_ID_NDFE_CMD_DMA1_IRQ  61
#define QLIC1_ID_NDFE_CMD_DMA2_IRQ  62
#define QLIC1_ID_NDFE_CMD_DMA3_IRQ  63
#define QLIC1_ID_NDFE_IRQ0	    64
#define QLIC1_ID_NDFE_IRQ1	    65
#define QLIC1_ID_NDFE_IRQ2	    66
#define QLIC1_ID_NDFE_IRQ3	    67
#define QLIC1_ID_NDFE_DRQ0	    68
#define QLIC1_ID_NDFE_DRQ1	    69
#define QLIC1_ID_NDFE_DRQ2	    70
#define QLIC1_ID_NDFE_DRQ3	    71
#define QLIC1_ID_NDFE_DRQ4	    72
#define QLIC1_ID_NDFE_DRQ5	    73
#define QLIC1_ID_NDFE_DRQ6	    74
#define QLIC1_ID_NDFE_DRQ7	    75
#define QLIC1_ID_NDFE_DRQ8	    76
#define QLIC1_ID_NDFE_DRQ9	    77
#define QLIC1_ID_NDFE_DRQ10	    78
#define QLIC1_ID_NDFE_DRQ11	    79
#define QLIC1_ID_NDFE_DRQ12	    80
#define QLIC1_ID_NDFE_DRQ13	    81
#define QLIC1_ID_NDFE_DRQ14	    82
#define QLIC1_ID_NDFE_DRQ15	    83
#define QLIC1_ID_NDFE_DRQ16	    84
#define QLIC1_ID_NDFE_DRQ17	    85
#define QLIC1_ID_NDFE_DRQ18	    86
#define QLIC1_ID_NDFE_DRQ19	    87
#define QLIC1_ID_NDFE_DRQ20_	    88
#define QLIC1_ID_JESD0_RX_INTERRUPT 89
#define QLIC1_ID_JESD0_TX_INTERRUPT 90
#define QLIC1_ID_JESD1_RX_INTERRUPT 91
#define QLIC1_ID_JESD1_TX_INTERRUPT 92
#define QLIC1_ID_GNSS_IRQ	    93
#define QLIC1_ID_VITDEC_IRQ	    94
#define QLIC1_ID_CONVENC_IRQ	    95
#define QLIC1_ID_TDC_IRQ	    96
#define QLIC1_ID_TEC_IRQ	    97
#define QLIC1_ID_FFT_IRQ	    98
#define QLIC1_ID_MPSM_IRQ	    99
#define QLIC1_ID_DMA0_IRQ0	    100
#define QLIC1_ID_DMA0_IRQ1	    101
#define QLIC1_ID_DMA0_IRQ2	    102
#define QLIC1_ID_DMA0_IRQ3	    103
#define QLIC1_ID_DMA0_IRQ4	    104
#define QLIC1_ID_DMA0_IRQ5	    105
#define QLIC1_ID_DMA0_IRQ6	    106
#define QLIC1_ID_DMA0_IRQ7	    107
#define QLIC1_ID_DMA1_IRQ0	    108
#define QLIC1_ID_DMA1_IRQ1	    109
#define QLIC1_ID_DMA1_IRQ2	    110
#define QLIC1_ID_DMA1_IRQ3	    111
#define QLIC1_ID_DMA1_IRQ4	    112
#define QLIC1_ID_DMA1_IRQ5	    113
#define QLIC1_ID_DMA1_IRQ6	    114
#define QLIC1_ID_DMA1_IRQ7	    115
#define QLIC1_ID_PCIE0_SYSTEM_IRQ   116
#define QLIC1_ID_PCIE0_ERROR_IRQ    117
#define QLIC1_ID_PCIE0_PME_IRQ	    118
#define QLIC1_ID_PCIE0_EDMA_IRQ	    119
#define QLIC1_ID_PCIE0_INTX_IRQ	    120
#define QLIC1_ID_PCIE0_MSI_IRQ	    121
#define QLIC1_ID_PCIE1_SYSTEM_IRQ   122
#define QLIC1_ID_PCIE1_ERROR_IRQ    123
#define QLIC1_ID_PCIE1_PME_IRQ	    124
#define QLIC1_ID_PCIE1_EDMA_IRQ	    125
#define QLIC1_ID_PCIE1_INTX_IRQ	    126
#define QLIC1_ID_PCIE1_MSI_IRQ	    127

#define QLIC_MAX_IRQ_PRIORITY	       0x7
#define QLIC_MAX_IRQ_PRIORITY_TRESHOLD 0x7
#define QLIC_IRQ_PRIORITY_COUNT	       128
#define QLIC_PEND_REG_COUNT	       4

typedef struct {
	unsigned int PRI[QLIC_IRQ_PRIORITY_COUNT];
	unsigned int Reserve0[384];
	unsigned int PEND[QLIC_PEND_REG_COUNT];
	unsigned int Reserve1[508];
	struct {
		unsigned int Base32[32];
	} TARG_ENS[QLIC_TARG_MAX];
	unsigned int Reserve2[14784];
	struct {
		unsigned int THD;
		unsigned int CC;
		unsigned int Reserve[1022];
	} TARG_THD_CC[QLIC_TARG_MAX];
} qlic_register_t;

typedef struct {
	qlic_register_t *regs;
	unsigned int QlicNum;
} qlic_t;

/**
 * @brief The qlic instance initialization
 *
 * @param qlic    - pointer to qlic context to be inited
 * @param QlicNum - qlic number
 *
 * @return qlic output state
 */
int qlic_init(qlic_t *qlic, unsigned int QlicNum);

/**
 * @brief The function enables a specific external MIPS interrupt
 *        Enables a specific external MIPS interrupt. The function manipulates bits [10: 2]
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - target to be enabled
 *
 * @todo FIXME: Change name function
 */
void qlic_enable_target_output(qlic_t *qlic, unsigned int target);

/**
 * @brief Disables a specific external MIPS interrupt. The function manipulates bits [10: 2]
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - target to be disabled
 *
 * @todo FIXME: Change name function
 */
void qlic_disable_target_output(qlic_t *qlic, unsigned int target);

/**
 * @brief get info is specific external MIPS interrupt enabled
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - target to be checked
 *
 * @return qlic output state
 *
 * @todo FIXME: Change name function
 */
int qlic_is_target_output_enabled(qlic_t *qlic, unsigned int target);

/**
 * @brief enables all MIPS global interrupts
 *
 * @param qlic - pointer to registered qlic context
 *
 * @todo FIXME: Change name function
 */
void qlic_global_irq_enable(qlic_t *qlic);

/**
 * @brief disable all MIPS global interrupts
 *
 * @param qlic - pointer to registered qlic context
 *
 * @todo FIXME: Change name function
 */
void qlic_global_irq_disable(qlic_t *qlic);

/**
 * @brief get info is MIPS global interrupts enabled or not
 *
 * @param qlic - pointer to registered qlic context
 *
 * @todo FIXME: Change name function
 */
int qlic_global_is_irq_enabled(qlic_t *qlic);

/**
 * @brief Enables the interrupt source number ID
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - qlic target output number
 * @param ID     - interrupt identification number
 *
 * @return returns the code of the error that occurred when the function was running
 */
int qlic_enable_IntID(qlic_t *qlic, unsigned int target, unsigned int ID);

/**
 * @brief Disables the interrupt source number ID
 * @param qlic   - pointer to registered qlic context
 * @param target - qlic target output number
 * @param ID     - interrupt identification number
 *
 * @return errors
 */
int qlic_disable_IntID(qlic_t *qlic, unsigned int target, unsigned int ID);

/**
 * @brief Returns for which target output the interrupt with ID is allowed
 * @param qlic   - pointer to registered qlic context
 * @param ID     - interrupt identification number
 *
 * @return Returns which target outputs are allowed interrupts with ID.
 *         The position of bit corresponds to the target output number.
 *         1 in a bit - interrupt enabled.
 */
int qlic_is_enable_intID(qlic_t *qlic, unsigned int ID);

/**
 * @brief Sets the priority of the specified ID
 *
 * @param qlic     - pointer to registered qlic context
 * @param ID       - interrupt identification number
 * @param priority - interrupt priority
 *
 * @return returns the code of the error that occurred when the function was running
 */
int qlic_set_int_priority(qlic_t *qlic, unsigned int ID, unsigned int priority);

/**
 * @brief Gets the priority of the specified ID
 *
 * @param qlic - pointer to registered qlic context
 * @param ID   - interrupt identification number
 *
 * @return returns the priority of the specified ID, or negative error code
 */
int qlic_get_int_priority(qlic_t *qlic, unsigned int ID);

/**
 * @brief function set priority threshold
 *
 * @param qlic               - pointer to registered qlic context
 * @param target             - qlic target output number
 * @param priority_threshold - priority threshold
 *
 * @return errors
 */
int qlic_set_priority_threshold(qlic_t *qlic, unsigned int target, unsigned int priority_threshold);

/**
 * @brief function get priority threshold
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - qlic target output number
 *
 * @return returns the priority threshold, or negative error code
 */
int qlic_get_priority_threshold(qlic_t *qlic, unsigned int target);

/**
 * @brief FIXME: Add function name
 *
 * @param qlic - pointer to registered qlic context
 * @param ID   - interrupt identification number
 *
 * @return returns the code of the error that occurred when the function was running
 */
int qlic_get_int_pending(qlic_t *qlic, unsigned int ID);

/**
 * @brief Get interrupt request ID.Function is called in ISR
 *
 * @param qlic   - pointer to registered qlic context
 * @param target - qlic target output number
 *
 * @return interrupt request ID
 */
int qlic_get_irq_request(qlic_t *qlic, unsigned int target);

/**
 * @brief Clear interrupt request ID.Function is called in ISR
 *
 * @param qlic    - pointer to registered qlic context
 * @param target  - qlic target output number
 * @param request - interrupt request ID to be cleaned
 *
 * @return returns the code of the error that occurred when the function was running
 */
int qlic_set_irq_terminate(qlic_t *qlic, unsigned int target, unsigned int request);

/**
 * @brief Get current IRQ target.Function is called in ISR
 *
 * @param qlic    - pointer to registered qlic context
 *
 * @return current IRQ target
 */
int qlic_get_irq_target(qlic_t *qlic);

#endif /* __QLIC_H__ */
