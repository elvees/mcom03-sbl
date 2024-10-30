// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <malloc.h>
#include <stddef.h>

#include <third-party/llist/llist.h>

#include "queue.h"

void queue_push(queue_t *queue, void *data)
{
	if (!queue || !data)
		return;

	struct llist *node = malloc(sizeof(struct llist));
	if (!node)
		return;

	node->data = data;

	if (!queue->head && !queue->tail) {
		// List is empty - assign new element to list.
		node->prev = NULL;
		node->next = NULL;
		queue->head = node;
		queue->tail = node;
	} else {
		node->prev = queue->head;
		node->next = NULL;
		queue->head->next = node;
		queue->head = node;
	}
}

void *queue_pop(queue_t *queue)
{
	struct llist *node;
	void *data = NULL;

	if (queue && queue->head && queue->tail) {
		node = queue->tail;
		data = node->data;
		if (queue->tail == queue->head) {
			// Last element has to be removed.
			queue->tail = NULL;
			queue->head = NULL;
		} else {
			queue->tail = queue->tail->next;
			queue->tail->prev = NULL;
		}
		free(node);
	}

	return data;
}
