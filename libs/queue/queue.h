// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <lib/llist/llist.h>

typedef struct {
	struct llist *head; /*!< queue head */
	struct llist *tail; /*!< queue tail */
} queue_t;

void queue_push(queue_t *queue, struct llist *node);
struct llist *queue_pop(queue_t *queue);

#endif /* __QUEUE_H__ */
