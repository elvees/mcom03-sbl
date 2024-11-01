// Copyright 2023-2024 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>
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

	if (uart == NULL)
		return -ENULL;

	uart->uartNum = UART0;
	uart_drv_get_handler(uart->uartNum, &uart->uart_ptr);

	uart->baudrate = BAUDRATE_DEFAULT;
	uart->bits = UART_8BIT;
	uart->stopBit = UART_STOPBIT1;
	uart->parity = UART_NOPARITY;
	uart->max_retries = UINT32_MAX;

	return uart_drv_config(ctx);
}

int uart_hw_enable(void)
{
	int ret;
	uint32_t val;
	ucg_regs_t *ucg;
	gpio_regs_t *gpio1;

	// Initialize and configure the LS Peripheral 1 clocking system
	// Release reset signal of LS Peripheral 1
	service_enable_lsp1();

	// Initialize the UCG register for clocking GPIO1 and UART0
	ucg = ucg_get_registers(UCG_SUBSYS_LSP1, 0);

	ret = ucg_enable_bp(ucg, LSP1_SUBS_UCG_ALL_CH_MASK);
	if (ret)
		return ret;
	ret = ucg_set_divider(ucg, LSP1_UCG_CLK_GPIO1, 1, 1000);
	if (ret)
		return ret;
	ret = ucg_set_divider(ucg, LSP1_UCG_CLK_UART0, 1, 1000);
	if (ret)
		return ret;
	ret = ucg_set_divider(ucg, LSP1_UCG_CLK_TIMERS, 1, 1000);
	if (ret)
		return ret;
	ret = ucg_sync_and_disable_bp(ucg, LSP1_SUBS_UCG_ALL_CH_MASK, LSP1_SUBS_UCG_SYNC_MASK);
	if (ret)
		return ret;

	// Initialize GPIO1 PORTB PIN6 and PIN7 for UART 0
	gpio1 = gpio_get_registers((void *)BASE_ADDR_LSP1_GPIO1_BASE);
	gpio_init(gpio1, UART0_PORT, UART0_SOUT_PIN, GPIO_MODE_HW, GPIO_DIR_OUTPUT);
	gpio_init(gpio1, UART0_PORT, UART0_SIN_PIN, GPIO_MODE_HW, GPIO_DIR_INPUT);

	val = FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_SL, 3) |
	      FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_CTL, PAD_DRIVER_STREGTH_6mA);
	mmio_write_32(LSP1_SUBS_GPIO1_PORTBN_PADCTR(UART0_SOUT_PIN), val);

	val |= FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_SUS, 1) |
	       FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_E, 1);
	mmio_write_32(LSP1_SUBS_GPIO1_PORTBN_PADCTR(UART0_SIN_PIN), val);

	return 0;
}

int uart_drv_get_handler(uint32_t uartNum, uart_reg_t **uart_ptr)
{
	if (uart_ptr == NULL)
		return -ENULL;
	if (uartNum > UART_MAX_NUMBER)
		return -EINVALIDPARAM;
	switch (uartNum) {
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

	if (uart == NULL)
		return -ENULL;
	if (uart->uart_ptr == NULL)
		return -ENULL;

	// Setup baudrate rate generator.
	unsigned divisor = MC_DL_BAUD(UART_CLK_HZ, uart->baudrate);

	// Disable receiving
	uart->uart_ptr->MCR = UART_MCR_RESETVALUE; // check cfg flow

	// Setup divisor
	uart->uart_ptr->LCR |= FIELD_PREP(UART_LCR_DLAB_MASK, 1);
	uart->uart_ptr->DLH_IER = divisor >> 8;
	uart->uart_ptr->RBR_THR_DLL = divisor;
	uart->uart_ptr->LCR &= ~FIELD_PREP(UART_LCR_DLAB_MASK, 1); // reset bit dlab

	// Setup Line Control Register
	uint32_t uart_lcr = UART_LCR_RESETVALUE;
	uart_lcr = FIELD_PREP(UART_LCR_DLS_MASK, uart->bits) |
	           FIELD_PREP(UART_LCR_STOP_MASK, uart->stopBit);

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
	uart->uart_ptr->LCR = uart_lcr;

	// Disable interrupts
	uart->uart_ptr->DLH_IER = UART_IER_RESETVALUE;
	uart->uart_ptr->MSR = UART_MSR_RESETVALUE;

	uart->uart_ptr->SCR = UART_SCR_RESETVALUE;

	uart->uart_ptr->MCR = FIELD_PREP(UART_MCR_DTR_MASK, 1) | FIELD_PREP(UART_MCR_RTS_MASK, 1);

	// Enable FIFO, reset rx/tx FIFO
	uart->uart_ptr->FCR = FIELD_PREP(UART_FCR_RFIFOR_MASK, 1) |
	                      FIELD_PREP(UART_FCR_XFIFOR_MASK, 1) |
	                      FIELD_PREP(UART_FCR_FIFOE_MASK, 1);
	return 0;
}

int uart_drv_deinit(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (uart == NULL)
		return -ENULL;
	if (uart->uart_ptr == NULL)
		return -ENULL;

	uart_drv_flush(ctx);
	// Disable interrupts
	uart->uart_ptr->DLH_IER = UART_IER_RESETVALUE;

	// Disable receiving
	uart->uart_ptr->MCR = UART_MCR_RESETVALUE;

	// Disable transfering
	uart->uart_ptr->FCR = UART_FCR_RESETVALUE;

	uart->uart_ptr = NULL;

	return 0;
}

int uart_drv_flush(void *ctx)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (uart == NULL)
		return -ENULL;
	if (uart->uart_ptr == NULL)
		return -ENULL;

	uint32_t max_retries = uart->max_retries;

	while (!(uart->uart_ptr->LSR & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
		if (max_retries == 0)
			break;

		max_retries--;
	}

	return 0;
}

int uart_drv_putchar(void *ctx, char c)
{
	uart_param_t *uart = (uart_param_t *)ctx;

	if (uart == NULL)
		return -ENULL;
	if (uart->uart_ptr == NULL)
		return -ENULL;

	while (1) {
		uint32_t max_retries = uart->max_retries;

		while (!(uart->uart_ptr->LSR & (UART_LSR_THRE_MASK | UART_LSR_TEMT_MASK))) {
			if (max_retries == 0)
				break;

			max_retries--;
		}
		uart->uart_ptr->RBR_THR_DLL = c;

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

	if (uart == NULL)
		return -ENULL;
	if (uart->uart_ptr == NULL)
		return -ENULL;
	if (c == NULL)
		return -ENULL;

	uint32_t max_retries = uart->max_retries;
	uint32_t lsr_reg = 0;

	do {
		if (max_retries == 0)
			break;
		max_retries--;

		lsr_reg = uart->uart_ptr->LSR;

		if (lsr_reg & UART_LSR_RFE_MASK) {
			int err = uart_rcv_err_check(lsr_reg);
			return err;
		}

	} while (!(lsr_reg & UART_LSR_DR_MASK));

	*c = uart->uart_ptr->RBR_THR_DLL;

	return 0;
}
