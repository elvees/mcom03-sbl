// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <drivers/mips-cp0/mips-cp0.h>

#include "qlic.h"

#define IRQ_ID_CP0_CNT_CMP                          MIPS_ID_CP0_CNT_CMP
#define IRQ_ID_RISC0_IRQ_DMA                        QLIC0_ID_RISC0_IRQ_DMA
#define IRQ_ID_RISC0_IOMMU_IRQ                      QLIC0_ID_RISC0_IOMMU_IRQ
#define IRQ_ID_DSP0_EVENT                           QLIC0_ID_DSP0_EVENT
#define IRQ_ID_DSP1_EVENT                           QLIC0_ID_DSP1_EVENT
#define IRQ_ID_CPU_EVENT                            QLIC0_ID_CPU_EVENT
#define IRQ_ID_RISC1_EVENT                          QLIC0_ID_RISC1_EVENT
#define IRQ_ID_AXICORE_AXISYS_DLOCK_IRQ             QLIC0_ID_AXICORE_AXISYS_DLOCK_IRQ
#define IRQ_ID_MB0_RRQ_RISC0                        QLIC0_ID_MB0_RRQ_RISC0
#define IRQ_ID_MB0_RRQ_RISC1                        QLIC0_ID_MB0_RRQ_RISC1
#define IRQ_ID_MB0_RRQ_DSP0                         QLIC0_ID_MB0_RRQ_DSP0
#define IRQ_ID_MB0_RRQ_DSP1                         QLIC0_ID_MB0_RRQ_DSP1
#define IRQ_ID_MB0_RRQ_CPU0                         QLIC0_ID_MB0_RRQ_CPU0
#define IRQ_ID_MB0_RRQ_CPU1                         QLIC0_ID_MB0_RRQ_CPU1
#define IRQ_ID_MB0_RRQ_CPU2                         QLIC0_ID_MB0_RRQ_CPU2
#define IRQ_ID_MB0_RRQ_CPU3                         QLIC0_ID_MB0_RRQ_CPU3
#define IRQ_ID_MFBSP0_RXIRQ                         QLIC0_ID_MFBSP0_RXIRQ
#define IRQ_ID_MFBSP0_TXIRQ                         QLIC0_ID_MFBSP0_TXIRQ
#define IRQ_ID_MFBSP0_SRQ                           QLIC0_ID_MFBSP0_SRQ
#define IRQ_ID_MFBSP1_RXIRQ                         QLIC0_ID_MFBSP1_RXIRQ
#define IRQ_ID_MFBSP1_TXIRQ                         QLIC0_ID_MFBSP1_TXIRQ
#define IRQ_ID_MFBSP1_SRQ                           QLIC0_ID_MFBSP1_SRQ
#define IRQ_ID_MFBSP0_DMAIRQ0                       QLIC0_ID_MFBSP0_DMAIRQ0
#define IRQ_ID_MFBSP0_DMAIRQ1                       QLIC0_ID_MFBSP0_DMAIRQ1
#define IRQ_ID_MFBSP1_DMAIRQ0                       QLIC0_ID_MFBSP1_DMAIRQ0
#define IRQ_ID_MFBSP1_DMAIRQ1                       QLIC0_ID_MFBSP1_DMAIRQ1
#define IRQ_ID_MFBSP0_CANINT0                       QLIC0_ID_MFBSP0_CANINT0
#define IRQ_ID_MFBSP0_CANINT1                       QLIC0_ID_MFBSP0_CANINT1
#define IRQ_ID_MFBSP1_CANINT0                       QLIC0_ID_MFBSP1_CANINT0
#define IRQ_ID_MFBSP1_CANINT1                       QLIC0_ID_MFBSP1_CANINT1
#define IRQ_ID_PVT_TS_IRQ                           QLIC0_ID_PVT_TS_IRQ
#define IRQ_ID_PVT_VM_IRQ                           QLIC0_ID_PVT_VM_IRQ
#define IRQ_ID_RTC_IT_EVENT                         QLIC0_ID_RTC_IT_EVENT
#define IRQ_ID_RTC_ALARM_EVENT                      QLIC0_ID_RTC_ALARM_EVENT
#define IRQ_ID_OTP_SPI_INTR                         QLIC0_ID_OTP_SPI_INTR
#define IRQ_ID_TRNG_IRQ                             QLIC0_ID_TRNG_IRQ
#define IRQ_ID_WDT0_INTR                            QLIC0_ID_WDT0_INTR
#define IRQ_ID_I2C4_IC_INTR                         QLIC0_ID_I2C4_IC_INTR
#define IRQ_ID_QSPI0_INTREQ                         QLIC0_ID_QSPI0_INTREQ
#define IRQ_ID_EXTINT0                              QLIC0_ID_EXTINT0
#define IRQ_ID_EXTINT1                              QLIC0_ID_EXTINT1
#define IRQ_ID_USB0_COMB_IRQ                        QLIC0_ID_USB0_COMB_IRQ
#define IRQ_ID_USB1_COMB_IRQ                        QLIC0_ID_USB1_COMB_IRQ
#define IRQ_ID_QLIC1_IRQREQ                         QLIC0_ID_QLIC1_IRQREQ
#define IRQ_ID_ISP_IRQ                              QLIC0_ID_ISP_IRQ
#define IRQ_ID_GPU_IRQ                              QLIC0_ID_GPU_IRQ
#define IRQ_ID_VPU_IRQ                              QLIC0_ID_VPU_IRQ
#define IRQ_ID_DISP_DE_IRQ                          QLIC0_ID_DISP_DE_IRQ
#define IRQ_ID_DISP_SE_IRQ                          QLIC0_ID_DISP_SE_IRQ
#define IRQ_ID_DSI_IRQ                              QLIC0_ID_DSI_IRQ
#define IRQ_ID_DDR0_DWC_DDRPHY_INT_N                QLIC0_ID_DDR0_DWC_DDRPHY_INT_N
#define IRQ_ID_DDR0_DFI_ALERT_ERR_INTR              QLIC0_ID_DDR0_DFI_ALERT_ERR_INTR
#define IRQ_ID_DDR0_DERATE_TEMP_LIMIT_INTR          QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR
#define IRQ_ID_DDR0_DERATE_TEMP_LIMIT_INTR_FAULT    QLIC0_ID_DDR0_DERATE_TEMP_LIMIT_INTR_FAULT
#define IRQ_ID_DDR1_DWC_DDRPHY_INT_N                QLIC0_ID_DDR1_DWC_DDRPHY_INT_N
#define IRQ_ID_DDR1_DFI_ALERT_ERR_INTR              QLIC0_ID_DDR1_DFI_ALERT_ERR_INTR
#define IRQ_ID_DDR1_DERATE_TEMP_LIMIT_INTR          QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR
#define IRQ_ID_DDR1_DERATE_TEMP_LIMIT_INTR_FAULT    QLIC0_ID_DDR1_DERATE_TEMP_LIMIT_INTR_FAULT
#define IRQ_ID_DDR0_ECC_CORRECTED_ERR_INTR          QLIC0_ID_DDR0_ECC_CORRECTED_ERR_INTR
#define IRQ_ID_DDR0_ECC_CORRECTE_D_ERR_INTR_FAULT   QLIC0_ID_DDR0_ECC_CORRECTE_D_ERR_INTR_FAULT
#define IRQ_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR       QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR
#define IRQ_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR_FAULT QLIC0_ID_DDR0_ECC_UNCORREC_TED_ERR_INTR_FAULT
#define IRQ_ID_DDR0_ECC_AP_ERR_INTR                 QLIC0_ID_DDR0_ECC_AP_ERR_INTR
#define IRQ_ID_DDR0_ECC_AP_ERR_INTR_FAULT           QLIC0_ID_DDR0_ECC_AP_ERR_INTR_FAULT
#define IRQ_ID_DDR1_ECC_CORRECTED_ERR_INTR          QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR
#define IRQ_ID_DDR1_ECC_CORRECTED_ERR_INTR_FAULT    QLIC0_ID_DDR1_ECC_CORRECTED_ERR_INTR_FAULT
#define IRQ_ID_DDR1_ECC_UNCORRECTED_ERR_INTR        QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR
#define IRQ_ID_DDR1_ECC_UNCORRECTED_ERR_INTR_FAULT  QLIC0_ID_DDR1_ECC_UNCORRECTED_ERR_INTR_FAULT
#define IRQ_ID_DDR1_ECC_AP_ERR_INTR                 QLIC0_ID_DDR1_ECC_AP_ERR_INTR
#define IRQ_ID_DDR1_ECC_AP_ERR_INTR_FAULT           QLIC0_ID_DDR1_ECC_AP_ERR_INTR_FAULT
#define IRQ_ID_DDR_POISON_INTR                      QLIC0_ID_DDR_POISON_INTR
#define IRQ_ID_IRQ_EMAC0_COMB                       QLIC0_ID_IRQ_EMAC0_COMB
#define IRQ_ID_IRQ_EMAC0_WKUP                       QLIC0_ID_IRQ_EMAC0_WKUP
#define IRQ_ID_IRQ_EMAC1_COMB                       QLIC0_ID_IRQ_EMAC1_COMB
#define IRQ_ID_IRQ_EMAC1_WKUP                       QLIC0_ID_IRQ_EMAC1_WKUP
#define IRQ_ID_IRQ_NFC_COMB                         QLIC0_ID_IRQ_NFC_COMB
#define IRQ_ID_IRQ_PDMA2_COMB                       QLIC0_ID_IRQ_PDMA2_COMB
#define IRQ_ID_IRQ_QSPI_COMB                        QLIC0_ID_IRQ_QSPI_COMB
#define IRQ_ID_IRQ_SDMMC0_COMB                      QLIC0_ID_IRQ_SDMMC0_COMB
#define IRQ_ID_IRQ_SDMMC0_WKUP                      QLIC0_ID_IRQ_SDMMC0_WKUP
#define IRQ_ID_IRQ_SDMMC1_COMB                      QLIC0_ID_IRQ_SDMMC1_COMB
#define IRQ_ID_IRQ_SDMMC1_WKUP                      QLIC0_ID_IRQ_SDMMC1_WKUP
#define IRQ_ID_UST_DBG_COMB                         QLIC0_ID_UST_DBG_COMB
#define IRQ_ID_INTERCONNECT_DLK_EVENT               QLIC0_ID_INTERCONNECT_DLK_EVENT
#define IRQ_ID_PDMA0_INT_COMB                       QLIC0_ID_PDMA0_INT_COMB
#define IRQ_ID_UART3_INTR                           QLIC0_ID_UART3_INTR
#define IRQ_ID_UART1_INTR                           QLIC0_ID_UART1_INTR
#define IRQ_ID_UART2_INTR                           QLIC0_ID_UART2_INTR
#define IRQ_ID_SSI0_INTR                            QLIC0_ID_SSI0_INTR
#define IRQ_ID_I2C0_INTR                            QLIC0_ID_I2C0_INTR
#define IRQ_ID_LSP1_GPIO0_INT                       QLIC0_ID_LSP1_GPIO0_INT
#define IRQ_ID_LSP1_GPIO1_INT                       QLIC0_ID_LSP1_GPIO1_INT
#define IRQ_ID_LSP1_GPIO2_INT                       QLIC0_ID_LSP1_GPIO2_INT
#define IRQ_ID_LSP1_GPIO3_INT                       QLIC0_ID_LSP1_GPIO3_INT
#define IRQ_ID_LSP1_GPIO4_INT                       QLIC0_ID_LSP1_GPIO4_INT
#define IRQ_ID_LSP1_GPIO5_INT                       QLIC0_ID_LSP1_GPIO5_INT
#define IRQ_ID_LSP1_GPIO6_INT                       QLIC0_ID_LSP1_GPIO6_INT
#define IRQ_ID_LSP1_GPIO7_INT                       QLIC0_ID_LSP1_GPIO7_INT
#define IRQ_ID_TIMERS0_INT                          QLIC0_ID_TIMERS0_INT
#define IRQ_ID_TIMERS1_INT                          QLIC0_ID_TIMERS1_INT
#define IRQ_ID_TIMERS2_INT                          QLIC0_ID_TIMERS2_INT
#define IRQ_ID_TIMERS3_INT                          QLIC0_ID_TIMERS3_INT
#define IRQ_ID_TIMERS4_INT                          QLIC0_ID_TIMERS4_INT
#define IRQ_ID_TIMERS5_INT                          QLIC0_ID_TIMERS5_INT
#define IRQ_ID_TIMERS6_INT                          QLIC0_ID_TIMERS6_INT
#define IRQ_ID_TIMERS7_INT                          QLIC0_ID_TIMERS7_INT
#define IRQ_ID_I2C1_IC_INTR                         QLIC0_ID_I2C1_IC_INTR
#define IRQ_ID_I2C2_IC_INTR                         QLIC0_ID_I2C2_IC_INTR
#define IRQ_ID_I2C3_IC_INTR                         QLIC0_ID_I2C3_IC_INTR
#define IRQ_ID_SSI1_SSI_INTR                        QLIC0_ID_SSI1_SSI_INTR
#define IRQ_ID_I2S0_INTR                            QLIC0_ID_I2S0_INTR
#define IRQ_ID_UART0_INTR                           QLIC0_ID_UART0_INTR
#define IRQ_ID_PWM_INT                              QLIC0_ID_PWM_INT
#define IRQ_ID_PMWTU_INT                            QLIC0_ID_PMWTU_INT
#define IRQ_ID_PDMA1_INT_COMB                       QLIC0_ID_PDMA1_INT_COMB
#define IRQ_ID_WDT1_INTR                            QLIC0_ID_WDT1_INTR
#define IRQ_ID_PCIE0_SYSTEM_IRQ                     QLIC0_ID_PCIE0_SYSTEM_IRQ
#define IRQ_ID_PCIE0_ERROR_IRQ                      QLIC0_ID_PCIE0_ERROR_IRQ
#define IRQ_ID_PCIE0_PME_IRQ                        QLIC0_ID_PCIE0_PME_IRQ
#define IRQ_ID_PCIE0_EDMA_IRQ                       QLIC0_ID_PCIE0_EDMA_IRQ
#define IRQ_ID_PCIE0_INTX_IRQ                       QLIC0_ID_PCIE0_INTX_IRQ
#define IRQ_ID_PCIE0_MSI_IRQ                        QLIC0_ID_PCIE0_MSI_IRQ
#define IRQ_ID_PCIE1_SYSTEM_IRQ                     QLIC0_ID_PCIE1_SYSTEM_IRQ
#define IRQ_ID_PCIE1_ERROR_IRQ                      QLIC0_ID_PCIE1_ERROR_IRQ
#define IRQ_ID_PCIE1_PME_IRQ                        QLIC0_ID_PCIE1_PME_IRQ
#define IRQ_ID_PCIE1_EDMA_IRQ                       QLIC0_ID_PCIE1_EDMA_IRQ
#define IRQ_ID_PCIE1_INTX_IRQ                       QLIC0_ID_PCIE1_INTX_IRQ
#define IRQ_ID_PCIE1_MSI_IRQ                        QLIC0_ID_PCIE1_MSI_IRQ

typedef void (*irq_handler_t)(unsigned int id);

int32_t irq_init(void);
int32_t irq_attach_handler(unsigned int id, irq_handler_t handler);
int32_t irq_detach_handler(unsigned int id);
void irq_handler(void);
