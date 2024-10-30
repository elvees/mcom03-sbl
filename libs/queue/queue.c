// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include "queue.h"

#define QUEUE_NULL ((void *)0)

/*!
 * queue_push
 *
 * Places new element at the head of the list.
 *
 * @param head - list head
 * @param front - list front
 * @param node - new element to add
 *
 */
void queue_push(queue_t *queue, struct llist *node)
{
	if (node == QUEUE_NULL) {
		return;
	}

	if ((queue->head == QUEUE_NULL) && (queue->tail == QUEUE_NULL)) {
		/* List is empty - assign new element to list. */
		node->prev = QUEUE_NULL;
		node->next = QUEUE_NULL;
		queue->head = node;
		queue->tail = node;
	} else {
		node->prev = queue->head;
		node->next = QUEUE_NULL;
		queue->head->next = node;
		queue->head = node;
	}
}

/*!
 * queue_pop
 *
 * Get the element from the tail of list.
 *
 * @param head  - list head
 * @param front - list front
 *
 * @return the element from the tail of list.
 */
struct llist *queue_pop(queue_t *queue)
{
	struct llist *node;
	if ((queue->head == QUEUE_NULL) && (queue->tail == QUEUE_NULL)) {
		node = QUEUE_NULL;
	} else {
		node = queue->tail;
		if (queue->tail == queue->head) {
			/* Last element has to be removed. */
			queue->tail = QUEUE_NULL;
			queue->head = QUEUE_NULL;
		} else {
			queue->tail = queue->tail->next;
			queue->tail->prev = QUEUE_NULL;
		}
	}
	return node;
}
