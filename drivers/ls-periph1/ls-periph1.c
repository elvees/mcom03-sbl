// Copyright 2023-2025 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <libs/errors.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>

#include "ls-periph1.h"

lsp1_urb_regs_t *lsp1_get_urb_registers(void)
{
	return (lsp1_urb_regs_t *)BASE_ADDR_LSP1_URB_BASE;
}

int lsp1_pad_cfg(unsigned int port, unsigned int pin, unsigned int value)
{
	lsp1_urb_regs_t *urb = lsp1_get_urb_registers();

	switch (port) {
	case GPIO_PORTA:
		mmio_write_32((uintptr_t)&urb->gpio1_port_a_pad_ctr[pin], value);
		break;
	case GPIO_PORTB:
		mmio_write_32((uintptr_t)&urb->gpio1_port_b_pad_ctr[pin], value);
		break;
	case GPIO_PORTC:
		mmio_write_32((uintptr_t)&urb->gpio1_port_c_pad_ctr[pin], value);
		break;
	case GPIO_PORTD:
		mmio_write_32((uintptr_t)&urb->gpio1_port_d_pad_ctr[pin], value);
		break;
	default:
		return -EINVALIDPARAM;
	}

	return 0;
}
