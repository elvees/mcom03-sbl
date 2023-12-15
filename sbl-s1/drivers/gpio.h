// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __GPIO_H__
#define __GPIO_H__

#include <utils-def.h>
#include <mcom03.h>

/**
 * @brief Structure of the GPIO registers
 *
 */
typedef struct {
	volatile unsigned int dr; /**< \brief (R/W) GPIO Data */
	volatile unsigned int ddr; /**< \brief (R/W) GPIO Direction */
	volatile unsigned int ctl; /**< \brief (R/W) GPIO Control */
} gpio_swport_t;

typedef struct {
	gpio_swport_t swport[4]; /**< \brief (R/W) GPIO {A,B,C,D} SW settings */
	volatile unsigned int int_enable; /**< \brief (R/W) GPIO A IRQ Enable */
	volatile unsigned int int_mask; /**< \brief (R/W) GPIO A IRQ Mask */
	volatile unsigned int int_type_lvl; /**< \brief (R/W) GPIO A IRQ Level */
	volatile unsigned int int_polarity; /**< \brief (R/W) GPIO A IRQ Polarity */
	volatile unsigned int int_status; /**< \brief (R/ ) GPIO A IRQ Status */
	volatile unsigned int raw_int_status; /**< \brief (R/ ) GPIO A Unmasked IRQ Status */
	volatile unsigned int debounce; /**< \brief (R/W) GPIO A Debounce Enable */
	volatile unsigned int porta_eoi; /**< \brief ( /W) GPIO A IRQ Clear */
	volatile unsigned int ext_port[4]; /**< \brief (R/ ) GPIO {A,B,C,D} External */
} gpio_regs_t;

/**
 * @brief GPIO pins define
 *
 */
#define GPIO_PIN_0 U(0) /**< \brief Pin 0 selected */
#define GPIO_PIN_1 U(1) /**< \brief Pin 1 selected */
#define GPIO_PIN_2 U(2) /**< \brief Pin 2 selected */
#define GPIO_PIN_3 U(3) /**< \brief Pin 3 selected */
#define GPIO_PIN_4 U(4) /**< \brief Pin 4 selected */
#define GPIO_PIN_5 U(5) /**< \brief Pin 5 selected */
#define GPIO_PIN_6 U(6) /**< \brief Pin 6 selected */
#define GPIO_PIN_7 U(7) /**< \brief Pin 7 selected */

/**
 * @brief GPIO ports define
 *
 */
#define GPIO_PORTA U(0x0000) /**< \brief Port A selected */
#define GPIO_PORTB U(0x0001) /**< \brief Port B selected */
#define GPIO_PORTC U(0x0002) /**< \brief Port C selected */
#define GPIO_PORTD U(0x0003) /**< \brief Port D selected */

#define GPIO_DIR_INPUT	U(0x0)
#define GPIO_DIR_OUTPUT U(0x1)
#define GPIO_MODE_HW	U(0x1)
#define GPIO_MODE_SW	U(0x0)

#define GPIO_PIN_RESET U(0x0)
#define GPIO_PIN_SET   U(0x1)

#define GPIO_ACTIVE_HIGH BIT(0)
#define GPIO_ACTIVE_LOW	 BIT(1)

static inline gpio_regs_t *gpio_get_registers(void *base)
{
	return (gpio_regs_t *)base;
}

int gpio_init(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int mode,
	      unsigned int direction);
int gpio_read_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin);
int gpio_write_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int pin_state);

#endif /* __GPIO_H__ */
