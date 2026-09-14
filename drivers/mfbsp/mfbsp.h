// SPDX-License-Identifier: MIT
// Copyright 2026 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/utils-def.h>

#define MFBSP_PIN_LACK  BIT(0)
#define MFBSP_PIN_LCLK  BIT(1)
#define MFBSP_PIN_LDAT0 BIT(2)
#define MFBSP_PIN_LDAT1 BIT(3)
#define MFBSP_PIN_LDAT2 BIT(4)
#define MFBSP_PIN_LDAT3 BIT(5)
#define MFBSP_PIN_LDAT4 BIT(6)
#define MFBSP_PIN_LDAT5 BIT(7)
#define MFBSP_PIN_LDAT6 BIT(8)
#define MFBSP_PIN_LDAT7 BIT(9)

typedef struct {
	union {
		volatile uint32_t TX;
		volatile uint32_t RX;
	};
	volatile uint32_t CSR;
	volatile uint32_t DIR;
	volatile uint32_t DR;
	volatile uint32_t TCTR;
	volatile uint32_t RCTR;
	volatile uint32_t TSR;
	volatile uint32_t RSR;
	volatile uint32_t TCTR_RATE;
	volatile uint32_t RCTR_RATE;
	volatile uint32_t TSTART;
	volatile uint32_t RSTART;
	volatile uint32_t EMERG;
	volatile uint32_t IMASK;
} mfbsp_regs_t;

enum mfbsp_dir {
	MFBSP_DIR_IN,
	MFBSP_DIR_OUT,
};

/**
 * @brief The function returns a pointer to the MFBSP registers address
 *
 * @param mfbsp_idx - MFBSP controller index (0 or 1)
 *
 * @return Pointer to the MFBSP registers or NULL when mfbsp_idx is incorrect
 */
mfbsp_regs_t *mfbsp_get_registers(unsigned int mfbsp_idx);

/**
 * @brief Set direction of pins in GPIO mode
 *
 * @param regs      - registers pointer from mfbsp_get_registers() function
 * @param pins_mask - mask of pins (see MFBSP_PIN_ defines) to set direction
 * @param dir       - new direction
 */
void mfbsp_gpio_set_dir(mfbsp_regs_t *regs, uint32_t pins_mask, enum mfbsp_dir dir);

/**
 * @brief The function returns current state of all GPIO pins
 *
 * @param regs - registers pointer from mfbsp_get_registers() function
 *
 * @return Raw value of DR register
 */
uint32_t mfbsp_gpio_get_all(mfbsp_regs_t *regs);

/**
 * @brief The function returns current state of single GPIO pin
 *
 * @param regs - registers pointer from mfbsp_get_registers() function
 * @param pin - pin to get state (see MFBSP_PIN_ defines)
 *
 * @return Current state of pin (0 or 1)
 */
unsigned int mfbsp_gpio_get(mfbsp_regs_t *regs, uint32_t pin);
