// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/utils-def.h>

#define WDT_CR_WDT_EN_MASK           BIT(0)
#define WDT_CR_RMOD_MASK             BIT(1)
#define WDT_CR_RPL_MASK              GENMASK(4, 2)
#define WDT_CR_RMOD_RST_MODE         0
#define WDT_CR_RMOD_IRQ_MODE         1
#define WDT_CR_RPL_RST_PULSE_LEN_2   0
#define WDT_CR_RPL_RST_PULSE_LEN_4   1
#define WDT_CR_RPL_RST_PULSE_LEN_8   2
#define WDT_CR_RPL_RST_PULSE_LEN_16  3
#define WDT_CR_RPL_RST_PULSE_LEN_32  4
#define WDT_CR_RPL_RST_PULSE_LEN_64  5
#define WDT_CR_RPL_RST_PULSE_LEN_128 6
#define WDT_CR_RPL_RST_PULSE_LEN_256 7
#define WDT_CR_WDT_EN_OFF            0
#define WDT_CR_WDT_EN_ON             1

#define WDT_TORR_TOP_INT_MASK         GENMASK(7, 4)
#define WDT_TORR_TOP_MASK             GENMASK(3, 0)
#define WDT_TORR_TOP_MAX_TIMEOUT_COEF 0xF
#define WDT_TORR_TOP_MIN_TIMEOUT_COEF 0
#define WDT_TORR_NUM_TOPS             16

#define WDT_STAT_ISR_MASK     BIT(0)
#define WDT_STAT_ISR_INACTIVE 0
#define WDT_STAT_ISR_ACTIVE   1

#define WDT_CRR_RESET_VALUE 0x76

typedef struct {
	volatile uint32_t cr;
	volatile uint32_t torr;
	volatile uint32_t ccvr;
	volatile uint32_t crr;
	volatile uint32_t stat;
	volatile uint32_t eoi;
} wdt_regs_t;

typedef struct {
	wdt_regs_t *regs;
	uint32_t freq;
	uint32_t rpl; // Reset pulse length
	uint32_t rmod; // IRQ mode
} wdt_dev_t;

/**
 * @brief The function gets pointer of the wdt device structure instance
 *
 * @return Pointer of the wdt device structure instance
 */
wdt_dev_t *wdt_get_instance(void);

/**
 * @brief The function sets default WDT configuration
 *
 * @param wdt        - Pointer to WDT instance structure
 * @param timeout_ms - Timeout value in milliseconds
 *
 * @return  0     - Success,
 *         -ENULL - NULL pointer provided
 */
int wdt_config_default(wdt_dev_t *wdt);

/**
 * @brief Sets current timeout value
 *
 * @param wdt     - Pointer to WDT instance structure
 * @param timeout - New timeout value in milliseconds
 *
 * @return  0     - Success,
 *         -ENULL - NULL pointer provided
 */
int wdt_set_timeout_ms(wdt_dev_t *wdt, uint32_t timeout_ms);

/**
 * @brief Gets current timeout value
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return Current timeout value in milliseconds
 */
uint32_t wdt_get_timeout_ms(wdt_dev_t *wdt);

/**
 * @brief Gets min timeout value depending on freq
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return MIN timeout value in milliseconds
 */
uint32_t wdt_get_min_timeout_ms(wdt_dev_t *wdt);

/**
 * @brief Gets max timeout value depending on freq
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return MAX timeout value in milliseconds
 */
uint32_t wdt_get_max_timeout_ms(wdt_dev_t *wdt);

/**
 * @brief Checks if wdt is enabled or not
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return 1 - if enabled, 0 - otherwise
 */
int wdt_is_enabled(wdt_dev_t *wdt);

/**
 * @brief Enable WDT with max timeout
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return  0     - Success
 *         -ENULL - NULL pointer provided
 */
int wdt_start(wdt_dev_t *wdt);

/**
 * @brief Reset IRQ pending
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return  0     - Success
 *         -ENULL - NULL pointer provided
 */
int wdt_reset_irq(wdt_dev_t *wdt);

/**
 * @brief Resets counter to initial value and clear interrupt
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return  0     - Success
 *         -ENULL - NULL pointer provided
 */
int wdt_reset(wdt_dev_t *wdt);

/**
 * @brief Initializes WDT0 timer
 *
 * @param wdt - Pointer to WDT instance structure
 *
 * @return  0                   - Success
 *         -ENULL               - NULL pointer provided
 *         -EALREADYINITIALIZED - WDT is already enabled
 */
int wdt0_hw_enable(wdt_dev_t *wdt);
