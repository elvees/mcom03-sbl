// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <mcom03.h>
#include "uart.h"

#define MC_DL_BAUD(fr, bd) (((fr) / 16 + ((bd) / 2)) / (bd))

static mcom_err_t uart_rcv_err_check(uint32_t err);

mcom_err_t uart_drv_get_handler(uint32_t uartNum, uart_reg_t **uart_ptr)
{
	if (uart_ptr == NULL)
		return MCOM03_ERROR_NULL;
	if (uartNum > 3)
		return MCOM03_ERROR_INVALID_PARAM;
	switch (uartNum) {
	case 1:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LS0_UART1_BASE);
		break;
	case 2:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LS0_UART2_BASE);
		break;
	case 3:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LS0_UART3_BASE);
		break;
	case 0:
	default:
		*uart_ptr = (uart_reg_t *)(BASE_ADDR_LS1_UART0_BASE);
		break;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t uart_drv_config(uart_param_t *uart)
{
	if (uart == NULL)
		return MCOM03_ERROR_NULL;
	if (uart->uart_ptr == NULL)
		return MCOM03_ERROR_NULL;

	/* Setup baudrate rate generator. */
	unsigned divisor = MC_DL_BAUD(UART_CLK_HZ, uart->baudrate);

	//Disable receiving
	uart->uart_ptr->MCR = UART_MCR_RESETVALUE; //check cfg flow

	//Setup divisor
	uart->uart_ptr->LCR |= UART_LCR_DLAB(1);
	uart->uart_ptr->DLH_IER = divisor >> 8;
	uart->uart_ptr->RBR_THR_DLL = divisor;
	uart->uart_ptr->LCR &= ~(UART_LCR_DLAB(1)); // reset bit dlab

	//Setup Line Control Register
	uint32_t uart_lcr = UART_LCR_RESETVALUE;
	uart_lcr = UART_LCR_DLS(uart->bits) | UART_LCR_STOP(uart->stopBit);

	switch (uart->parity) {
	case UART_ODDPARITY:
		uart_lcr |= UART_LCR_EPS(0) | UART_LCR_PEN(1);
		break;
	case UART_EVENPARITY:
		uart_lcr |= UART_LCR_EPS(1) | UART_LCR_PEN(1);
		break;
	case UART_NOPARITY:
	default:
		uart_lcr |= UART_LCR_PEN(0);
		break;
	}
	uart->uart_ptr->LCR = uart_lcr;

	//Disable interrupts
	uart->uart_ptr->DLH_IER = UART_IER_RESETVALUE;
	uart->uart_ptr->MSR = UART_MSR_RESETVALUE;

	uart->uart_ptr->SCR = UART_SCR_RESETVALUE;

	uart->uart_ptr->MCR = UART_MCR_DTR(1) | UART_MCR_RTS(1);

	//Enable FIFO, reset rx/tx FIFO
	uart->uart_ptr->FCR = UART_FCR_RFIFOR(1) | UART_FCR_XFIFOR(1) | UART_FCR_FIFOE(1);
	return MCOM03_SUCCESS;
}

mcom_err_t uart_drv_config_default(uart_param_t *huart)
{
	if (huart == NULL)
		return MCOM03_ERROR_NULL;

	huart->uartNum = UART_DEFAULT;
	uart_drv_get_handler(huart->uartNum, &huart->uart_ptr);

	huart->baudrate = BAUDRATE_DEFAULT;
	huart->bits = UART_8BIT;
	huart->stopBit = UART_STOPBIT1;
	huart->parity = UART_NOPARITY;
	huart->timeout = UINT32_MAX;

	return uart_drv_config(huart);
}

mcom_err_t uart_drv_deinit(uart_param_t *uart)
{
	if (uart == NULL)
		return MCOM03_ERROR_NULL;
	if (uart->uart_ptr == NULL)
		return MCOM03_ERROR_NULL;

	uart_drv_flush(uart);
	//Disable interrupts
	uart->uart_ptr->DLH_IER = UART_IER_RESETVALUE;

	//Disable receiving
	uart->uart_ptr->MCR = UART_MCR_RESETVALUE;

	//Disable transfering
	uart->uart_ptr->FCR = UART_FCR_RESETVALUE;

	uart->uart_ptr = NULL;

	return MCOM03_SUCCESS;
}

mcom_err_t uart_drv_flush(uart_param_t *uart)
{
	if (uart == NULL)
		return MCOM03_ERROR_NULL;
	if (uart->uart_ptr == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t timeout = uart->timeout;

	while (!(uart->uart_ptr->LSR & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
		if (timeout == 0) {
			break;
		}

		timeout--;
	}

	return MCOM03_SUCCESS;
}

mcom_err_t uart_drv_putchar(uart_param_t *uart, char c)
{
	if (uart == NULL)
		return MCOM03_ERROR_NULL;
	if (uart->uart_ptr == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t timeout = uart->timeout;

	while (!(uart->uart_ptr->LSR & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
		if (timeout == 0) {
			break;
		}

		timeout--;
	}
	uart->uart_ptr->RBR_THR_DLL = c;
	return MCOM03_SUCCESS;
}

mcom_err_t uart_drv_getchar(uart_param_t *uart, int *c)
{
	if (uart == NULL)
		return MCOM03_ERROR_NULL;
	if (uart->uart_ptr == NULL)
		return MCOM03_ERROR_NULL;
	if (c == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t timeout = uart->timeout;
	uint32_t lsr_reg = 0;

	do {
		if (timeout == 0) {
			break;
		}
		timeout--;

		lsr_reg = uart->uart_ptr->LSR;

		if (lsr_reg & UART_LSR_RFE_MASK) {
			mcom_err_t err = uart_rcv_err_check(lsr_reg);
			return err;
		}

	} while (!(lsr_reg & UART_LSR_DR_MASK));

	*c = uart->uart_ptr->RBR_THR_DLL;

	return MCOM03_SUCCESS;
}

static mcom_err_t uart_rcv_err_check(uint32_t err)
{
	if (err & UART_LSR_FE_MASK) {
		return MCOM03_ERROR_UART_FRAME;
	} else if (err & UART_LSR_PE_MASK) {
		return MCOM03_ERROR_UART_PARITY;
	} else if (err & UART_LSR_OE_MASK) {
		return MCOM03_ERROR_UART_OVERFLOW;
	}

	return MCOM03_ERROR_UART_UNKNOWN;
}
