// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#define EMPTY_L     1
#define NOT_EMPTY_L 0
#define FULL_L      1
#define NOT_FULL_L  0

#define FIFO0 0
#define FIFO1 1
#define FIFO2 2
#define FIFO3 3
#define FIFO4 4
#define FIFO5 5
#define FIFO6 6
#define FIFO7 7

#define MAILBOX_FIFO_COUNT    8
#define MAILBOX_MAX_FIFO_SIZE 8

#define MBOX_MAX_RETRIES 100

typedef union {
	volatile unsigned int value;
	struct {
		volatile unsigned int full : 1;
		volatile unsigned int empty : 1;
		volatile unsigned int : 2;
		volatile unsigned int status : 4;
		volatile unsigned int : 24;
	};
} mailbox_fifo_status_t;

typedef struct {
	volatile unsigned int message;
	mailbox_fifo_status_t fifo_status;
	volatile unsigned int reserved0[2];
	volatile unsigned int reserved1;
	volatile unsigned int set_irq_read;
	volatile unsigned int reserved2;
	volatile unsigned int clr_irq_read;
	volatile unsigned int reserved3[1016];
} mbox_fifo_regs_t;

typedef struct {
	mbox_fifo_regs_t mailbox[MAILBOX_FIFO_COUNT];
	volatile unsigned int soft_reset;
} mailbox_regs_t;

typedef struct {
	mailbox_regs_t *mbox;
	int fifo;
} mbox_channel;

typedef void (*mbox_irq_handler_t)(unsigned int fifo_num);

/**
 * @brief The function returns pointer to mailbox registers
 *
 * @return Pointer to mailbox registers
 */
mailbox_regs_t *mbox_get_regs(void);

/**
 * @brief Inits mailbox device
 *
 * @param regs - Pointer to mailbox registers
 *
 * @return  0     - Success,
 *         -ENULL - regs is not provided (NULL pointers)
 */
int mbox_init(mailbox_regs_t *regs);

/**
 * @brief Deinits mailbox device
 *
 * @param regs - Pointer to mailbox registers
 *
 * @return  0     - Success,
 *         -ENULL - regs is not provided (NULL pointers)
 */
int mbox_deinit(mailbox_regs_t *regs);

/**
 * @brief The function returns status empty fifo
 *
 * @param mbox - Pointer to mailbox fifo registers
 *
 * @return -ENULL - mbox is not provided (NULL pointers),
 *          1     - Fifo mailbox is empty,
 *          0     - Fifo mailbox is not empty
 */
int mbox_is_empty(mbox_fifo_regs_t *mbox);

/**
 * @brief The function registers handler to mailbox event
 *
 * @param mbox_irq_handler - Handler
 *
 * @return  0             - Success,
 *         -ENULL         - Can not allocate memory for new node,
 *         -EINVALIDSTATE - Can not find lowest free address
 */
int mbox_attach_irq_handler(mbox_irq_handler_t mbox_irq_handler);

/**
 * @brief The function unregisters handler to mailbox event
 *
 * @param mbox_irq_handler - Handler
 *
 * @return 0 - Success
 */
int mbox_detach_irq_handler(mbox_irq_handler_t mbox_irq_handler);

/**
 * @brief The function writes data to mailbox
 *
 * @param mbox    - Pointer to mailbox fifo registers
 * @param message - Pointer to message to send to mbox
 * @param size    - Actual size of message in bytes
 *
 * @return The number of successfully written bytes
 */
unsigned int mbox_write(mbox_fifo_regs_t *mbox, const char *message, unsigned int size);

/**
 * @brief The function reads data from mailbox
 *
 * @param mbox    - Pointer to mailbox fifo registers
 * @param message - Pointer to message received from mbox
 * @param size    - Expected size of message in bytes
 *
 * @return The number of successfully read bytes
 */
unsigned int mbox_read(mbox_fifo_regs_t *mbox, char *message, unsigned int size);
