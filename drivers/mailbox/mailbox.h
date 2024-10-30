// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __MAILBOX_H__
#define __MAILBOX_H__

#define MAILBOX0 0
#define MAILBOX1 1

#define EMPTY_L	    1
#define NOT_EMPTY_L 0
#define FULL_L	    1
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

typedef union {
	unsigned int VALUE;
	struct {
		volatile unsigned int FULL : 1;
		volatile unsigned int EMPTY : 1;
		volatile unsigned int : 2;
		volatile unsigned int STATUS : 4;
		volatile unsigned int : 24;
	};
} mailbox_fifo_status_t;

typedef struct {
	volatile unsigned int MESSAGE;
	mailbox_fifo_status_t FIFO_STATUS;
	unsigned int RESERVED0[2];
	unsigned int RESERVED1; // volatile unsigned int SET_IRQ_WRITE;
	volatile unsigned int SET_IRQ_READ;
	unsigned int RESERVED2; // volatile unsigned int CLR_IRQ_WRITE;
	volatile unsigned int CLR_IRQ_READ;
	unsigned int RESERVED3[1016];
} mbox_fifo_regs_t;

typedef struct {
	mbox_fifo_regs_t mailbox[MAILBOX_FIFO_COUNT];
	volatile unsigned int SOFT_RESET;
} mailbox_regs_t;

typedef struct {
	mailbox_regs_t *mbox;
	int fifo;
} mbox_channel;

typedef void (*mbox_irq_handler_t)(unsigned int mboxNum);

mailbox_regs_t *mbox_get_regs(unsigned int mboxNum);

int mbox_init(mailbox_regs_t *mbox);
int mbox_deinit(mailbox_regs_t *regs);
int mbox_is_full(mbox_fifo_regs_t *mbox);
int mbox_is_empty(mbox_fifo_regs_t *mbox);
int mbox_status(mbox_fifo_regs_t *mbox);
unsigned int mbox_write(mbox_fifo_regs_t *mbox, const char *message, unsigned int size,
			unsigned int timeout);
unsigned int mbox_read(mbox_fifo_regs_t *mbox, char *message, unsigned int size,
		       unsigned int timeout);
int mbox_set_IRQread(mbox_fifo_regs_t *mbox, int irq);
int mbox_clr_IRQread(mbox_fifo_regs_t *mbox, int irq);
unsigned int mbox_get_IRQread(mbox_fifo_regs_t *mbox);
int mbox_attach_irq_handler(mailbox_regs_t *mbox, mbox_irq_handler_t mbox_irq_handler);
int mbox_detach_irq_handler(mailbox_regs_t *mbox, mbox_irq_handler_t mbox_irq_handler);
int mbox_enable_irq(mailbox_regs_t *mbox);
int mbox_disable_irq(mailbox_regs_t *mbox);

#endif /* __MAILBOX_H__ */
