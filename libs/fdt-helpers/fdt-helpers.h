// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __FDT_HELPERS_H__
#define __FDT_HELPERS_H__

/**
 * fdt_read_uint32() - Read single UINT32 value of specified node of DTB
 *
 * @fdt:         Pointer to DTB blob
 * @node_off:    Node offset
 * @prop_name:   Property name
 * @value:       Output value
 *
 * Returns:      -FDT_ERR_MAX in case of bad pointer or negative node offset are provided;
 *               -FDT_ERR_NOTFOUND if property is not found;
 *               -FDT_ERR_BADVALUE if value exceeds UINT32 size;
 *                0 in case of successful execution.
 */
int fdt_read_uint32(const void *fdt, int node_off, const char *prop_name, uint32_t *value);

/**
 * fdt_get_reg_props_by_index() - Read "reg" property by index of specified node of DTB
 *
 * @fdt:         Pointer to DTB blob
 * @node_off:    Node offset
 * @index:       Property name
 * @value:       Output value
 *
 * Returns:      -FDT_ERR_MAX in case of bad pointer or negative node offset are provided;
 *               -FDT_ERR_BADOFFSET if bad node offset is provided;
 *               -FDT_ERR_NOTFOUND if "reg" property is not found using provided node offset;
 *               -FDT_ERR_BADVALUE if value exceeds "reg" property len;
 *                0 in case of successful execution.
 */
int fdt_get_reg_props_by_index(const void *fdt, int node_off, int index, uint64_t *base, int *size);

#endif /* __FDT_HELPERS_H__ */
