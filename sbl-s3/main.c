// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <platform_helpers.h>

#include <tl_services/server/tl_services_api.h>

#include <drivers/service_subs/urb.h>
#include <drivers/iommu/iommu.h>
#include <drivers/mailbox/mailbox.h>
#include <drivers/intmgr/intmgr.h>
#include <drivers/nvm/nvm.h>
#include <drivers/pll/pll.h>
#include <drivers/ucg/ucg.h>

#define PFX "TL-CORE"

#ifdef WDT_ENABLE
#include <drivers/wdt/wdt.h>
wdt_dev_t wdt;
#endif /* WDT_ENABLE */

extern nvm_dev_t nvm_device;

int main(int argc, char **argv)
{
	int32_t ret;

	NOTICE(PFX " (" __DATE__ " - " __TIME__ "): " COMMIT "\r\n");
#ifdef BUILD_ID
	NOTICE(PFX ": Build: %s\r\n", BUILD_ID);
#endif

#ifdef WDT_ENABLE
	uint32_t freq_mul = 0;
	uint32_t freq_div = 0;

	service_urb_regs_t *ss_urb = getServiceURBRegisters();
	ret = pll_get_freq_mult((pll_cfg_reg_t *)&ss_urb->service_subs_pllcnfg, &freq_mul);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("Failed to get freq mult ret=%ld\r\n", ret);
	}

	ucg_regs_t *ss_ucg = ucg_get_service_registers(0);
	ret = ucg_get_divider(ss_ucg, SERVICE_UCG1_CHANNEL_CLK_APB, &freq_div);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("Failed to get ucg divider ret=%ld\r\n", ret);
	}

	wdt.id = WDT0;
	wdt.rmod = WDT_RST_MODE;
	wdt.rpl = WDT_RST_PULSE_LEN_2;
	wdt.timeout = WDT_MAX_TIMEOUT;
	wdt.wdt_freq = ((XTI_CLK_HZ * freq_mul) / freq_div);

	ret = wdt_init(&wdt);
	if ((ret != MCOM03_SUCCESS) && (ret != MCOM03_ERROR_ALREADY_INITIALIZED)) {
		plat_panic_handler("WDT init failed ret=%ld\r\n", ret);
	}

	ret = wdt_start(&wdt);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("WDT start failed ret=%ld\r\n", ret);
	}
#endif /* WDT_ENABLE */

	ret = nvm_deinit(&nvm_device);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("NVM deinitialization failed ret=%ld\r\n", ret);
	}
	ret = nvm_init(&nvm_device);
	if (ret != MCOM03_SUCCESS) {
		plat_panic_handler("NVM initialization failed ret=%ld\r\n", ret);
	}

	/* Initialize IOMMU */
	iommu_t *iommu_regs = iommu_get_registers();
	if (iommu_regs == NULL) {
		plat_panic_handler("Iommu init failed!\r\n");
	}

	iommu_deinit(iommu_regs);
	iommu_init(iommu_regs, (void *)(0x40000000));

	if ((ret = intmgr_init()) != 0) {
		plat_panic_handler("Intmgr init failed ret=%ld\r\n", ret);
	}

	mailbox_regs_t *regs = mbox_get_regs(MAILBOX0);
	if ((ret = mbox_init(regs)) != 0) {
		plat_panic_handler("Mailbox0 init failed ret=%ld\r\n", ret);
	}

	if ((ret = intmgr_global_enable_irq()) != 0) {
		plat_panic_handler("Global irq enabling failed ret=%ld\r\n", ret);
	}

	if ((ret = tl_services_start()) != 0) {
		plat_panic_handler("Services start failed ret=%ld\r\n", ret);
	}

	// start CPU using provided entrypoint
	// TODO: add macros to be able to change entrypoint during building
	service_enable_arm0_core(0x80300000, 0x08);

	for (;;) {
		tl_services_handler();

#if defined(WDT_ENABLE) && defined(WDT_ENABLE_RESET)
		wdt_reset(&wdt);
#endif /* WDT_ENABLE_RESET */

		__asm__ volatile("nop");
	}
}
