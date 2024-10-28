// Copyright 2023-2025 RnD Center "ELVEES", JSC
// Copyright (c) 2018-2021, ARM Limited and Contributors. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <stdint.h>

/**
 * @brief Read single UINT32 value of specified node of DTB
 *
 * @param fdt       - Pointer to DTB blob
 * @param node_off  - Node offset
 * @param prop_name - Property name
 * @param value     - Output value
 *
 * @return  0                - Success,
 *         -FDT_ERR_MAX      - Bad pointer or negative node offset are provided,
 *         -FDT_ERR_NOTFOUND - Property is not found,
 *         -FDT_ERR_BADVALUE - Value exceeds UINT32 size
 */
int fdt_read_uint32(const void *fdt, int node_off, const char *prop_name, uint32_t *value);

/**
 * @brief Read "reg" property by index of specified node of DTB
 *
 * @param fdt      - Pointer to DTB blob
 * @param node_off - Node offset
 * @param index    - Property name
 * @param value    - Output value
 *
 * @return  0                 - Success,
 *         -FDT_ERR_MAX       - Bad pointer or negative node offset are provided,
 *         -FDT_ERR_BADOFFSET - Bad node offset is provided,
 *         -FDT_ERR_NOTFOUND  - "Reg" property is not found using provided node offset,
 *         -FDT_ERR_BADVALUE  - Value exceeds "reg" property len
 */
int fdt_get_reg_props_by_index(const void *fdt, int node_off, int index, uint64_t *base, int *size);
