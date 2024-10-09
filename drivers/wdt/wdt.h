// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/utils-def.h>

#define WDT_RST_MODE 0
#define WDT_IRQ_MODE 1

#define WDT_RST_PULSE_LEN_2   0
#define WDT_RST_PULSE_LEN_4   1
#define WDT_RST_PULSE_LEN_8   2
#define WDT_RST_PULSE_LEN_16  3
#define WDT_RST_PULSE_LEN_32  4
#define WDT_RST_PULSE_LEN_64  5
#define WDT_RST_PULSE_LEN_128 6
#define WDT_RST_PULSE_LEN_256 7

#define WDT_OFF 0
#define WDT_ON  1

#define WDT_MAX_TIMEOUT_COEF 0xF
#define WDT_MIN_TIMEOUT_COEF 0

#define WDT_MAX_TIMEOUT 0x7FFFFFFF
#define WDT_MIN_TIMEOUT 0

#define WDT_NUM_TOPS 16

#define WDT_CR_RPL_MASK    GENMASK(4, 2)
#define WDT_CR_RMOD_MASK   BIT(1)
#define WDT_CR_WDT_EN_MASK BIT(0)

#define WDT_TORR_TOP_INT_MASK GENMASK(7, 4)
#define WDT_TORR_TOP_MASK     GENMASK(3, 0)

#define WDT_CRR_RESET_VALUE 0x76

typedef struct {
	volatile uint32_t WDT_CR;
	volatile uint32_t WDT_TORR;
	volatile uint32_t WDT_CCVR;
	volatile uint32_t WDT_CRR;
	volatile uint32_t WDT_STAT;
	volatile uint32_t WDT_EOI;
} wdt_regs_t;

typedef struct {
	wdt_regs_t *regs;
	uint32_t wdt_freq;
	uint32_t rpl; // Reset pulse length
	uint32_t rmod; // IRQ mode
	uint32_t timeout;
	uint32_t pretimeout;
	uint32_t min_timeout;
	uint32_t max_timeout;
	uint32_t status; // Run or stop wdt status
} wdt_dev_t;

wdt_dev_t *wdt_get_instance(void);

int wdt_set_config(wdt_dev_t *wdt_dev, uint32_t timeout);
int wdt_is_enabled(wdt_dev_t *wdt_dev);
int wdt_init(wdt_dev_t *wdt_dev);
int wdt_reset(wdt_dev_t *wdt_dev);
int wdt_stop(wdt_dev_t *wdt_dev);
int wdt_start(wdt_dev_t *wdt_dev);
int wdt_set_timeout(wdt_dev_t *wdt_dev, uint32_t timeout);
int wdt_reset_irq(wdt_dev_t *wdt_dev);

uint32_t wdt_get_timeleft(wdt_dev_t *wdt_dev);
uint32_t wdt_get_timeout(wdt_dev_t *wdt_dev);
uint32_t wdt_get_min_timeout(wdt_dev_t *wdt_dev);
uint32_t wdt_get_max_timeout(wdt_dev_t *wdt_dev);
