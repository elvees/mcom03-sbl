// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <drivers/pll/pll.h>
#include <drivers/service/service.h>
#include <drivers/timer/timer.h>
#include <libs/helpers/helpers.h>
#include <libs/mmio.h>
#include <libs/utils-def.h>

#include "otp.h"

#define OTP_MODE_DCTRL     BIT(1)
#define OTP_MODE_DCTRL_OFF 0
#define OTP_MODE_DCTRL_ON  1

#define OTP_MODE_PD     BIT(2)
#define OTP_MODE_PD_OFF 0
#define OTP_MODE_PD_ON  1

#define OTP_FLAG_BD_STATUS      BIT(1)
#define OTP_FLAG_BD_STATUS_BUSY 0
#define OTP_FLAG_BD_STATUS_COMP 1

#define OTP_BASE BASE_ADDR_SERVICE_OTP

COMPILE_TIME_ASSERT(!(sizeof(otp_t) % sizeof(uint32_t)));

// It is public instance as it has to be visible for debugger
otp_t otp_dump;

static pll_cfg_t pll_cfg_to_restore = { 0 };

static int otp_enable_read(void)
{
	uint32_t val;
	int ret;

	service_urb_regs_t *urb = service_get_urb_registers();

	// Store current PLL settings
	ret = pll_get_freq((pll_cfg_reg_t *)(&urb->service_subs_pllcnfg), &pll_cfg_to_restore);
	if (ret)
		return ret;

	/**
	 * According to requirements on OTP read, the UCG1 APB clock has to be less than 25MHz.
	 * So, setup PLL to 135 MHz without changing UCG1 dividers to get UCG1 APB clock equal
	 * to 22.5 MHz, assuming that XTI = 27 MHz. Use NR = 0 to minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 59;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 11;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)(&urb->service_subs_pllcnfg), &pll_cfg, 1000);
	if (ret)
		return ret;

	// Enable the direct OTP access
	ret = read_poll_timeout(mmio_read_32, val,
	                        FIELD_GET(OTP_FLAG_BD_STATUS, val) == OTP_FLAG_BD_STATUS_COMP, 0,
	                        1000, (uintptr_t)&urb->otp_flag);
	if (ret)
		return ret;

	val = mmio_read_32((uintptr_t)&urb->otp_mode);
	val &= ~(OTP_MODE_DCTRL | OTP_MODE_PD);
	val |= FIELD_PREP(OTP_MODE_DCTRL, OTP_MODE_DCTRL_ON) |
	       FIELD_PREP(OTP_MODE_PD, OTP_MODE_PD_OFF);
	mmio_write_32((uintptr_t)&urb->otp_mode, val);

	// Now we can directly read OTP

	return 0;
}

static int otp_disable_read(void)
{
	uint32_t val;
	int ret;

	service_urb_regs_t *urb = service_get_urb_registers();

	// Disable the direct OTP access
	val = mmio_read_32((uintptr_t)&urb->otp_mode);
	val &= ~(OTP_MODE_DCTRL | OTP_MODE_PD);
	val |= FIELD_PREP(OTP_MODE_DCTRL, OTP_MODE_DCTRL_OFF) |
	       FIELD_PREP(OTP_MODE_PD, OTP_MODE_PD_ON);
	mmio_write_32((uintptr_t)&urb->otp_mode, val);

	// Restore current PLL settings
	ret = pll_set_manual_freq((pll_cfg_reg_t *)(&urb->service_subs_pllcnfg),
	                          &pll_cfg_to_restore, 1000);
	if (ret)
		return ret;

	return 0;
}

otp_t *otp_get_dump(void)
{
	int ret;

	static bool is_read = false;
	if (!is_read) {
		int count = sizeof(otp_dump) / sizeof(uint32_t);
		uint32_t *potp_dump = (uint32_t *)&otp_dump;
		uint32_t *potp = (uint32_t *)OTP_BASE;

		ret = otp_enable_read();
		if (ret)
			return NULL;

		// Make OTP dump. Only word addressing is allowed.
		for (; count; potp_dump++, potp++, count--)
			*potp_dump = *potp;

		// Ensure the above instructions are completed.
		mem_barrier();

		/* Make label to access otp in MDB scripts using the
		 * label as breakpoint after dump is made.
		 */
		__asm__ volatile(".global jtag_otp\njtag_otp: nop" ::);

		ret = otp_disable_read();
		if (ret)
			return NULL;

		is_read = true;
	}

	return (otp_t *)&otp_dump;
}

void otp_clean_dump(void)
{
	memset((void *)&otp_dump, 0, sizeof(otp_dump));
}
