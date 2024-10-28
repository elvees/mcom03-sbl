// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/service/service.h>
#include <libs/errors.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "qspi.h"

#define qspi_write_reg(reg_ptr, mask, val)                       \
	do {                                                     \
		uint32_t tmp = mmio_read_32((uintptr_t)reg_ptr); \
		tmp = (~(mask) & tmp) | FIELD_PREP(mask, val);   \
		mmio_write_32((uintptr_t)reg_ptr, tmp);          \
	} while (0)

static qspi_regs_t *qspi_get_regs(void)
{
	return (qspi_regs_t *)BASE_ADDR_SERVICE_QSPI0;
}

static int qspi_xip_disable(void)
{
	service_urb_regs_t *service_urb_regs = service_get_urb_registers();

	if (service_urb_regs->qspi0_xip_en_out != QSPI_XIP_DISABLE)
		service_urb_regs->qspi0_xip_en_req = QSPI_XIP_DISABLE;

	for (volatile int r = 0; r < UINT16_MAX; r++)
		if (service_urb_regs->qspi0_xip_en_out == QSPI_XIP_DISABLE)
			return 0;

	return -ETIMEOUT;
}

static int qspi_enable(qspi_regs_t *qspi_reg)
{
	qspi_write_reg(&qspi_reg->enable, QSPI_ENABLE_ENABLEREQ_MASK, QSPI_ENABLE);

	for (int r = 0; r < UINT8_MAX; r++) // deadlock need timeout
		if (FIELD_GET(QSPI_ENABLE_ENABLEREQ_MASK, qspi_reg->enable) == QSPI_ENABLE)
			return 0;

	return -ETIMEOUT;
}

static void qspi_full_duplex_en(qspi_regs_t *qspi_reg, unsigned int en)
{
	if (en)
		qspi_write_reg(&qspi_reg->gpo_clr, QSPI_GPO_FDPX_MASK, 1);
	else
		qspi_write_reg(&qspi_reg->gpo_set, QSPI_GPO_FDPX_MASK, 1);
}

int qspi_write(const void *o_buff, size_t count)
{
	if (!o_buff)
		return -ENULL;

	qspi_regs_t *qspi_regs = qspi_get_regs();

	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_BITSIZE_MASK, 7);

	// Prohibit writing to the fifo
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_INHIBITDIN_MASK, 1);

	while (!(FIELD_GET(QSPI_STAT_TXEMPTY_MASK, qspi_regs->stat)) ||
	       FIELD_GET(QSPI_STAT_XFERIP_MASK, qspi_regs->stat))
		;
	for (unsigned int i = 0; i < count; i++) {
		while (FIELD_GET(QSPI_STAT_TXFULL_MASK, qspi_regs->stat))
			;
		qspi_regs->tx_data = ((uint8_t *)o_buff)[i];
	}

	// Waiting for the completion of all transfers
	while (!(FIELD_GET(QSPI_STAT_TXEMPTY_MASK, qspi_regs->stat)) ||
	       FIELD_GET(QSPI_STAT_XFERIP_MASK, qspi_regs->stat))
		;

	return 0;
}

int qspi_read(void *i_buff, size_t count)
{
	if (!i_buff)
		return -ENULL;

	qspi_regs_t *qspi_regs = qspi_get_regs();

	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_BITSIZE_MASK, 7);

	while (qspi_regs->rx_fifo_lvl)
		qspi_regs->rx_data; // empty fifo

	// Allow writing to fifo
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_INHIBITDIN_MASK, 0);
	for (unsigned int i = 0; i < count; i++) {
		qspi_regs->tx_data = 0x3C;
		while (FIELD_GET(QSPI_STAT_RXEMPTY_MASK, qspi_regs->stat))
			;
		((uint8_t *)i_buff)[i] = (unsigned char)qspi_regs->rx_data;
	}

	return 0;
}

int qspi_init(void)
{
	int ret;
	qspi_regs_t *qspi_regs = qspi_get_regs();

	ret = qspi_xip_disable();
	if (ret)
		return ret;

	// Disable all interrupts of the qspi controller
	qspi_regs->intr_en &= ~(QSPI_STAT_IRQ_ALL);
	// Clear all interrupts from the qspi controller
	qspi_regs->intr_clr = QSPI_STAT_IRQ_ALL;
	// Set polarity
	qspi_write_reg(&qspi_regs->ss_polarity, QSPI_SS_POL_SSPOL_MASK, QSPI_SS_ACTIVE);
	// Reset all SLAVE
	qspi_write_reg(&qspi_regs->ss, QSPI_SS_SSOUT_MASK, 0);
	// Set values to control register
	qspi_regs->ctrl = (QSPI_CTRL_MASTER_MASK | QSPI_CTRL_CONTXFER_MASK | QSPI_CTRL_MSB1ST_MASK);
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_BITSIZE_MASK, 7);
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_CONTXFEREXTEND_MASK, 1);
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_XFERFORMAT_MASK, QSPI_FORMAT_MOTOROLA);
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_INHIBITDIN_MASK, 1);

	ret = qspi_enable(qspi_regs);
	if (ret)
		return ret;

	// allow qspi outputs
	qspi_write_reg(&qspi_regs->ctrl_aux, QSPI_CTRL_AUX_QMODE_MASK, QSPI_MODE_NORMAL);
	// Enable full duplex mode
	qspi_full_duplex_en(qspi_regs, QSPI_FULL_DUPLEX);
	// Allowed control outputs from the register
	qspi_write_reg(&qspi_regs->gpo_set, QSPI_GPO_GPO_MASK, 0xC);
	qspi_write_reg(&qspi_regs->gpo_clr, QSPI_GPO_GPO_MASK, ~0xC);
	// 0 QSPI_SISO
	qspi_write_reg(&qspi_regs->gpo_set, QSPI_GPO_OEN_MASK, 0);
	qspi_write_reg(&qspi_regs->gpo_clr, QSPI_GPO_OEN_MASK, ~0);
	// Enable outputs
	qspi_write_reg(&qspi_regs->gpo_set, QSPI_GPO_EN_MASK, 0xC);
	qspi_write_reg(&qspi_regs->gpo_clr, QSPI_GPO_EN_MASK, ~0xC);

	return 0;
}

void qspi_ss_ctrl(bool enable)
{
	qspi_regs_t *qspi_regs = qspi_get_regs();

	qspi_write_reg(&qspi_regs->ss, SLAVE_MASK, (enable) ? SLAVE_MASK : ~SLAVE_MASK);
}

int qspi_xfer(uint8_t *send_buf, int send_len, uint8_t *recv_buf, int recv_len)
{
	int ret;

	// Toggle SS pin to start transmission
	qspi_ss_ctrl(true);

	if (send_buf && send_len) {
		ret = qspi_write(send_buf, send_len);
		if (ret)
			return ret;
	}

	if (recv_buf && recv_len) {
		ret = qspi_read(recv_buf, recv_len);
		if (ret)
			return ret;
	}

	// Toggle SS pin to end transmission
	qspi_ss_ctrl(false);

	return 0;
}
