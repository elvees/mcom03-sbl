// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __TL_SERVICES_API_H__
#define __TL_SERVICES_API_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <tl_services/tl_services.h>

uint32_t tl_services_start(void);
uint32_t tl_services_stop(void);
void tl_services_handler(void);

#endif /* __TL_SERVICES_API_H__ */
