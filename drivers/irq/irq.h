// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

typedef void (*irq_handler_t)(unsigned int id);

int32_t irq_init(void);
int32_t irq_deinit(void);
int32_t irq_attach_handler(unsigned int id, irq_handler_t handler);
int32_t irq_detach_handler(unsigned int id);
void irq_handler(void);
