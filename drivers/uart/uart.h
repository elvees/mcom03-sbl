// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>

#include <mcom03-errors.h>

#define UART0               0
#define UART1               1
#define UART2               2
#define UART3               3
#define UART_MAX_NUMBER     UART3
#define UART_DEFAULT        UART0
#define BAUDRATE_DEFAULT    115200
#define UART_IER_RESETVALUE 0

#define UART_FCR_RESETVALUE  0
#define UART_FCR_XFIFOR_MASK BIT(2)
#define UART_FCR_XFIFOR(val) FIELD_PREP(UART_FCR_XFIFOR_MASK, (val))
#define UART_FCR_RFIFOR_MASK BIT(1)
#define UART_FCR_RFIFOR(val) FIELD_PREP(UART_FCR_RFIFOR_MASK, (val))
#define UART_FCR_FIFOE_MASK  BIT(0)
#define UART_FCR_FIFOE(val)  FIELD_PREP(UART_FCR_FIFOE_MASK, (val))

#define UART_LCR_RESETVALUE 0
#define UART_LCR_DLAB_MASK  BIT(7)
#define UART_LCR_DLAB(val)  FIELD_PREP(UART_LCR_DLAB_MASK, (val))
#define UART_LCR_EPS_MASK   BIT(4)
#define UART_LCR_EPS(val)   FIELD_PREP(UART_LCR_EPS_MASK, (val))
#define UART_LCR_PEN_MASK   BIT(3)
#define UART_LCR_PEN(val)   FIELD_PREP(UART_LCR_PEN_MASK, (val))
#define UART_LCR_STOP_MASK  BIT(2)
#define UART_LCR_STOP(val)  FIELD_PREP(UART_LCR_STOP_MASK, (val))
#define UART_LCR_DLS_MASK   GENMASK(1, 0)
#define UART_LCR_DLS(val)   FIELD_PREP(UART_LCR_DLS_MASK, (val))

#define UART_MCR_RESETVALUE 0
#define UART_MCR_RTS_MASK   BIT(1)
#define UART_MCR_RTS(val)   FIELD_PREP(UART_MCR_RTS_MASK, (val))
#define UART_MCR_DTR_MASK   BIT(0)
#define UART_MCR_DTR(val)   FIELD_PREP(UART_MCR_DTR_MASK, (val))

#define UART_LSR_RFE_MASK  BIT(7)
#define UART_LSR_TEMT_MASK BIT(6)
#define UART_LSR_THRE_MASK BIT(5)
#define UART_LSR_FE_MASK   BIT(3)
#define UART_LSR_PE_MASK   BIT(2)
#define UART_LSR_OE_MASK   BIT(1)
#define UART_LSR_DR_MASK   BIT(0)

#define UART_MSR_RESETVALUE 0

#define UART_SCR_RESETVALUE     0
#define UART_SOFT_BUFFER_LENGTH 512 // soft buffer length

typedef struct {
	volatile uint32_t RBR_THR_DLL;
	volatile uint32_t DLH_IER;
	union {
		volatile uint32_t IIR;
		volatile uint32_t FCR;
	};
	volatile uint32_t LCR;
	volatile uint32_t MCR;
	volatile uint32_t LSR;
	volatile uint32_t MSR;
	volatile uint32_t SCR;
	volatile uint32_t reserved0[4];
	volatile uint32_t SRBR[16];
	volatile uint32_t FAR;
	volatile uint32_t TFR;
	volatile uint32_t RFW;
	volatile uint32_t USR;
	volatile uint32_t TFL;
	volatile uint32_t RFL;
	volatile uint32_t SRR;
	volatile uint32_t SRTS;
	volatile uint32_t SBCR;
	volatile uint32_t SDMAM;
	volatile uint32_t SFE;
	volatile uint32_t SRT;
	volatile uint32_t STET;
	volatile uint32_t HTX;
	volatile uint32_t DMASA;
	volatile uint32_t TCR;
	volatile uint32_t DE_EN;
	volatile uint32_t RE_EN;
	volatile uint32_t DET;
	volatile uint32_t TAT;
	volatile uint32_t DLF;
	volatile uint32_t reservrd1[12];
	volatile uint32_t CPR;
	volatile uint32_t UCV;
	volatile uint32_t CTR;
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
	unsigned int read_position;
	unsigned int write_position;
	int overflow;
	int lock;
	unsigned char buffer[UART_SOFT_BUFFER_LENGTH];
} uart_buffer_t;

typedef struct {
	uart_reg_t *uart_ptr; // The pointer for UART controller registers
	uint32_t uartNum; // The number of UART in system
	uint32_t baudrate; // The transmission speed
	uart_data_len_t bits; // The data length
	uart_stop_bit stopBit; // The number stop bits
	uart_parity_t parity; // The parity control
	uint32_t mode; // The mode of work functions uart deriver
	int linesplit; // The mode splite lines text flow
	uint32_t timeout; // The timeout cycles
	uint32_t error_receive; // Errors of reciever
} uart_param_t;

/**
 * @brief The function return pointer into uart controller
 *
 * @param uartNum Number of uart controller
 * @param uart_ptr The pointer to registers UART controller
 * @return mcom_err_t Error code
 */
mcom_err_t uart_drv_get_handler(uint32_t uartNum, uart_reg_t **uart_ptr);

/*!
 * \brief The function set UART controller configuration
 * \param uart The pointer to structure uart_param_t. Uart_context describes of parametrs uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_config(uart_param_t *uart);

/*!
 * \brief The function set default configuration of UART controller
 * \param huart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_config_default(uart_param_t *huart);

/*!
 * \brief The function disable transfering, receiving, interrupts
 * \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
 * \return mcom_err_t Error code
 */
mcom_err_t uart_drv_deinit(uart_param_t *uart);

/*!
* \brief The function get char from rx buffer UART controller. This function receive text strings.
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \param c Pointer for reading from rx buffer
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_getchar(uart_param_t *uart, int *c);

/*!
* \brief The function put char direct to transmit register UART controller
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \param c char for transmit
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_putchar(uart_param_t *uart, char c);

/*!
* \brief The function used to flush the tx buffer immediately
* \param uart The pointer to structure uart_param_t. Structure uart_param_t describes parametrs of uart controller
* \return mcom_err_t Error code
*/
mcom_err_t uart_drv_flush(uart_param_t *uart);

#endif /* UART_H */
