// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <third-party/llist/llist.h>

typedef struct {
	struct llist *head; // queue head
	struct llist *tail; // queue tail
} queue_t;

/**
 * @brief The function places new data at the head of the list
 *
 * @param queue - Pointer to queue
 * @param data  - Pointer to data to add
 */
void queue_push(queue_t *queue, void *data);

/**
 * @brief The function gets the data from the tail of list
 *
 * @param queue - Pointer to queue
 *
 * @return The data from the tail of list
 */
void *queue_pop(queue_t *queue);
