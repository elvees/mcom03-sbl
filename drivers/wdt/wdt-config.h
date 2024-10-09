// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include "wdt.h"

int wdt_set_config(wdt_dev_t *wdt_dev, uint32_t timeout);
wdt_dev_t *wdt_return_instance(void);
