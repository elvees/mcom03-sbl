// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <mmio.h>
#include <mcom03.h>
#include <mcom03-errors.h>

#include "library/uart-printf.h"

#include "drivers/gpio.h"
#include "drivers/hs-periph-urb.h"
#include "drivers/ls-periph1-urb.h"
#include "drivers/pll.h"
#include "drivers/service-urb.h"
#include "drivers/uart.h"
#include "drivers/ucg.h"
#include "drivers/wdt.h"

#define PREFIX "SBL-S1"

#define UART0_PORT     GPIO_PORTB
#define UART0_SOUT_PIN GPIO_PIN_6
#define UART0_SIN_PIN  GPIO_PIN_7

#define UART1_PORT     GPIO_PORTB
#define UART1_SOUT_PIN GPIO_PIN_6
#define UART1_SIN_PIN  GPIO_PIN_5

#define INTERCONNECT_PLL_ADDR (BASE_ADDR_TOP_URB_BASE)
#define SERVICE_PLL_ADDR      (BASE_ADDR_SERVICE_URB + 0x1000)

struct ucg_channel {
	int ucg_id;
	int chan_id;
	int div;
	bool enable;
};

/* Interconnect PLL output frequency is 1188 MHz, assuming that XTI = 27 MHz */
static struct ucg_channel interconnect_ucg_channels[] = {
	{ 0, 0, 6, true }, /* DP           198 MHz */
	{ 0, 1, 4, true }, /* VPU          297 MHz */
	{ 0, 2, 4, true }, /* GPU          297 MHz */
	{ 0, 3, 6, true }, /* ISP          198 MHz */
	{ 0, 4, 2, true }, /* CPU          594 MHz */
	{ 0, 5, 4, true }, /* ACP          297 MHz */
	{ 0, 6, 12, true }, /* LSP0          99 MHz */
	{ 0, 7, 2, true }, /* COH_COMM    594  MHz */
	/* To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{ 1, 0, 30, true }, /* SLOW_COMM    39.6 MHz */
	{ 1, 2, 8, true }, /* FAST_COMM   148.5 MHz */
	{ 1, 4, 2, true }, /* DSP         594   MHz */
	{ 1, 5, 4, true }, /* PCIe        297   MHz */
	{ 1, 6, 12, true }, /* LSP1         99   MHz */
	{ 1, 7, 8, true }, /* SERVICE     148.5 MHz */
	{ 1, 8, 6, true }, /* HSP         198   MHz */
};

/* Service Subsystem PLL output frequency is 594 MHz, assuming that XTI = 27 MHz */
/* Commenting any line below means "Don't touch", keep as is */
static struct ucg_channel mcom03_risc0_ucg_param[] = {
	{ 1, 0, 6, true }, /* SERVICE UCG1 APB       99 MHz */
	{ 1, 1, 1, true }, /* SERVICE UCG1 CORE      594 MHz */
	{ 1, 2, 1, true }, /* SERVICE UCG1 QSPI0     594 MHz */
	{ 1, 4, 1, true }, /* SERVICE UCG1 RISC0     594 MHz */
	{ 1, 5, 6, true }, /* SERVICE UCG1 MFBSP0    99 MHz */
	{ 1, 6, 6, true }, /* SERVICE UCG1 MFBSP1    99 MHz */
	{ 1, 7, 6, true }, /* SERVICE UCG1 MAILBOX0  99 MHz */
	{ 1, 8, 6, true }, /* SERVICE UCG1 PVTCTR    99 MHz */
	{ 1, 9, 6, true }, /* SERVICE UCG1 I2C4      99 MHz */
	{ 1, 10, 6, true }, /* SERVICE UCG1 TRNG      99 MHz */
	{ 1, 11, 6, true }, /* SERVICE UCG1 SPIOTP    99 MHz */
	{ 1, 12, 12, true }, /* SERVICE UCG1 I2C4_EXT  49.5 MHz */
	{ 1, 13, 22, true }, /* SERVICE UCG1 QSPI0_EXT 27 MHz */
};

#ifdef WDT_USE
static wdt_dev_t wdt0;
static uint32_t apb_freq;

void wdt_set_config(wdt_dev_t *wdt_dev, uint32_t wdt_clk, uint32_t timeout);
mcom_err_t service_get_apb_clock(uint32_t *apb_freq);
#endif /* WDT_USE */

#ifdef PRESETUP_UARTS
static uart_param_t uart;

mcom_err_t uart_subs_init(void);
#endif /* PRESETUP_UARTS */

mcom_err_t soc_debug_disable(void);
mcom_err_t service_set_clock(void);
mcom_err_t interconnect_set_clock(void);
mcom_err_t ucg_hsp_refclk_setup(void);
mcom_err_t ucg_lsp1_i2s_rstn(void);

int main(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	/* Initialize and configure the TOP clock gate */
	uint32_t top_clkgate = (SERVICE_TOP_CLK_GATE_SERVICE) | (SERVICE_TOP_CLK_GATE_LSPERIPH0) |
	                       (SERVICE_TOP_CLK_GATE_LSPERIPH1) | (SERVICE_TOP_CLK_GATE_HSPERIPH) |
	                       (SERVICE_TOP_CLK_GATE_DDR) | (SERVICE_TOP_CLK_GATE_TOP_INTERCONNECT);

	mmio_write_32(SERV_URB_TOP_CLKGATE, top_clkgate);

#ifdef PRESETUP_UARTS
	/* Initialize LS Peripheral 0 and 1 for UART 0 and 1 */
	ret_code = uart_subs_init();
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

	ret_code = uart_drv_config_default(&uart);
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

	uart_printf(&uart, PREFIX " (" __DATE__ " - " __TIME__ "): " COMMIT "\r\n");
#ifdef BUILD_ID
	uart_printf(&uart, PREFIX ": Build: %s\r\n", BUILD_ID);
#endif

#endif /* PRESETUP_UARTS */

	/* Initialize and configure the RISC0 clocking system */
	ret_code = service_set_clock();
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

#ifdef WDT_USE
	ret_code = service_get_apb_clock(&apb_freq);
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

	/* Initialize and configure the watchdog */
	wdt_set_config(&wdt0, apb_freq, WDT_MAX_TIMEOUT);

	ret_code = wdt_init(&wdt0);
	if ((ret_code != MCOM03_SUCCESS) && (ret_code != MCOM03_ERROR_ALREADY_INITIALIZED)) {
		goto exit;
	}

#ifdef PRESETUP_UARTS
	if (wdt_is_enabled(&wdt0))
		uart_printf(&uart, "WDT0 is already enabled\r\n");
#endif /* PRESETUP_UARTS */

	ret_code = wdt_start(&wdt0);
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}
#endif /* WDT_USE */

	/* Initialize and configure the InterConnect clocking system */
	ret_code = interconnect_set_clock();
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

	/* Disabling the debugging subsystem */
	ret_code = soc_debug_disable();
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}

	/*
	 * This is a temporary solution.
	 * All clocks should be configured by clock driver.
	 */
	ucg_hsp_refclk_setup();

	/* I2S RSTN must be enabled before LSP1 UCGs setup */
	ucg_lsp1_i2s_rstn();

	/* TODO: May be necessary to block writing to the flash memory */

#ifdef WDT_USE
	/* Reconfigure the watchdog */
	wdt_set_config(&wdt0, apb_freq, 3000);
	ret_code = wdt_start(&wdt0);
	if (ret_code != MCOM03_SUCCESS) {
		goto exit;
	}
#endif /* WDT_USE */

	return 0;

exit:
	for (;;) {
		__asm__ __volatile__("nop");
	}
}

#ifdef WDT_USE
void wdt_set_config(wdt_dev_t *wdt_dev, uint32_t wdt_clk, uint32_t timeout)
{
	wdt_dev->id = WDT0;
	wdt_dev->rmod = WDT_RST_MODE;
	wdt_dev->rpl = WDT_RST_PULSE_LEN_2;
	wdt_dev->timeout = timeout;
	wdt_dev->wdt_freq = wdt_clk;
}
#endif /* WDT_USE */

#ifdef PRESETUP_UARTS
mcom_err_t uart_subs_init(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	/* Initialize and configure the TOP clock gate */
	uint32_t top_clkgate = mmio_read_32(SERV_URB_TOP_CLKGATE);

	top_clkgate |= (SERVICE_TOP_CLK_GATE_LSPERIPH0) | (SERVICE_TOP_CLK_GATE_LSPERIPH1) |
	               (SERVICE_TOP_CLK_GATE_HSPERIPH);

	mmio_write_32(SERV_URB_TOP_CLKGATE, top_clkgate);

	service_urb_regs_t *service_urb = service_get_urb_registers();

	/* Release reset signal LS Peripheral 0 */
	ret_code = service_reset_ls_periph0_deassert(service_urb, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	/* Release reset signal LS Peripheral 1 */
	ret_code = service_reset_ls_periph1_deassert(service_urb, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	/* Initialize the UCG register for clocking LS Peripheral 1 */
	ucg_regs_t *lsp1_ucg = ucg_get_ls_periph1_registers(0);
	uint32_t lsp1_ucg_mask = (1 << LS1_UCG_CLK_GPIO1) | (1 << LS1_UCG_CLK_UART0);

	ucg_enable_bp(lsp1_ucg, lsp1_ucg_mask);
	ret_code = ucg_set_divider(lsp1_ucg, LS1_UCG_CLK_GPIO1, 1, true, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;
	ret_code = ucg_set_divider(lsp1_ucg, LS1_UCG_CLK_UART0, 1, true, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;
	ucg_sync_and_disable_bp(lsp1_ucg, lsp1_ucg_mask);

	/* Initialize GPIO for UART 0 */
	gpio_regs_t *gpio1 = gpio_get_registers((void *)BASE_ADDR_LS1_GPIO1_BASE);
	gpio_init(gpio1, UART0_PORT, UART0_SIN_PIN, GPIO_MODE_HW, GPIO_DIR_INPUT);
	gpio_init(gpio1, UART0_PORT, UART0_SOUT_PIN, GPIO_MODE_HW, GPIO_DIR_OUTPUT);

	/* Initialize the UCG register for clocking LS Peripheral 0 */
	ucg_regs_t *lsp0_ucg = ucg_get_ls_periph0_registers(0);
	uint32_t lsp0_ucg_mask = (1 << LS0_UCG2_CLK_UART1) | (1 << LS0_UCG2_CLK_GPIO0);

	ucg_enable_bp(lsp0_ucg, lsp0_ucg_mask);
	ret_code = ucg_set_divider(lsp0_ucg, LS0_UCG2_CLK_UART1, 1, true, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;
	ret_code = ucg_set_divider(lsp0_ucg, LS0_UCG2_CLK_GPIO0, 1, true, 1000);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;
	ucg_sync_and_disable_bp(lsp0_ucg, lsp0_ucg_mask);

	/* Initialize GPIO for UART 1 */
	gpio_regs_t *gpio0 = gpio_get_registers((void *)BASE_ADDR_LS0_GPIO0_BASE);
	gpio_init(gpio0, UART1_PORT, UART1_SIN_PIN, GPIO_MODE_HW, GPIO_DIR_INPUT);
	gpio_init(gpio0, UART1_PORT, UART1_SOUT_PIN, GPIO_MODE_HW, GPIO_DIR_OUTPUT);

	return ret_code;
}
#endif /* PRESETUP_UARTS */

mcom_err_t service_set_clock(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	ucg_regs_t *ucg = ucg_get_service_registers(0);

	uint32_t ucg_ena_mask =
		(1 << SERVICE_UCG1_CHANNEL_CLK_APB) | (1 << SERVICE_UCG1_CHANNEL_CLK_CORE) |
		(1 << SERVICE_UCG1_CHANNEL_CLK_QSPI0) | (1 << SERVICE_UCG1_CHANNEL_CLK_RISC0) |
		(1 << SERVICE_UCG1_CHANNEL_CLK_MFBSP0) | (1 << SERVICE_UCG1_CHANNEL_CLK_MFBSP1) |
		(1 << SERVICE_UCG1_CHANNEL_CLK_MAILBOX0) | (1 << SERVICE_UCG1_CHANNEL_CLK_PVTCTR) |
		(1 << SERVICE_UCG1_CHANNEL_CLK_I2C4) | (1 << SERVICE_UCG1_CHANNEL_CLK_TRNG) |
		(1 << SERVICE_UCG1_CHANNEL_CLK_QSPI0_EXT);

	/* Enable UCG1 bypass */
	ucg_enable_bp(ucg, ucg_ena_mask);

	/* Setup PLL to 594 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 131;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 5;
	pll_cfg.inp_freq = MCOM03_XTI_CLK_HZ;
	pll_cfg.out_freq = 0;

	ret_code = pll_set_manual_freq((pll_cfg_reg_t *)(SERVICE_PLL_ADDR), &pll_cfg, 1000);
	if (ret_code != MCOM03_SUCCESS) {
		return ret_code;
	}

	/* Setup UCG1 Divider */
	for (int i = 0; i < ARRAY_SIZE(mcom03_risc0_ucg_param); i++) {
		ret_code = ucg_set_divider(ucg, mcom03_risc0_ucg_param[i].chan_id,
		                           mcom03_risc0_ucg_param[i].div,
		                           mcom03_risc0_ucg_param[i].enable, 1000);
		if (ret_code != MCOM03_SUCCESS) {
			return ret_code;
		}
	}

	/* Sync and disable UCG1 bypass */
	ucg_sync_and_disable_bp(ucg, ucg_ena_mask);

	return ret_code;
}

mcom_err_t service_get_apb_clock(uint32_t *apb_freq)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;
	uint32_t freq_mul = 0;
	uint32_t freq_div = 0;
	ucg_regs_t *ucg = ucg_get_service_registers(0);

	if (!apb_freq) {
		return MCOM03_ERROR_NULL;
	}

	ret_code = pll_get_freq_mult((pll_cfg_reg_t *)(SERVICE_PLL_ADDR), &freq_mul);
	if (ret_code != MCOM03_SUCCESS) {
		return ret_code;
	}

	ret_code = ucg_get_divider(ucg, SERVICE_UCG1_CHANNEL_CLK_APB, &freq_div);
	if (ret_code != MCOM03_SUCCESS) {
		return ret_code;
	}

	*apb_freq = ((MCOM03_XTI_CLK_HZ * freq_mul) / freq_div);

	return ret_code;
}

mcom_err_t interconnect_set_clock(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	/* Enable bypass for all channels */

	/* Enable UCG0 bypass */
	ucg_regs_t *interconnect_ucg0 = ucg_get_top_registers(0);
	ucg_enable_bp(interconnect_ucg0, TOP_UCG0_ALL_CH_MASK);

	/* Enable UCG1 bypass */
	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);
	ucg_enable_bp(interconnect_ucg1, TOP_UCG1_ALL_CH_MASK);

	/* Setup PLL to 1188 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 87;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 1;
	pll_cfg.inp_freq = MCOM03_XTI_CLK_HZ;
	pll_cfg.out_freq = 0;

	ret_code = pll_set_manual_freq((pll_cfg_reg_t *)INTERCONNECT_PLL_ADDR, &pll_cfg, 1000);
	if (ret_code != MCOM03_SUCCESS) {
		return ret_code;
	}

	/* Set dividers */
	for (int i = 0; i < ARRAY_SIZE(interconnect_ucg_channels); i++) {
		if (interconnect_ucg_channels[i].ucg_id == 0) {
			ret_code = ucg_set_divider(interconnect_ucg0,
			                           interconnect_ucg_channels[i].chan_id,
			                           interconnect_ucg_channels[i].div,
			                           interconnect_ucg_channels[i].enable, 1000);
			if (ret_code != MCOM03_SUCCESS) {
				return ret_code;
			}
		}
		if (interconnect_ucg_channels[i].ucg_id == 1) {
			ret_code = ucg_set_divider(interconnect_ucg1,
			                           interconnect_ucg_channels[i].chan_id,
			                           interconnect_ucg_channels[i].div,
			                           interconnect_ucg_channels[i].enable, 1000);
			if (ret_code != MCOM03_SUCCESS) {
				return ret_code;
			}
		}
	}

	/* Sync and disable UCG0 bypass */
	ucg_sync_and_disable_bp(interconnect_ucg0, TOP_UCG0_ALL_CH_MASK);

	/* Sync and disable UCG0 bypass */
	ucg_sync_and_disable_bp(interconnect_ucg1, TOP_UCG1_ALL_CH_MASK);

	return ret_code;
}

#ifdef DEBUG_IFACE_USE
mcom_err_t soc_debug_disable(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	return ret_code;
}
#else
mcom_err_t soc_debug_disable(void)
{
	mcom_err_t ret_code = MCOM03_SUCCESS;

	/* Check Clock Service Subs UCG1 CH3 & CH15 (BPAM & RISC0_TCK_UCG)*/
	ucg_regs_t *serv_ucg = ucg_get_service_registers(0);
	uint32_t bpam_div = 0;
	uint32_t risc0_tck_div = 0;
	bool bpam_enable = false;
	bool risc0_tck_enable = false;

	ret_code = ucg_get_state(serv_ucg, SERVICE_UCG1_CHANNEL_CLK_BPAM, &bpam_div, &bpam_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	ret_code = ucg_get_state(serv_ucg, SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG, &risc0_tck_div,
	                         &risc0_tck_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	/* Disable Clock Service Subs UCG1 CH3 & CH15 */
	if (bpam_enable || risc0_tck_enable) {
		uint32_t ucg_ena_mask = (1 << SERVICE_UCG1_CHANNEL_CLK_BPAM) |
		                        (1 << SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG);

		ucg_enable_bp(serv_ucg, ucg_ena_mask);

		ret_code = ucg_set_divider(serv_ucg, SERVICE_UCG1_CHANNEL_CLK_BPAM, bpam_div, false,
		                           1000);
		if (ret_code != MCOM03_SUCCESS) {
			return ret_code;
		}

		ret_code = ucg_set_divider(serv_ucg, SERVICE_UCG1_CHANNEL_RISC0_TCK_UCG,
		                           risc0_tck_div, false, 1000);
		if (ret_code != MCOM03_SUCCESS) {
			return ret_code;
		}

		ucg_sync_and_disable_bp(serv_ucg, ucg_ena_mask);
	}

	/* Disable Debugging in Service Subs URB Register */
	service_urb_regs_t *service_urb = service_get_urb_registers();
	bool is_dbg_enable = true;

	ret_code = service_get_tp_dbgen(service_urb, &is_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	if (is_dbg_enable != false) {
		ret_code = service_set_tp_dbgen(service_urb, false);
		if (ret_code != MCOM03_SUCCESS)
			return ret_code;
	}

	is_dbg_enable = true;
	ret_code = service_get_sdr_dbgen(service_urb, &is_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	if (is_dbg_enable != false) {
		ret_code = service_set_sdr_dbgen(service_urb, false);
		if (ret_code != MCOM03_SUCCESS)
			return ret_code;
	}

	is_dbg_enable = true;
	ret_code = service_get_sp_dbgen(service_urb, &is_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	if (is_dbg_enable != false) {
		ret_code = service_set_sp_dbgen(service_urb, false);
		if (ret_code != MCOM03_SUCCESS)
			return ret_code;
	}

	is_dbg_enable = true;
	ret_code = service_get_s_dbgen(service_urb, &is_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	if (is_dbg_enable != false) {
		ret_code = service_set_s_dbgen(service_urb, false);
		if (ret_code != MCOM03_SUCCESS)
			return ret_code;
	}

	is_dbg_enable = true;
	ret_code = service_get_ust_dbgen(service_urb, &is_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	if (is_dbg_enable != false) {
		ret_code = service_set_ust_dbgen(service_urb, false);
		if (ret_code != MCOM03_SUCCESS)
			return ret_code;
	}

	/* Check Clock HS Peripheral Subs UCG1 CH4 */
	ucg_regs_t *hsp_ucg = ucg_get_hs_periph_registers(1);
	uint32_t hsp_dbg_div = 0;
	bool hsp_dbg_enable = false;

	ret_code = ucg_get_state(hsp_ucg, HS_UCG1_CHANNEL_CLK_DBG, &hsp_dbg_div, &hsp_dbg_enable);
	if (ret_code != MCOM03_SUCCESS)
		return ret_code;

	/* Disable Clock HS Peripheral Subs UCG1 CH4 */
	if (hsp_dbg_enable) {
		uint32_t ucg_ena_mask = HS_UCG1_CHANNEL_CLK_DBG;

		ucg_enable_bp(hsp_ucg, ucg_ena_mask);

		ret_code =
			ucg_set_divider(hsp_ucg, HS_UCG1_CHANNEL_CLK_DBG, hsp_dbg_div, false, 1000);
		if (ret_code != MCOM03_SUCCESS) {
			return ret_code;
		}

		ucg_sync_and_disable_bp(hsp_ucg, ucg_ena_mask);
	}

	/* Disable Debugging in HS Peripheral Subs URB Register */
	hs_urb_regs_t *hs_urb = hs_periph_get_urb_registers();

	if ((hs_urb->dbg_ctr & HS_URB_DBG_CTR_MASK) != 0) {
		hs_urb->dbg_ctr = 0;
	}
	return ret_code;
}
#endif /* DEBUG_IFACE_USE */

mcom_err_t ucg_hsp_refclk_setup(void)
{
	hs_urb_regs_t *urb = hs_periph_get_urb_registers();
	urb->refclk = 0;
	return MCOM03_SUCCESS;
}

mcom_err_t ucg_lsp1_i2s_rstn(void)
{
	ls1_urb_regs_t *urb = ls1_periph_get_urb_registers();

	urb->i2s_ucg_rstn_ppolicy = 0x10;
	while ((urb->i2s_ucg_rstn_pstatus & 0x1f) != 0x10) {
	}

	return MCOM03_SUCCESS;
}
