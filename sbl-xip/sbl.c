// Copyright 2020-2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <string.h>
#include <common.h>
#include "printf.h"
#include "mips/m32c0.h"
#include "bitops.h"
#include "ucg.h"
#include "pll.h"
#include "regs.h"

#define PREFIX "SBL-XIP"

#define INFO(msg, ...) printf(PREFIX ": " msg, ##__VA_ARGS__)

#define PP_OFF      0x01
#define PP_WARM_RST 0x08
#define PP_ON       0x10

#define DDRINIT_START_ADDR_VIRT 0xA0000000
#define TFA_START_ADDR_VIRT     0xC0300000
#define TFA_START_ADDR_PHYS     0x880300000
#define BL32_START_ADDR_VIRT    0xC1380000
#define UBOOT_START_ADDR_VIRT   0xC0080000
#define UBOOT_DTB_ADDR_VIRT     0xC0002000

#define TIMER_FREQ 27000000
#define USEC       1ULL
#define MSEC       1000ULL
#define SEC        1000000ULL

#define UART0_SOUT_PIN    GPIO_PIN_6
#define UART0_SIN_PIN     GPIO_PIN_7
#define UART0_LCR_DEFAULT 3 /* 8 bit, no parity, 1 stop bit. */
#define UART0_BASE_FREQ   27000000
#define UART0_BAUDRATE    115200

#define DIV_ROUND_UP(n, d) (((n) + (d)-1) / (d))
#define DIV_ROUND_CLOSEST(x, divisor)                                             \
	({                                                                        \
		typeof(x) __x = x;                                                \
		typeof(divisor) __d = divisor;                                    \
		(((typeof(x))-1) > 0 || ((typeof(divisor))-1) > 0 || (__x) > 0) ? \
			(((__x) + ((__d) / 2)) / (__d)) :                         \
			(((__x) - ((__d) / 2)) / (__d));                          \
	})

#define read_poll_timeout(op, val, cond, sleep_us, timeout_us, args...) \
	({                                                              \
		int timeout = timer_get_usec() + timeout_us;            \
		for (;;) {                                              \
			(val) = op(args);                               \
			if (cond)                                       \
				break;                                  \
			if (timer_get_usec() > timeout) {               \
				(val) = op(args);                       \
				break;                                  \
			}                                               \
			if (sleep_us)                                   \
				delay_usec(sleep_us);                   \
		}                                                       \
		(cond) ? 0 : -1;                                        \
	})

#define readl_poll_timeout(val, cond, sleep_us, timeout_us, addr) \
	read_poll_timeout(readl, val, cond, sleep_us, timeout_us, addr)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define WRITE_CPU_START_ADDR_REG(reg, val)                         \
	do {                                                       \
		writel((reg), ((uint64_t)(val)) >> 32);            \
		writel((reg) + 4, ((uint64_t)(val)) & 0xffffffff); \
	} while (0)

extern unsigned long __ddrinit_start, __ddrinit_end;
extern unsigned long __tfa_start, __tfa_end;
extern unsigned long __dtb_start, __dtb_end;
#if defined(HAS_BL32)
extern unsigned long __bl32_start, __bl32_end;
#endif
extern unsigned long __uboot_start, __uboot_end;

struct ucg_channel {
	int ucg_id;
	int chan_id;
	int div;
};

static struct ucg_channel top_ucg_channels[] = {
	{ 0, TOP_UCG0_CHANNEL_DDR_DP, 6 }, /* 198 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_VPU, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_GPU, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_ISP, 6 }, /* 198 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_CPU, 2 }, /* 594 MHz */
	{ 0, TOP_UCG0_CHANNEL_CPU_ACP, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0, 12 }, /* 99 MHz */
	{ 0, TOP_UCG0_CHANNEL_AXI_COH_COMM, 2 }, /* 594 MHz */

	/* To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{ 1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM, 30 }, /* 39.6 MHz */
	{ 1, TOP_UCG1_CHANNEL_AXI_FAST_COMM, 8 }, /* 148.5 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_DSP, 2 }, /* 594 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_PICE, 4 }, /* 297 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_LSPERIPH1, 12 }, /* 99 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SERVICE, 8 }, /* 148.5 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_HSPERIPH, 6 }, /* 198 MHz */
};

/* Service Subsystem PLL output frequency is 594 MHz, assuming that XTI = 27 MHz */
static struct ucg_channel serv_ucg_channels[] = {
	{ 1, SERV_UCG1_CHANNEL_CLK_APB, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_CORE, 1 }, /* 594 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_QSPI0, 1 }, /* 594 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_BPAM, 1 }, /* 594 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_RISC0, 1 }, /* 594 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_MFBSP0, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_MFBSP1, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_MAILBOX0, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_PVTCTR, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_I2C4, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_TRNG, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_SPIOTP, 6 }, /* 99 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_I2C4_EXT, 12 }, /* 49.5 MHz */
	{ 1, SERV_UCG1_CHANNEL_CLK_QSPI0_EXT, 22 }, /* 27 MHz */
};

/* CPU Subsystem PLL output frequency is 1161 MHz, assuming that XTI = 27 MHz */
static struct ucg_channel cpu_ucg_channels[] = {
	{ 0, CPU_UCG_CHANNEL_CLK_SYS, 4 }, /* 290.25 MHz */
	{ 0, CPU_UCG_CHANNEL_CLK_CORE, 1 }, /* 1161 MHz */
	{ 0, CPU_UCG_CHANNEL_CLK_DBUS, 2 }, /* 580.5 MHz */
};

static inline void writel(unsigned long addr, uint32_t value)
{
	(*(volatile uint32_t *)addr) = value;
}

static inline uint32_t readl(unsigned long addr)
{
	return *(volatile uint32_t *)addr;
}

static inline ucg_regs_t *ucg_get_top_registers(uint32_t ucg_id)
{
	return (ucg_regs_t *)(TOP_UCG0_BASE + (0x1000 * ucg_id));
}

static inline ucg_regs_t *ucg_get_service_registers(void)
{
	return (ucg_regs_t *)(SERV_UCG1_BASE);
}

static inline ucg_regs_t *ucg_get_cpu_registers(void)
{
	return (ucg_regs_t *)(CPU_UCG_BASE);
}

static inline ucg_regs_t *ucg_get_lsp1_registers(void)
{
	return (ucg_regs_t *)(LSP1_SUBS_UCG_BASE);
}

static uint64_t usec_to_tick(int usec)
{
	return (uint64_t)usec * TIMER_FREQ / SEC;
}

static uint64_t get_ticks(void)
{
	static uint32_t timebase_h, timebase_l;
	uint32_t now = ~readl(DW_APB_CURRENT_VALUE(0));

	/* Increment tbh if tbl has rolled over */
	if (now < timebase_l)
		timebase_h++;
	timebase_l = now;
	return ((uint64_t)timebase_h << 32) | timebase_l;
}

void delay_usec(int usec)
{
	uint64_t tmp;

	/* Get current timestamp */
	tmp = get_ticks() + usec_to_tick(usec);

	/* Loop till event */
	while (get_ticks() < tmp + 1)
		/*NOP*/;
}

int timer_get_usec(void)
{
	return get_ticks() * SEC / TIMER_FREQ;
}

int putchar(int c)
{
	int ret;
	uint32_t val = 0;

	while (1) {
		ret = readl_poll_timeout(val, val & UART_LSR_THRE, USEC, 100 * USEC, UART_LSR);
		if (ret)
			return ret;

		writel(UART_THR, c);
		if (c == '\n')
			c = '\r';
		else
			break;
	}

	return 0;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* While all data is aligned (common case), copy a word at a time */
	if ((((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* Copy the rest one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}

int top_set_clock(void)
{
	int ret;
	ucg_regs_t *top_ucg[2];

	/* Enable interconnect UCG0 bypass */
	top_ucg[0] = ucg_get_top_registers(0);
	ret = ucg_enable_bp(top_ucg[0], TOP_UCG0_ALL_CH_MASK);
	if (ret)
		return ret;

	/* Enable interconnect UCG1 bypass */
	top_ucg[1] = ucg_get_top_registers(1);
	ret = ucg_enable_bp(top_ucg[1], TOP_UCG1_ALL_CH_MASK);
	if (ret)
		return ret;

	/* Setup PLL to 1188 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 87;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 1;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)TOP_PLL_ADDR, &pll_cfg, 1000);
	if (ret)
		return ret;

	/* Set dividers */
	for (int i = 0; i < ARRAY_SIZE(top_ucg_channels); i++) {
		ret = ucg_set_divider(top_ucg[top_ucg_channels[i].ucg_id],
		                      top_ucg_channels[i].chan_id, top_ucg_channels[i].div, 1000);
		if (ret) {
			return ret;
		}
	}

	/* Sync and disable interconnect UCG0 bypass */
	ret = ucg_sync_and_disable_bp(top_ucg[0], TOP_UCG0_ALL_CH_MASK, TOP_UCG0_SYNC_MASK);
	if (ret)
		return ret;

	/* Sync and disable interconnect UCG1 bypass */
	ret = ucg_sync_and_disable_bp(top_ucg[1], TOP_UCG1_ALL_CH_MASK, TOP_UCG1_SYNC_MASK);
	if (ret)
		return ret;

	return 0;
}

int service_set_clock(void)
{
	int ret;

	ucg_regs_t *serv_ucg = ucg_get_service_registers();

	/* Enable Service Sub UCG1 bypass for all channel */
	ret = ucg_enable_bp(serv_ucg, SERV_UCG1_ALL_CH_MASK);
	if (ret)
		return ret;

	/* Setup PLL to 594 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 131;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 5;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)(SERV_PLL_ADDR), &pll_cfg, 1000);
	if (ret)
		return ret;

	/* Setup UCG1 Divider */
	for (int i = 0; i < ARRAY_SIZE(serv_ucg_channels); i++) {
		ret = ucg_set_divider(serv_ucg, serv_ucg_channels[i].chan_id,
		                      serv_ucg_channels[i].div, 1000);
		if (ret)
			return ret;
	}

	/* Sync and disable Service Sub UCG1 bypass */
	ret = ucg_sync_and_disable_bp(serv_ucg, SERV_UCG1_ALL_CH_MASK, SERV_UCG1_SYNC_MASK);
	if (ret)
		return ret;

	return 0;
}

static void set_ppolicy(unsigned long addr, uint32_t value)
{
	value &= 0x1f;
	writel(addr, value);
	while ((readl(addr + 0x4) & 0x1f) != value) {
	}
}

static int start_arm_cpu(void)
{
	int ret;

	ucg_regs_t *cpu_ucg = ucg_get_cpu_registers();

	/* Enable CPU_SUBS */
	set_ppolicy(SERV_CPU_PPOLICY, PP_ON);

	/* Enable CPU Sub UCG bypass for all channel */
	ret = ucg_enable_bp(cpu_ucg, CPU_UCG_ALL_CH_MASK);
	if (ret)
		return ret;

	/* Setup PLL to 1161 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg_t pll_cfg;
	pll_cfg.nf_value = 85;
	pll_cfg.nr_value = 0;
	pll_cfg.od_value = 1;

	ret = pll_set_manual_freq((pll_cfg_reg_t *)CPU_PLL_ADDR, &pll_cfg, 1000);
	if (ret)
		return ret;

	/* Set dividers */
	for (int i = 0; i < ARRAY_SIZE(cpu_ucg_channels); i++) {
		ret = ucg_set_divider(cpu_ucg, cpu_ucg_channels[i].chan_id, cpu_ucg_channels[i].div,
		                      1000);
		if (ret) {
			return ret;
		}
	}

	/* Sync and disable CPU UCG bypass */
	ret = ucg_sync_and_disable_bp(cpu_ucg, CPU_UCG_ALL_CH_MASK, CPU_UCG_SYNC_MASK);
	if (ret)
		return ret;

	set_ppolicy(CPU_SYS_PPOLICY, PP_ON);
	/* Setup CPU cores start addresses */
	for (int i = 0; i < 4; i++)
		WRITE_CPU_START_ADDR_REG(CPU_RVBADDR(i), TFA_START_ADDR_PHYS);
	set_ppolicy(CPU_CPU0_PPOLICY, PP_ON);

	return 0;
}

int lsp1_set_clock(void)
{
	int ret;

	/* Release reset signal of LS Peripheral 1 */
	set_ppolicy(SERV_LSP1_PPOLICY, PP_ON);

	/* Initialize the UCG register for clocking GPIO1 and UART0 */
	ucg_regs_t *ucg = ucg_get_lsp1_registers();

	ret = ucg_enable_bp(ucg, LSP1_SUBS_UCG_ALL_CH_MASK);
	if (ret)
		return ret;

	ret = ucg_set_divider(ucg, LSP1_SUBS_UCG_CLK_GPIO1, 1, 1000);
	if (ret)
		return ret;

	ret = ucg_set_divider(ucg, LSP1_SUBS_UCG_CLK_UART0, 1, 1000);
	if (ret)
		return ret;

	ret = ucg_set_divider(ucg, LSP1_SUBS_UCG_CLK_TIMERS, 1, 1000);
	if (ret)
		return ret;

	ret = ucg_sync_and_disable_bp(ucg, LSP1_SUBS_UCG_ALL_CH_MASK, LSP1_SUBS_UCG_SYNC_MASK);
	if (ret)
		return ret;

	return 0;
}

void uart0_gpio_init(unsigned int pin, unsigned int direction)
{
	unsigned int swportb_ddr = readl(LSP1_SUBS_GPIO1_SWPORTB_DDR);
	unsigned int swportb_ctl = readl(LSP1_SUBS_GPIO1_SWPORTB_CTL);

	/* Set pin direction */
	if (direction == GPIO_DIR_OUTPUT)
		swportb_ddr |= BIT(pin);
	else
		swportb_ddr &= ~(BIT(pin));

	/* Set hw mode */
	swportb_ctl |= BIT(pin);

	writel(LSP1_SUBS_GPIO1_SWPORTB_DDR, swportb_ddr);
	writel(LSP1_SUBS_GPIO1_SWPORTB_CTL, swportb_ctl);
}

int uart0_enable(void)
{
	uint16_t divisor;
	uint32_t val;

	/* Initialize GPIO1 PORTB PIN6 and PIN7 for UART 0 */
	uart0_gpio_init(UART0_SOUT_PIN, GPIO_DIR_OUTPUT);
	uart0_gpio_init(UART0_SIN_PIN, GPIO_DIR_INPUT);

	val = FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_SL, 3) |
	      FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_CTL, PAD_DRIVER_STREGTH_6mA);
	writel(LSP1_SUBS_GPIO1_PORTBN_PADCTR(UART0_SOUT_PIN), val);

	val |= FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_SUS, 1) |
	       FIELD_PREP(LSP1_SUBS_GPIO1_PORTBN_PADCTR_E, 1);
	writel(LSP1_SUBS_GPIO1_PORTBN_PADCTR(UART0_SIN_PIN), val);

	/* Set baudrate */
	divisor = DIV_ROUND_CLOSEST(UART0_BASE_FREQ, UART0_BAUDRATE * 16);
	writel(UART_LCR, UART0_LCR_DEFAULT | UART_LCR_DLAB);
	writel(UART_DLH, divisor >> 8);
	writel(UART_DLL, divisor);
	writel(UART_LCR, UART0_LCR_DEFAULT);

	/* Make sure last LCR write wasn't ignored */
	while (readl(UART_LCR) != UART0_LCR_DEFAULT) {
		writel(UART_FCR, 0);
		writel(UART_FCR, UART_FCR_FIFOE);
		writel(UART_FCR, UART_FCR_FIFOE | UART_FCR_XFIFOR | UART_FCR_RFIFOR);
		readl(UART_THR);
		writel(UART_LCR, UART0_LCR_DEFAULT);
	}

	writel(UART_IER, 0);
	writel(UART_MCR, UART_MCR_DTR | UART_MCR_RTS);

	return 0;
}

int main(void)
{
	int ret;

	/* Initialize and configure the TOP clock gate */
	writel(SERV_TOP_CLK_GATE_URB_BASE, SERV_TOP_CLK_GATE_ALL_CH_MASK);

	/* Once enabled, WDT cannot be disabled again even after
	 * a system reset. Set WDT timeout to the maximum value (if it is
	 * already enabled) as a workaround (2^31 clock cycles, assuming that
	 * reference clock for WDT0 is 99 MHz the timeout value is ~21s).
	 */
	if (readl(SERV_WDT0_BASE + SERV_WDT_CR) & SERV_WDT_EN) {
		writel(SERV_WDT0_BASE + SERV_WDT_TORR, 0xff);
		writel(SERV_WDT0_BASE + SERV_WDT_CRR, SERV_WDT_CRR_KICK_VALUE);
	}

	/* Initialize and configure the RISC0 clocking system */
	ret = service_set_clock();
	if (ret)
		goto exit;

	/* Initialize and configure the InterConnect clocking system */
	ret = top_set_clock();
	if (ret)
		goto exit;

	/* Initialize and configure the LS Peripheral 1 clocking system */
	ret = lsp1_set_clock();
	if (ret)
		goto exit;

	/* Init timer */
	writel(DW_APB_LOAD_COUNT(0), 0);
	writel(DW_APB_CTRL(0), 0x5);

	/* Initialize and configure the UART0 */
	ret = uart0_enable();
	if (ret)
		return ret;

	printf(PREFIX " (" __DATE__ " - " __TIME__ "): " COMMIT "\r\n");
#ifdef BUILD_ID
	printf(PREFIX ": Build: %s\r\n", BUILD_ID);
#endif

	/* Relocate ddrinit */
	INFO("Load images ... \n");
	unsigned long *start = (unsigned long *)&__ddrinit_start;
	unsigned long *end = (unsigned long *)&__ddrinit_end;
	uint32_t size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)DDRINIT_START_ADDR_VIRT, (void *)start, size);

	/* Note that ddrinit does memory mapping:
	 * +----------------------------+-------------------------+
	 * | 64Bit phys addrs           |   32bit virt addrs      |
	 * +----------------------------+-------------------------+
	 * | 0x890400000 - 0x8905FFFFF  | 0xC0000000 - 0xC01FFFFF |
	 * +----------------------------+-------------------------+
	 * | 0x880200000 - 0x88FFFFFFF  | 0xC0200000 - 0xCFFFFFFF |
	 * +----------------------------+-------------------------+
	 */
	void (*start_ddrinit)(void) = (void *)DDRINIT_START_ADDR_VIRT;
	start_ddrinit();

	/* Mark the first 256 MB of DDR High as Secure.
	 * This code is provided as example and doesn't affect on security levels at VS_EN = 1
	 * but can be useful for test purpose in case of boot with VS_EN = 0.
	 */
	set_secure_region();

	/* Relocate DTB */
	start = (unsigned long *)&__dtb_start;
	end = (unsigned long *)&__dtb_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_DTB_ADDR_VIRT, start, size);

	/* Relocate TF-A */
	start = (unsigned long *)&__tfa_start;
	end = (unsigned long *)&__tfa_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)TFA_START_ADDR_VIRT, (void *)start, size);

#if defined(HAS_BL32)
	/* Relocate BL32 */
	start = (unsigned long *)&__bl32_start;
	end = (unsigned long *)&__bl32_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)BL32_START_ADDR_VIRT, (void *)start, size);
#endif

	/* Relocate U-Boot */
	start = (unsigned long *)&__uboot_start;
	end = (unsigned long *)&__uboot_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_START_ADDR_VIRT, (void *)start, size);

	/*
	 * Set PLL as clock source for all HSPERIPH UCGs
	 * Some HSperiph UCGs use CLK125 external pad as default source
	 * (this is considered as chip bug).
	 * Some boards might not have CLK125 routed so it's not possible
	 * to read/write UCG registers (UCG configuration hangs).
	 * This is a temporary solution.
	 * All clocks should be configured by clock driver.
	 */
	writel(HSP_SUBS_REFCLK_REG, 0);

	/* I2S RSTN must be deasserted before LSP1 UCGs setup */
	set_ppolicy(LSP1_SUBS_I2S_UCG1_RSTN_PPOLICY_REG, PP_ON);

	/* Initialize and configure the CPU clocking system and run it */
	INFO("Done.\nRun TF-A ... \n");
	ret = start_arm_cpu();
	if (ret)
		goto exit;

exit:
	while (1)
		asm volatile("wait");

	return 0;
}
