// SPDX-License-Identifier: MIT
// Copyright 2024 RnD Center "ELVEES", JSC

#include <stdint.h>

#include <drivers/service/service.h>
#include <libs/errors.h>

#include "wdt-config.h"
#include "wdt.h"

static wdt_dev_t wdt_dev;

int wdt_set_config(wdt_dev_t *wdt_dev, uint32_t timeout)
{
	int ret;
	uint32_t apb_freq;

	ret = service_get_apb_clock(&apb_freq);
	if (ret)
		return ret;

	wdt_dev->rmod = WDT_RST_MODE;
	wdt_dev->rpl = WDT_RST_PULSE_LEN_2;
	wdt_dev->timeout = timeout;
	wdt_dev->wdt_freq = apb_freq;

	return 0;
}

wdt_dev_t *wdt_return_instance(void)
{
	return &wdt_dev;
}
