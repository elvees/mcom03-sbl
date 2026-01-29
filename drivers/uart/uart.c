// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <drivers/ls-periph0/ls-periph0.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/platform-def-common.h>
#include <libs/utils-def.h>

#include "uart.h"

#define MC_DL_BAUD(fr, bd) (((fr) / 16 + ((bd) / 2)) / (bd))

static int uart_rcv_err_check(uint32_t err)
{
	if (err & UART_LSR_FE_MASK)
		return -EINVALIDDATA;
	else if (err & UART_LSR_PE_MASK)
		return -EINVALIDDATA;
	else if (err & UART_LSR_OE_MASK)
		return -EOVERFLOW;

	return -EUNKNOWN;
}

int uart_drv_config_default(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;

	uart->baudrate = BAUDRATE_DEFAULT;
	uart->bits = UART_8BIT;
	uart->stop_bit = UART_STOPBIT1;
	uart->parity = UART_NOPARITY;
	uart->max_retries = UINT32_MAX;

	return 0;
}

int uart0_hw_enable(void *ctx)
{
	int ret;
	gpio_regs_t *gpio;
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;

	uart->uart_num = UART0;
	uart_drv_get_handler(uart->uart_num, &uart->uart_ptr);

	ret = lsp1_enable();
	if (ret)
		return ret;

	gpio = gpio_get_registers((void *)BASE_ADDR_LSP1_GPIO1_BASE);

	// Set modes and directions for GPIO1 PORTB PIN6 and PIN7 for UART0
	ret = gpio_init(gpio, UART0_PORT, UART0_SOUT_PIN, GPIO_MODE_HW, GPIO_DIR_OUTPUT);
	if (ret)
		return ret;

	ret = gpio_init(gpio, UART0_PORT, UART0_SIN_PIN, GPIO_MODE_HW, GPIO_DIR_INPUT);
	if (ret)
		return ret;

	// Configure pads for GPIO1 PORTB PIN6 and PIN7
	ret = lsp1_pad_cfg(UART0_PORT, UART0_SOUT_PIN,
	                   FIELD_PREP(LSP1_PADCTR_SL_MASK, 3) |
	                           FIELD_PREP(LSP1_PADCTR_CTL_MASK, PAD_DRIVER_STREGTH_6_MILLI_A));
	if (ret)
		return ret;

	ret = lsp1_pad_cfg(UART0_PORT, UART0_SIN_PIN,
	                   FIELD_PREP(LSP1_PADCTR_SUS_MASK, 1) | FIELD_PREP(LSP1_PADCTR_E_MASK, 1));
	if (ret)
		return ret;

	return 0;
}

int uart_drv_get_handler(uint32_t uart_num, uart_reg_t **uart_ptr)
{
	if (!uart_ptr)
		return -ENULL;
	if (uart_num > UART_MAX_NUMBER)
		return -EINVALIDPARAM;
	switch (uart_num) {
	case UART1:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LSP0_UART1_BASE);
		break;
	case UART2:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LSP0_UART2_BASE);
		break;
	case UART3:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LSP0_UART3_BASE);
		break;
	case UART0:
	default:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LSP1_UART0_BASE);
		break;
	}
	return 0;
}

int uart_drv_config(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;
	if (!uart->uart_ptr)
		return -ENULL;

	// Setup baudrate rate generator.
	unsigned divisor = MC_DL_BAUD(UART_CLK_HZ, uart->baudrate);

	// Disable receiving
	uart->uart_ptr->mcr = UART_MCR_RESETVALUE; // check cfg flow

	// Setup divisor
	uart->uart_ptr->lcr |= FIELD_PREP(UART_LCR_DLAB_MASK, 1);
	uart->uart_ptr->dlh_ier = divisor >> 8;
	uart->uart_ptr->rbr_thr_dll = divisor;
	uart->uart_ptr->lcr &= ~FIELD_PREP(UART_LCR_DLAB_MASK, 1); // reset bit dlab

	// Setup Line Control Register
	uint32_t uart_lcr = UART_LCR_RESETVALUE;
	uart_lcr = FIELD_PREP(UART_LCR_DLS_MASK, uart->bits) |
	           FIELD_PREP(UART_LCR_STOP_MASK, uart->stop_bit);

	switch (uart->parity) {
	case UART_ODDPARITY:
		uart_lcr |= FIELD_PREP(UART_LCR_EPS_MASK, 0) | FIELD_PREP(UART_LCR_PEN_MASK, 1);
		break;
	case UART_EVENPARITY:
		uart_lcr |= FIELD_PREP(UART_LCR_EPS_MASK, 1) | FIELD_PREP(UART_LCR_PEN_MASK, 1);
		break;
	case UART_NOPARITY:
	default:
		uart_lcr |= FIELD_PREP(UART_LCR_PEN_MASK, 0);
		break;
	}
	uart->uart_ptr->lcr = uart_lcr;

	// Disable interrupts
	uart->uart_ptr->dlh_ier = UART_IER_RESETVALUE;
	uart->uart_ptr->msr = UART_MSR_RESETVALUE;

	uart->uart_ptr->scr = UART_SCR_RESETVALUE;

	uart->uart_ptr->mcr = FIELD_PREP(UART_MCR_DTR_MASK, 1) | FIELD_PREP(UART_MCR_RTS_MASK, 1);

	// Enable FIFO, reset rx/tx FIFO
	uart->uart_ptr->fcr = FIELD_PREP(UART_FCR_RFIFOR_MASK, 1) |
	                      FIELD_PREP(UART_FCR_XFIFOR_MASK, 1) |
	                      FIELD_PREP(UART_FCR_FIFOE_MASK, 1);
	return 0;
}

int uart_drv_deinit(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;
	if (!uart->uart_ptr)
		return -ENULL;

	uart_drv_flush(ctx);

	// Disable interrupts
	uart->uart_ptr->dlh_ier = UART_IER_RESETVALUE;

	// Disable receiving
	uart->uart_ptr->mcr = UART_MCR_RESETVALUE;

	// Disable transferring
	uart->uart_ptr->fcr = UART_FCR_RESETVALUE;

	uart->uart_ptr = NULL;

	return 0;
}

int uart_drv_flush(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;
	if (!uart->uart_ptr)
		return -ENULL;

	uint32_t max_retries = uart->max_retries;

	while (!(uart->uart_ptr->lsr & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
		if (!max_retries)
			break;

		max_retries--;
	}

	return 0;
}

int uart_drv_putchar(void *ctx, char c)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;
	if (!uart->uart_ptr)
		return -ENULL;

	while (1) {
		uint32_t max_retries = uart->max_retries;

		while (!(uart->uart_ptr->lsr & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
			if (!max_retries)
				break;

			max_retries--;
		}
		uart->uart_ptr->rbr_thr_dll = c;

		if (c == '\n')
			c = '\r';
		else
			break;
	}
	return 0;
}

int uart_drv_getchar(void *ctx, int *c)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (!uart)
		return -ENULL;
	if (!uart->uart_ptr)
		return -ENULL;
	if (!c)
		return -ENULL;

	uint32_t max_retries = uart->max_retries;
	uint32_t lsr_reg = 0;

	do {
		if (!max_retries)
			break;
		max_retries--;

		lsr_reg = uart->uart_ptr->lsr;

		if (lsr_reg & UART_LSR_RFE_MASK) {
			int err = uart_rcv_err_check(lsr_reg);
			return err;
		}

	} while (!(lsr_reg & UART_LSR_DR_MASK));

	*c = uart->uart_ptr->rbr_thr_dll;

	return 0;
}
