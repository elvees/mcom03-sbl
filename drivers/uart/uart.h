// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <drivers/gpio/gpio.h>
#include <libs/utils-def.h>

#define UART0               0
#define UART1               1
#define UART2               2
#define UART3               3
#define UART_MAX_NUMBER     UART3
#define BAUDRATE_DEFAULT    115200
#define UART_IER_RESETVALUE 0

#define UART0_PORT     GPIO_PORTB
#define UART0_SOUT_PIN GPIO_PIN_6
#define UART0_SIN_PIN  GPIO_PIN_7

#define UART_FCR_RESETVALUE  0
#define UART_FCR_XFIFOR_MASK BIT(2)
#define UART_FCR_RFIFOR_MASK BIT(1)
#define UART_FCR_FIFOE_MASK  BIT(0)

#define UART_LCR_RESETVALUE 0
#define UART_LCR_DLAB_MASK  BIT(7)
#define UART_LCR_EPS_MASK   BIT(4)
#define UART_LCR_PEN_MASK   BIT(3)
#define UART_LCR_STOP_MASK  BIT(2)
#define UART_LCR_DLS_MASK   GENMASK(1, 0)

#define UART_MCR_RESETVALUE 0
#define UART_MCR_RTS_MASK   BIT(1)
#define UART_MCR_DTR_MASK   BIT(0)

#define UART_LSR_RFE_MASK  BIT(7)
#define UART_LSR_TEMT_MASK BIT(6)
#define UART_LSR_THRE_MASK BIT(5)
#define UART_LSR_FE_MASK   BIT(3)
#define UART_LSR_PE_MASK   BIT(2)
#define UART_LSR_OE_MASK   BIT(1)
#define UART_LSR_DR_MASK   BIT(0)

#define UART_MSR_RESETVALUE 0

#define UART_SCR_RESETVALUE 0

typedef struct {
	volatile uint32_t rbr_thr_dll;
	volatile uint32_t dlh_ier;
	union {
		volatile uint32_t iir;
		volatile uint32_t fcr;
	};
	volatile uint32_t lcr;
	volatile uint32_t mcr;
	volatile uint32_t lsr;
	volatile uint32_t msr;
	volatile uint32_t scr;
	volatile uint32_t reserved0[4];
	volatile uint32_t srbr[16];
	volatile uint32_t far;
	volatile uint32_t tfr;
	volatile uint32_t rfw;
	volatile uint32_t usr;
	volatile uint32_t tfl;
	volatile uint32_t rfl;
	volatile uint32_t srr;
	volatile uint32_t srts;
	volatile uint32_t sbcr;
	volatile uint32_t sdmam;
	volatile uint32_t sfe;
	volatile uint32_t srt;
	volatile uint32_t stet;
	volatile uint32_t htx;
	volatile uint32_t dmasa;
	volatile uint32_t tcr;
	volatile uint32_t de_en;
	volatile uint32_t re_en;
	volatile uint32_t det;
	volatile uint32_t tat;
	volatile uint32_t dlf;
	volatile uint32_t reservrd1[12];
	volatile uint32_t cpr;
	volatile uint32_t ucv;
	volatile uint32_t ctr;
} uart_reg_t;

typedef enum {
	UART_5BIT = 0, // character length 5 bit
	UART_6BIT = 1, // character length 6 bit
	UART_7BIT = 2, // character length 7 bit
	UART_8BIT = 3 // character length 8 bit
} uart_data_len_t;

typedef enum {
	UART_STOPBIT1 = 0, // one stop bit
	UART_STOPBIT2 = 1, // two stop bits
} uart_stop_bit;

typedef enum {
	UART_NOPARITY = 0, // No parity control flow
	UART_ODDPARITY = 1, // The odd parity control
	UART_EVENPARITY = 2 // The even parity control
} uart_parity_t;

typedef struct {
	uart_reg_t *uart_ptr; // The pointer for UART controller registers
	uint32_t uart_num; // The number of UART in system
	uint32_t baudrate; // The transmission speed
	uart_data_len_t bits; // The data length
	uart_stop_bit stop_bit; // The number stop bits
	uart_parity_t parity; // The parity control
	uint32_t mode; // The mode of work functions uart driver
	int linesplit; // The mode splits lines text flow
	uint32_t max_retries; // The max number of tries
	uint32_t error_receive; // Errors of receiver
} uart_param_t;

/**
 * @brief The function configures uart
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller.
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or uart_ptr field in ctx are not provided (NULL pointers)
 */
int uart_drv_config_default(void *ctx);

/**
 * @brief The function initializes the required subsystems for uart
 *
 * @return  0        - Success,
 *         -ETIMEOUT - Function reached maximum attempts to set one of UCG divides
 */
int uart_hw_enable(void);

/**
 * @brief The function returns pointer into uart controller
 *
 * @param uart_num - Number of uart controller
 * @param uart_ptr - The pointer to registers UART controller
 *
 * @return  0             - Success,
 *         -ENULL         - uart_ptr param is not provided (NULL pointer),
 *         -EINVALIDPARAM - uart_num is bigger than the maximum UART number
 */
int uart_drv_get_handler(uint32_t uart_num, uart_reg_t **uart_ptr);

/**
 * @brief The function sets UART controller configuration
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller.
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or uart_ptr field in ctx are not provided (NULL pointers)
 */
int uart_drv_config(void *ctx);

/**
 * @brief The function disables transferring, receiving, interrupts
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or uart_ptr field in ctx are not provided (NULL pointers)
 */
int uart_drv_deinit(void *ctx);

/**
 * @brief The function gets char from rx buffer UART controller.
 *        This function receives text strings.
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller.
 * @param c   - Pointer for reading from rx buffer
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or c param or uart_ptr field in ctx are not
 *                  provided (NULL pointers)
 */
int uart_drv_getchar(void *ctx, int *c);

/**
 * @brief The function puts char direct to transmit register UART controller
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller.
 * @param c   - char for transmit
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or uart_ptr field in ctx are not provided (NULL pointers)
 */
int uart_drv_putchar(void *ctx, char c);

/**
 * @brief The function flushes the tx buffer immediately
 *
 * @param ctx - The pointer to structure uart_param_t. Structure uart_param_t describes
 *              parameters of uart controller.
 *
 * @return  0     - Success,
 *         -ENULL - ctx param or uart_ptr field in ctx are not provided (NULL pointers)
 */
int uart_drv_flush(void *ctx);
