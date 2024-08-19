// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __GPIO_H__
#define __GPIO_H__

#include <libs/utils-def.h>

typedef struct {
	volatile unsigned int dr;
	volatile unsigned int ddr;
	volatile unsigned int ctl;
} gpio_swport_t;

typedef struct {
	gpio_swport_t swport[4];
	volatile unsigned int int_enable;
	volatile unsigned int int_mask;
	volatile unsigned int int_type_lvl;
	volatile unsigned int int_polarity;
	volatile unsigned int int_status;
	volatile unsigned int raw_int_status;
	volatile unsigned int debounce;
	volatile unsigned int porta_eoi;
	volatile unsigned int ext_port[4];
} gpio_regs_t;

#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7

#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3

#define GPIO_DIR_INPUT  0
#define GPIO_DIR_OUTPUT 1

#define GPIO_MODE_SW 0
#define GPIO_MODE_HW 1

#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET   1

#define GPIO_ACTIVE_HIGH BIT(0)
#define GPIO_ACTIVE_LOW  BIT(1)

static inline gpio_regs_t *gpio_get_registers(void *base)
{
	return (gpio_regs_t *)base;
}

int gpio_init(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int mode,
              unsigned int direction);
int gpio_read_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int *pin_state);
int gpio_write_pin(gpio_regs_t *gpio, unsigned int port, unsigned int pin, unsigned int pin_state);

#endif /* __GPIO_H__ */
