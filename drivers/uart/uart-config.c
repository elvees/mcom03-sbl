// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <drivers/ls-periph1/ls-periph1.h>
#include <drivers/mcom03-regs.h>
#include <drivers/service/service.h>
#include <drivers/ucg/ucg.h>
#include <libs/errors.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "uart-config.h"
#include "uart.h"

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
