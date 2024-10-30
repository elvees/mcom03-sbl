// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#define QLIC_TARG_MAX 18

#define QLIC0_TARG_SERVICE_RISC0_IP2        0
#define QLIC0_TARG_SERVICE_RISC0_IP3        1
#define QLIC0_TARG_SERVICE_RISC0_IP4        2
#define QLIC0_TARG_SERVICE_RISC0_IP5        3
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU0 4
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU1 5
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU2 6
#define QLIC0_TARG_SERVICE_RISC0_PPI28_CPU3 7
#define QLIC0_TARG_SERVICE_RISC0_QLIC1      8
#define QLIC0_TARG_SERVICE_RISC0_EXTINT0    9
#define QLIC0_TARG_SERVICE_RISC0_EXTINT1    10
#define QLIC0_TARG_SERVICE_RISC0_EXTINT2    11

// TODO: Add check target to max possible value for QLIC0

#define QLIC_INTERRUPTS_COUNT 128
#define QLIC_ID0_INVALID      0 // Invalid interrupt vector

// QLIC0
#define QLIC0_ID_RISC0_IRQ_DMA                        1
#define QLIC0_ID_RISC0_IOMMU_IRQ                      2
#define QLIC0_ID_DSP0_EVENT                           3
#define QLIC0_ID_DSP1_EVENT                           4
#define QLIC0_ID_CPU_EVENT                            5
#define QLIC0_ID_RISC1_EVENT                          6
#define QLIC0_ID_AXICORE_AXISYS_DLOCK_IRQ             7
#define QLIC0_ID_MB0_RRQ_RISC0                        8
#define QLIC0_ID_MB0_RRQ_RISC1                        9
#define QLIC0_ID_MB0_RRQ_DSP0                         10
#define QLIC0_ID_MB0_RRQ_DSP1                         11
#define QLIC0_ID_MB0_RRQ_CPU0                         12
#define QLIC0_ID_MB0_RRQ_CPU1                         13
#define QLIC0_ID_MB0_RRQ_CPU2                         14
#define QLIC0_ID_MB0_RRQ_CPU3                         15
#define QLIC0_ID_MFBSP0_RXIRQ                         16
#define QLIC0_ID_MFBSP0_TXIRQ                         17
#define QLIC0_ID_MFBSP0_SRQ                           18
#define QLIC0_ID_MFBSP1_RXIRQ                         19
#define QLIC0_ID_MFBSP1_TXIRQ                         20
#define QLIC0_ID_MFBSP1_SRQ                           21
#define QLIC0_ID_MFBSP0_DMAIRQ0                       22
#define QLIC0_ID_MFBSP0_DMAIRQ1                       23
#define QLIC0_ID_MFBSP1_DMAIRQ0                       24
#define QLIC0_ID_MFBSP1_DMAIRQ1                       25
#define QLIC0_ID_MFBSP0_CANINT0                       26
#define QLIC0_ID_MFBSP0_CANINT1                       27
#define QLIC0_ID_MFBSP1_CANINT0                       28
#define QLIC0_ID_MFBSP1_CANINT1                       29
#define QLIC0_ID_PVT_TS_IRQ                           30
#define QLIC0_ID_PVT_VM_IRQ                           31
#define QLIC0_ID_RTC_IT_EVENT                         32
#define QLIC0_ID_RTC_ALARM_EVENT                      33
#define QLIC0_ID_OTP_SPI_INTR                         34
#define QLIC0_ID_TRNG_IRQ                             35
#define QLIC0_ID_WDT0_INTR                            36
#define QLIC0_ID_I2C4_IC_INTR                         37
#define QLIC0_ID_QSPI0_INTREQ                         38
#define QLIC0_ID_EXTINT0                              39
#define QLIC0_ID_EXTINT1                              40
#define QLIC0_ID_USB0_COMB_IRQ                        41
#define QLIC0_ID_USB1_COMB_IRQ                        42
#define QLIC0_ID_QLIC1_IRQREQ                         43
#define QLIC0_ID_ISP_IRQ                              44
#define QLIC0_ID_GPU_IRQ                              45
#define QLIC0_ID_VPU_IRQ                              46
#define QLIC0_ID_DISP_DE_IRQ                          47
#define QLIC0_ID_DISP_SE_IRQ                          48
#define QLIC0_ID_DSI_IRQ                              49
#define QLIC0_ID_DDR0_DWC_DDRPHY_INT_N                50
#define QLIC0_ID_DDR0_DFI_ALERT_ERR_INTR              51
#define QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR          52
#define QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR_FAULT    53
#define QLIC0_ID_DDR1_DWC_DDRPHY_INT_N                54
#define QLIC0_ID_DDR1_DFI_ALERT_ERR_INTR              55
#define QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR          56
#define QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR_FAULT    57
#define QLIC0_ID_DDR0_ECC_CORRECTED_ERR_INTR          58
#define QLIC0_ID_DDR0_ECC_CORRECTE_D_ERR_INTR_FAULT   59
#define QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR       60
#define QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR_FAULT 61
#define QLIC0_ID_DDR0_ECC_AP_ERR_INTR                 62
#define QLIC0_ID_DDR0_ECC_AP_ERR_INTR_FAULT           63
#define QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR          64
#define QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR_FAULT    65
#define QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR        66
#define QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR_FAULT  67
#define QLIC0_ID_DDR1_ECC_AP_ERR_INTR                 68
#define QLIC0_ID_DDR1_ECC_AP_ERR_INTR_FAULT           69
#define QLIC0_ID_DDR_POISON_INTR                      70
#define QLIC0_ID_IRQ_EMAC0_COMB                       71
#define QLIC0_ID_IRQ_EMAC0_WKUP                       72
#define QLIC0_ID_IRQ_EMAC1_COMB                       73
#define QLIC0_ID_IRQ_EMAC1_WKUP                       74
#define QLIC0_ID_IRQ_NFC_COMB                         75
#define QLIC0_ID_IRQ_PDMA2_COMB                       76
#define QLIC0_ID_IRQ_QSPI_COMB                        77
#define QLIC0_ID_IRQ_SDMMC0_COMB                      78
#define QLIC0_ID_IRQ_SDMMC0_WKUP                      79
#define QLIC0_ID_IRQ_SDMMC1_COMB                      80
#define QLIC0_ID_IRQ_SDMMC1_WKUP                      81
#define QLIC0_ID_UST_DBG_COMB                         82
#define QLIC0_ID_INTERCONNECT_DLK_EVENT               83
#define QLIC0_ID_PDMA0_INT_COMB                       84
#define QLIC0_ID_UART3_INTR                           85
#define QLIC0_ID_UART1_INTR                           86
#define QLIC0_ID_UART2_INTR                           87
#define QLIC0_ID_SSI0_INTR                            88
#define QLIC0_ID_I2C0_INTR                            89
#define QLIC0_ID_LSP1_GPIO0_INT                       90
#define QLIC0_ID_LSP1_GPIO1_INT                       91
#define QLIC0_ID_LSP1_GPIO2_INT                       92
#define QLIC0_ID_LSP1_GPIO3_INT                       93
#define QLIC0_ID_LSP1_GPIO4_INT                       94
#define QLIC0_ID_LSP1_GPIO5_INT                       95
#define QLIC0_ID_LSP1_GPIO6_INT                       96
#define QLIC0_ID_LSP1_GPIO7_INT                       97
#define QLIC0_ID_TIMERS0_INT                          98
#define QLIC0_ID_TIMERS1_INT                          99
#define QLIC0_ID_TIMERS2_INT                          100
#define QLIC0_ID_TIMERS3_INT                          101
#define QLIC0_ID_TIMERS4_INT                          102
#define QLIC0_ID_TIMERS5_INT                          103
#define QLIC0_ID_TIMERS6_INT                          104
#define QLIC0_ID_TIMERS7_INT                          105
#define QLIC0_ID_I2C1_IC_INTR                         106
#define QLIC0_ID_I2C2_IC_INTR                         107
#define QLIC0_ID_I2C3_IC_INTR                         108
#define QLIC0_ID_SSI1_SSI_INTR                        109
#define QLIC0_ID_I2S0_INTR                            110
#define QLIC0_ID_UART0_INTR                           111
#define QLIC0_ID_PWM_INT                              112
#define QLIC0_ID_PMWTU_INT                            113
#define QLIC0_ID_PDMA1_INT_COMB                       114
#define QLIC0_ID_WDT1_INTR                            115
#define QLIC0_ID_PCIE0_SYSTEM_IRQ                     116
#define QLIC0_ID_PCIE0_ERROR_IRQ                      117
#define QLIC0_ID_PCIE0_PME_IRQ                        118
#define QLIC0_ID_PCIE0_EDMA_IRQ                       119
#define QLIC0_ID_PCIE0_INTX_IRQ                       120
#define QLIC0_ID_PCIE0_MSI_IRQ                        121
#define QLIC0_ID_PCIE1_SYSTEM_IRQ                     122
#define QLIC0_ID_PCIE1_ERROR_IRQ                      123
#define QLIC0_ID_PCIE1_PME_IRQ                        124
#define QLIC0_ID_PCIE1_EDMA_IRQ                       125
#define QLIC0_ID_PCIE1_INTX_IRQ                       126
#define QLIC0_ID_PCIE1_MSI_IRQ                        127

#define QLIC_MAX_IRQ_PRIORITY          0x7
#define QLIC_MAX_IRQ_PRIORITY_TRESHOLD 0x7
#define QLIC_IRQ_PRIORITY_COUNT        128
#define QLIC_PEND_REG_COUNT            4

typedef struct {
	volatile unsigned int pri[QLIC_IRQ_PRIORITY_COUNT];
	volatile unsigned int reserve0[384];
	volatile unsigned int pend[QLIC_PEND_REG_COUNT];
	volatile unsigned int reserve1[508];
	struct {
		volatile unsigned int base32[32];
	} targ_ens[QLIC_TARG_MAX];
	volatile unsigned int reserve2[14784];
	struct {
		volatile unsigned int thd;
		volatile unsigned int cc;
		volatile unsigned int reserve[1022];
	} targ_thd_cc[QLIC_TARG_MAX];
} qlic_regs_t;

/**
 * @brief The qlic instance initialization.
 *        The function resets all qlic registers.
 *
 * @return 0 - Success
 */
int qlic_init(void);

/**
 * @brief Enables the interrupt source number id
 *
 * @param target - qlic target output number
 * @param id     - Interrupt identification number
 *
 * @return  0             - Success,
 *         -EINVALIDPARAM - id is bigger or equal QLIC_INTERRUPTS_COUNT
 */
int qlic_enable_int_id(unsigned int target, unsigned int id);

/**
 * @brief Disables the interrupt source number id
 *
 * @param target - qlic target output number
 * @param id     - Interrupt identification number
 *
 * @return  0             - Success,
 *         -EINVALIDPARAM - id is bigger or equal QLIC_INTERRUPTS_COUNT
 */
int qlic_disable_int_id(unsigned int target, unsigned int id);

/**
 * @brief Sets the priority of the specified id
 *
 * @param id       - Interrupt identification number
 * @param priority - Interrupt priority
 *
 * @return  0             - Success,
 *         -EINVALIDPARAM - id is bigger or equal QLIC_INTERRUPTS_COUNT
 */
int qlic_set_int_priority(unsigned int id, unsigned int priority);

/**
 * @brief Gets the priority of the specified id
 *
 * @param id - Interrupt identification number
 *
 * @return -EINVALIDPARAM - id is bigger or equal QLIC_INTERRUPTS_COUNT
 *
 *         In case of success returns the priority of the specified id
 */
int qlic_get_int_priority(unsigned int id);

/**
 * @brief The function sets priority threshold
 *
 * @param target             - qlic target output number
 * @param priority_threshold - Priority threshold
 *
 * @return 0 - Success
 */
int qlic_set_priority_threshold(unsigned int target, unsigned int priority_threshold);

/**
 * @brief The function gets priority threshold
 *
 * @param target - qlic target output number
 *
 * @return Returns the priority threshold
 */
int qlic_get_priority_threshold(unsigned int target);

/**
 * @brief Gets interrupt request id. Function is called in ISR.
 *
 * @param target - qlic target output number
 *
 * @return Returns interrupt request id
 */
int qlic_get_irq_request(unsigned int target);

/**
 * @brief Clears interrupt request id. Function is called in ISR
 *
 * @param target  - qlic target output number
 * @param request - Interrupt request id to be cleaned
 *
 * @return 0 - Success
 */
int qlic_clear_irq_request(unsigned int target, unsigned int request);
