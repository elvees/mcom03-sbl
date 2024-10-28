// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <platform_def.h>
#include <platform_helpers.h>
#include "factory_reset.h"

int factory_reset_get_info(const void *fdt, factory_reset_info_t *info)
{
	int ret;
	int node_off, parent_node_off;
	int size;
	const fdt32_t *phandle;
	uint32_t phandle_cells = 0;
	uint64_t base;

	if (!fdt || !info)
		return -FDT_ERR_MAX;

	memset(info, 0, sizeof(*info));

	ret = fdt_check_header(fdt);
	if (ret)
		return ret;

	/*
	 * If we are not given the size, make do with calculating it.
	 * This is not as secure.
	 */
	ret = fdt_check_full(fdt, fdt_totalsize(fdt));
	if (ret)
		return ret;

	node_off = fdt_node_offset_by_compatible(fdt, -1, "gpio-keys");
	if (node_off < 0)
		return node_off;

	node_off = fdt_subnode_offset(fdt, node_off, "factory-reset");
	if (node_off < 0)
		return node_off;

	phandle = fdt_getprop(fdt, node_off, "gpios", &size);
	if (!phandle)
		return size;

	node_off = fdt_node_offset_by_phandle(fdt, fdt32_to_cpu(phandle[0]));
	if (node_off < 0)
		return node_off;

	ret = fdt_read_uint32(fdt, node_off, "#gpio-cells", &phandle_cells);
	if (ret)
		return ret;

	if (phandle_cells != 2)
		return -FDT_ERR_BADNCELLS;

	info->pin = fdt32_to_cpu(phandle[1]);
	info->active_low = fdt32_to_cpu(phandle[2]);

	ret = fdt_read_uint32(fdt, node_off, "reg", &info->port);
	if (ret)
		return ret;

	parent_node_off = fdt_parent_offset(fdt, node_off);
	ret = fdt_get_reg_props_by_index(fdt, parent_node_off, 0, &base, NULL);
	if (ret)
		return ret;

	info->gpio_regs = gpio_get_registers(plat_convert_pa_to_va(base));

	return 0;
}
