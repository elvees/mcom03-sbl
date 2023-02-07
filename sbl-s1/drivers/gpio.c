// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "gpio.h"

#include <mcom03.h>

static gpio_regs_t *gpio0_get_registers(void)
{
	return (gpio_regs_t *)BASE_ADDR_LS0_GPIO0_BASE;
}

static gpio_regs_t *gpio1_get_registers(void)
{
	return (gpio_regs_t *)BASE_ADDR_LS1_GPIO1_BASE;
}

void gpio0_init(unsigned int port, unsigned int pin, unsigned int mode, unsigned int direction)
{
	gpio_regs_t *gpio = gpio0_get_registers();
	unsigned int regval = 0x00;

	switch (port) {
	case GPIO_PORTA:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swporta_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swporta_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swporta_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ddr = regval;
			break;
		default:
			regval = gpio->swporta_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swporta_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swporta_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swporta_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ctl = regval;
			break;
		default:
			regval = gpio->swporta_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTB:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportb_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportb_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportb_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ddr = regval;
			break;
		default:
			regval = gpio->swportb_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportb_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportb_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportb_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ctl = regval;
			break;
		default:
			regval = gpio->swportb_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTC:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportc_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportc_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportc_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ddr = regval;
			break;
		default:
			regval = gpio->swportc_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportc_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportc_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportc_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ctl = regval;
			break;
		default:
			regval = gpio->swportc_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTD:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportd_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportd_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportd_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ddr = regval;
			break;
		default:
			regval = gpio->swportd_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportd_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportd_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportd_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ctl = regval;
			break;
		default:
			regval = gpio->swportd_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ctl = regval;
			break;
		}
		break;

	default:
		gpio->swporta_dr = GPIO_SW_PORTA_DR_RESETVALUE;
		gpio->swporta_ddr = GPIO_SW_PORTA_DDR_RESETVALUE;
		gpio->swporta_ctl = GPIO_SW_PORTA_CTL_RESETVALUE;

		gpio->swportb_dr = GPIO_SW_PORTB_DR_RESETVALUE;
		gpio->swportb_ddr = GPIO_SW_PORTB_DDR_RESETVALUE;
		gpio->swportb_ctl = GPIO_SW_PORTB_CTL_RESETVALUE;

		gpio->swportc_dr = GPIO_SW_PORTC_DR_RESETVALUE;
		gpio->swportc_ddr = GPIO_SW_PORTC_DDR_RESETVALUE;
		gpio->swportc_ctl = GPIO_SW_PORTC_CTL_RESETVALUE;

		gpio->swportd_dr = GPIO_SW_PORTD_DR_RESETVALUE;
		gpio->swportd_ddr = GPIO_SW_PORTD_DDR_RESETVALUE;
		gpio->swportd_ctl = GPIO_SW_PORTD_CTL_RESETVALUE;
		break;
	}
}

void gpio1_init(unsigned int port, unsigned int pin, unsigned int mode, unsigned int direction)
{
	gpio_regs_t *gpio = gpio1_get_registers();
	unsigned int regval = 0x00;

	switch (port) {
	case GPIO_PORTA:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swporta_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swporta_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swporta_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ddr = regval;
			break;
		default:
			regval = gpio->swporta_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swporta_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swporta_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swporta_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ctl = regval;
			break;
		default:
			regval = gpio->swporta_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swporta_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTB:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportb_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportb_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportb_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ddr = regval;
			break;
		default:
			regval = gpio->swportb_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportb_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportb_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportb_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ctl = regval;
			break;
		default:
			regval = gpio->swportb_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportb_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTC:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportc_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportc_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportc_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ddr = regval;
			break;
		default:
			regval = gpio->swportc_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportc_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportc_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportc_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ctl = regval;
			break;
		default:
			regval = gpio->swportc_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportc_ctl = regval;
			break;
		}
		break;

	case GPIO_PORTD:
		/* Set pin direction */
		switch (direction) {
		case GPIO_DIR_OUTPUT:
			regval = gpio->swportd_ddr;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportd_ddr = regval;
			break;
		case GPIO_DIR_INPUT:
			regval = gpio->swportd_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ddr = regval;
			break;
		default:
			regval = gpio->swportd_ddr;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ddr = regval;
			break;
		}
		/* Set pin mode */
		switch (mode) {
		case GPIO_MODE_HW:
			regval = gpio->swportd_ctl;
			regval |= GPIO_PIN_MASK(pin);
			gpio->swportd_ctl = regval;
			break;
		case GPIO_MODE_SW:
			regval = gpio->swportd_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ctl = regval;
			break;
		default:
			regval = gpio->swportd_ctl;
			regval &= ~(GPIO_PIN_MASK(pin));
			gpio->swportd_ctl = regval;
			break;
		}
		break;

	default:
		gpio->swporta_dr = GPIO_SW_PORTA_DR_RESETVALUE;
		gpio->swporta_ddr = GPIO_SW_PORTA_DDR_RESETVALUE;
		gpio->swporta_ctl = GPIO_SW_PORTA_CTL_RESETVALUE;

		gpio->swportb_dr = GPIO_SW_PORTB_DR_RESETVALUE;
		gpio->swportb_ddr = GPIO_SW_PORTB_DDR_RESETVALUE;
		gpio->swportb_ctl = GPIO_SW_PORTB_CTL_RESETVALUE;

		gpio->swportc_dr = GPIO_SW_PORTC_DR_RESETVALUE;
		gpio->swportc_ddr = GPIO_SW_PORTC_DDR_RESETVALUE;
		gpio->swportc_ctl = GPIO_SW_PORTC_CTL_RESETVALUE;

		gpio->swportd_dr = GPIO_SW_PORTD_DR_RESETVALUE;
		gpio->swportd_ddr = GPIO_SW_PORTD_DDR_RESETVALUE;
		gpio->swportd_ctl = GPIO_SW_PORTD_CTL_RESETVALUE;
		break;
	}
}

unsigned int gpio0_read_pin(unsigned int port, unsigned int pin)
{
	gpio_regs_t *gpio = gpio0_get_registers();
	unsigned int bit_status = 0x00;

	switch (port) {
	case GPIO_PORTA:
		if ((gpio->swporta_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTB:
		if ((gpio->swportb_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTC:
		if ((gpio->swportc_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTD:
		if ((gpio->swportd_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
	default:
		bit_status = 0xFF000000;
		break;
	}
	return bit_status;
}

unsigned int gpio1_read_pin(unsigned int port, unsigned int pin)
{
	gpio_regs_t *gpio = gpio1_get_registers();
	unsigned int bit_status = 0x00;

	switch (port) {
	case GPIO_PORTA:
		if ((gpio->swporta_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTB:
		if ((gpio->swportb_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTC:
		if ((gpio->swportc_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
		break;

	case GPIO_PORTD:
		if ((gpio->swportd_dr & pin) != (unsigned int)(GPIO_PIN_RESET)) {
			bit_status = GPIO_PIN_SET;
		} else {
			bit_status = GPIO_PIN_RESET;
		}
	default:
		bit_status = 0xFF000000;
		break;
	}
	return bit_status;
}

void gpio0_write_pin(unsigned int port, unsigned int pin, unsigned int pin_state)
{
	gpio_regs_t *gpio = gpio0_get_registers();

	switch (port) {
	case GPIO_PORTA:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swporta_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swporta_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTB:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportb_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportb_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTC:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportc_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportc_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTD:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportd_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportd_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;
	default:
		break;
	}
}

void gpio1_write_pin(unsigned int port, unsigned int pin, unsigned int pin_state)
{
	gpio_regs_t *gpio = gpio1_get_registers();

	switch (port) {
	case GPIO_PORTA:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swporta_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swporta_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTB:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportb_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportb_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTC:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportc_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportc_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;

	case GPIO_PORTD:
		if (pin_state != GPIO_PIN_RESET) {
			gpio->swportd_dr |= GPIO_PIN_MASK(pin);
		} else {
			gpio->swportd_dr &= ~(GPIO_PIN_MASK(pin));
		}
		break;
	default:
		break;
	}
}
