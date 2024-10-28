// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <libs/utils-def.h>

#define QSPI_DISABLE 0 // QSPI controller disable
#define QSPI_ENABLE  1 // QSPI controller enable

#define QSPI_XIP_DISABLE 0
#define QSPI_XIP_ENABLE  1

#define QSPI_MODE_NORMAL 0 // 1-wire mode spi
#define QSPI_MODE_DUAL   2 // 2-wire mode spi
#define QSPI_MODE_QUAD   3 // 4-wire mode spi

#define QSPI_HALF_DUPLEX 0
#define QSPI_FULL_DUPLEX 1

#define QSPI_SS_ACTIVE 0 // slave select active low

#define QSPI_FORMAT_MOTOROLA 0

#define QSPI_CTRL_MASTER_MASK   BIT(5)
#define QSPI_CTRL_MSB1ST_MASK   BIT(2)
#define QSPI_CTRL_CONTXFER_MASK BIT(0)

#define QSPI_CTRL_AUX_BITSIZE_MASK        GENMASK(12, 8)
#define QSPI_CTRL_AUX_CONTXFEREXTEND_MASK BIT(7)
#define QSPI_CTRL_AUX_XFERFORMAT_MASK     GENMASK(5, 4)
#define QSPI_CTRL_AUX_INHIBITDIN_MASK     BIT(3)
#define QSPI_CTRL_AUX_INHIBITDOUT_MASK    BIT(2)
#define QSPI_CTRL_AUX_QMODE_MASK          GENMASK(1, 0)

#define QSPI_STAT_RXFULL_MASK  BIT(7)
#define QSPI_STAT_RXEMPTY_MASK BIT(5)
#define QSPI_STAT_TXFULL_MASK  BIT(4)
#define QSPI_STAT_TXEMPTY_MASK BIT(2)
#define QSPI_STAT_XFERIP_MASK  BIT(0)

#define QSPI_STAT_IRQ_ALL GENMASK(7, 0) // All interrupts enabled

#define QSPI_ENABLE_ENABLEREQ_MASK BIT(0)

#define QSPI_SS_SSOUT_MASK GENMASK(7, 0)

#define QSPI_SS_POL_SSPOL_MASK GENMASK(7, 0)

#define QSPI_GPO_GPO_MASK  GENMASK(3, 0)
#define QSPI_GPO_OEN_MASK  GENMASK(7, 4)
#define QSPI_GPO_EN_MASK   GENMASK(11, 8)
#define QSPI_GPO_FDPX_MASK BIT(12)

#define SLAVE_MASK BIT(0)

typedef struct {
	volatile unsigned int tx_data;
	volatile unsigned int rx_data;
	volatile unsigned int reserved1;
	volatile unsigned int ctrl;
	volatile unsigned int ctrl_aux;
	volatile unsigned int stat;
	volatile unsigned int ss;
	volatile unsigned int ss_polarity;
	volatile unsigned int intr_en;
	volatile unsigned int intr_stat;
	volatile unsigned int intr_clr;
	volatile unsigned int tx_fifo_lvl;
	volatile unsigned int rx_fifo_lvl;
	volatile unsigned int reserved2;
	volatile unsigned int master_delay;
	volatile unsigned int enable;
	volatile unsigned int gpo_set;
	volatile unsigned int gpo_clr;
	volatile unsigned int fifo_depth;
	volatile unsigned int fifo_wmark;
	volatile unsigned int reserver3;
} qspi_regs_t;

int qspi_init(void);
int qspi_write(const void *o_buff, size_t count);
int qspi_read(void *i_buff, size_t count);
void qspi_ss_ctrl(bool enable);
int qspi_xfer(uint8_t *send_buf, int send_len, uint8_t *recv_buf, int recv_len);
