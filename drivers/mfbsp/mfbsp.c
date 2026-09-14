// SPDX-License-Identifier: MIT
// Copyright 2026 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <stdint.h>

#include <drivers/mfbsp/mfbsp.h>
#include <drivers/service/service.h>

mfbsp_regs_t *mfbsp_get_registers(unsigned int mfbsp_idx)
{
	if (mfbsp_idx == 0)
		return (mfbsp_regs_t *)BASE_ADDR_SERVICE_MFBSP0;
	else if (mfbsp_idx == 1)
		return (mfbsp_regs_t *)BASE_ADDR_SERVICE_MFBSP1;
	else
		return (mfbsp_regs_t *)NULL;
}

void mfbsp_gpio_set_dir(mfbsp_regs_t *regs, uint32_t pins_mask, enum mfbsp_dir dir)
{
	if (dir == MFBSP_DIR_OUT)
		regs->DIR |= pins_mask;
	else
		regs->DIR &= ~pins_mask;
}

uint32_t mfbsp_gpio_get_all(mfbsp_regs_t *regs)
{
	return regs->DR;
}

unsigned int mfbsp_gpio_get(mfbsp_regs_t *regs, uint32_t pin)
{
	return !!(regs->DR & pin);
}
