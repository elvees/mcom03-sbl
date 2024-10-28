/*
 * Copyright (c) 2018-2023, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <lib/libfdt/libfdt.h>
#include "fdt_helpers.h"

static uint64_t fdt_read_prop_cells(const fdt32_t *prop, int cells)
{
	uint64_t reg = fdt32_to_cpu(prop[0]);

	if (cells > 1)
		reg = (reg << 32) | fdt32_to_cpu(prop[1]);

	return reg;
}

static int fdt_read_uint32_array(const void *fdt, int node_off, const char *prop_name,
				 unsigned int cells, uint32_t *value)
{
	const fdt32_t *prop;
	int value_len;

	if (!fdt || !prop_name || !value || node_off < 0)
		return -FDT_ERR_MAX;

	/* Access property and obtain its length (in bytes) */
	prop = fdt_getprop(fdt, node_off, prop_name, &value_len);
	if (prop == NULL)
		return -FDT_ERR_NOTFOUND;

	/* Verify that property length can fill the entire array. */
	if ((unsigned int)value_len < cells * sizeof(uint32_t))
		return -FDT_ERR_BADVALUE;

	for (unsigned int i = 0U; i < cells; i++)
		value[i] = fdt32_to_cpu(prop[i]);

	return 0;
}

int fdt_read_uint32(const void *fdt, int node_off, const char *prop_name, uint32_t *value)
{
	return fdt_read_uint32_array(fdt, node_off, prop_name, 1, value);
}

int fdt_get_reg_props_by_index(const void *fdt, int node_off, int index, uint64_t *base, int *size)
{
	const fdt32_t *prop;
	int parent_off, len;
	int ac, sc;
	int cell;

	if (!fdt || node_off < 0)
		return -FDT_ERR_MAX;

	parent_off = fdt_parent_offset(fdt, node_off);
	if (parent_off < 0)
		return -FDT_ERR_BADOFFSET;

	ac = fdt_address_cells(fdt, parent_off);
	sc = fdt_size_cells(fdt, parent_off);

	cell = index * (ac + sc);

	prop = fdt_getprop(fdt, node_off, "reg", &len);
	if (!prop)
		return -FDT_ERR_NOTFOUND;

	if (((cell + ac + sc) * (int)sizeof(uint32_t)) > len)
		return -FDT_ERR_BADVALUE;

	if (base)
		*base = (uint64_t)fdt_read_prop_cells(&prop[cell], ac);

	if (size)
		*size = (int)fdt_read_prop_cells(&prop[cell + ac], sc);

	return 0;
}
