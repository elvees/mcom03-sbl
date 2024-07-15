// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include "top.h"

top_urb_regs_t *top_get_urb_registers(void)
{
	return (top_urb_regs_t *)(BASE_ADDR_TOP_URB_BASE + TOP_ADDR_URB_OFFSET);
}

void top_set_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->NONSECURETOSECURE = mask;
}

void top_reset_nonsecure_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->NONSECURETOSECURE &= ((~mask) & TOP_NONSECURETOSECURE_MASK);
}

void top_set_sdr_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->SDRTOSECURE = mask;
}

void top_reset_sdr_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->SDRTOSECURE &= ((~mask) & TOP_SDRTOSECURE_MASK);
}

void top_set_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->TRUSTEDTOSECURE = mask;
}

void top_reset_trusted_to_secure(top_urb_regs_t *urb, unsigned int mask)
{
	urb->TRUSTEDTOSECURE &= ((~mask) & TOP_TRUSTEDTOSECURE_MASK);
}

void top_set_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask)
{
	urb->DDR_LOW_RANGE_SECURE_CTR = mask;
}

void top_reset_low_ddr_secure_bit(top_urb_regs_t *urb, unsigned int mask)
{
	urb->DDR_LOW_RANGE_SECURE_CTR &= ((~mask) & TOP_DDR_LOW_RANGE_SECURE_CTR_MASK);
}
