// Copyright 2020-2021 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <string.h>
#include "mips/m32c0.h"
#include "bitops.h"
#include "pll.h"
#include "regs.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define TOP_UCG_CTR(i, j) (TOP_UCG0_BASE + (i)*0x1000 + (j)*4)
#define TOP_UCG_BP(i)	  (TOP_UCG0_BASE + (i)*0x1000 + 0x40)
#define TOP_PLL_ADDR	  (TOP_URB_BASE)

#define PLL_CFG_SEL  GENMASK(7, 0)
#define PLL_CFG_MAN  BIT(9)
#define PLL_CFG_OD   GENMASK(13, 10)
#define PLL_CFG_NF   GENMASK(26, 14)
#define PLL_CFG_NR   GENMASK(30, 27)
#define PLL_CFG_LOCK BIT(31)

#define UCG_CTR_LPI_EN	       BIT(0)
#define UCG_CTR_CLK_EN	       BIT(1)
#define UCG_CTR_CLK_EN_STS     GENMASK(4, 2)
#define UCG_CTR_QACTIVE_CTL_EN BIT(6)
#define UCG_CTR_QFSM_STATE     GENMASK(9, 7)
#define UCG_CTR_DIV_COEFF      GENMASK(29, 10)
#define UCG_CTR_DIV_LOCK       BIT(30)

#define SERV_URB_CPU_PPOLICY	       (SERV_URB_BASE)
#define SERV_URB_HSPERIPH_SUBS_PPOLICY (SERV_URB_BASE + 0x20)
#define SERV_URB_TOP_CLKGATE	       (SERV_URB_BASE + 0x1008)
#define SERV_WDT0_BASE		       (SERV_URB_BASE + 0x80000)
#define SERV_WDT_CR		       0x0
#define SERV_WDT_TORR		       0x4
#define SERV_WDT_CRR		       0xC
#define SERV_WDT_CRR_KICK_VALUE	       0x76
#define SERV_WDT_EN		       BIT(0)

#define SERV_RISC0_CSR 0xBFD08000

#define CPU_CPU0_PPOLICY (CPU_URB_BASE)
#define CPU_SYS_PPOLICY	 (CPU_URB_BASE + 0x40)
#define CPU_PLL_ADDR	 (CPU_URB_BASE + 0x50)
#define CPU_RVBADDR(i)	 (CPU_URB_BASE + (i)*8 + 0x118)
#define CPU_UCG_BYPASS	 (CPU_UCG_BASE + 0x40)
#define CPU_UCG_SYNC	 (CPU_UCG_BASE + 0x44)
#define CPU_UCG_CTR(i)	 (CPU_UCG_BASE + (i)*4)

#define PP_ON 0x10

#define TFA_MAGIC_ADDR	0xC0800000
#define TFA_MAGIC_VALUE 0xdeadc0de

#define DDRINIT_START_ADDR_VIRT 0xA0000000
#define TFA_START_ADDR_VIRT	0xC0300000
#define TFA_START_ADDR_PHYS	0x880300000
#define BL32_START_ADDR_VIRT	0xC1380000
#define UBOOT_START_ADDR_VIRT	0xC0080000

#define WRITE_CPU_START_ADDR_REG(reg, val)                         \
	do {                                                       \
		writel((reg), ((uint64_t)(val)) >> 32);            \
		writel((reg) + 4, ((uint64_t)(val)) & 0xffffffff); \
	} while (0)

extern unsigned long __ddrinit_start, __ddrinit_end;
extern unsigned long __tfa_start, __tfa_end;
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
	{ 0, TOP_UCG0_CHANNEL_DDR_VPU, 8 }, /* 148.5 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_GPU, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_ISP, 12 }, /* 99 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_CPU, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_CPU_ACP, 4 }, /* 297 MHz */
	{ 0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0, 12 }, /* 99 MHz */
	{ 0, TOP_UCG0_CHANNEL_AXI_COH_COMM, 4 }, /* 297 MHz */

	/* To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{ 1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM, 30 }, /* 39.6 MHz */
	{ 1, TOP_UCG1_CHANNEL_AXI_FAST_COMM, 8 }, /* 148.5 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_DSP, 4 }, /* 297 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SDR_PICE, 4 }, /* 297 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_LSPERIPH1, 12 }, /* 99 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_SERVICE, 8 }, /* 148.5 MHz */
	{ 1, TOP_UCG1_CHANNEL_DDR_HSPERIPH, 6 }, /* 198 MHz */
};

static inline void writel(unsigned long addr, uint32_t value)
{
	(*(volatile uint32_t *)addr) = value;
}

static inline uint32_t readl(unsigned long addr)
{
	return *(volatile uint32_t *)addr;
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

static void ucg_cfg(unsigned long ucg_addr, int div)
{
	uint32_t val;

	val = readl(ucg_addr);
	val |= FIELD_PREP(UCG_CTR_DIV_COEFF, div);
	writel(ucg_addr, val);
	while (!(readl(ucg_addr) & UCG_CTR_DIV_LOCK))
		continue;

	if (!(val & UCG_CTR_CLK_EN)) {
		val |= UCG_CTR_CLK_EN;
		writel(ucg_addr, val);
		while (!(readl(ucg_addr) & UCG_CTR_DIV_LOCK))
			continue;
	}
}

static int top_set_clock(void)
{
	int ret;

	/* Enable bypass for all channels */
	writel(TOP_UCG_BP(0), 0xff);
	writel(TOP_UCG_BP(1), 0x1f5);

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
	for (int i = 0; i < ARRAY_SIZE(top_ucg_channels); i++)
		ucg_cfg(TOP_UCG_CTR(top_ucg_channels[i].ucg_id, top_ucg_channels[i].chan_id),
			top_ucg_channels[i].div);

	/* Disable bypass */
	writel(TOP_UCG_BP(0), 0x0);
	writel(TOP_UCG_BP(1), 0x0);

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

	uint32_t val;
	int i;
	uint8_t divs[] = {
		4, /* sys clk 290.25 MHz */
		1, /* core clk 1161 MHz */
		2 /* dbus clk 580.5 MHz */
	};

	/* Enable CPU_SUBS */
	set_ppolicy(SERV_URB_CPU_PPOLICY, PP_ON);

	/* Enable core clk and dbus clk (sys clk is enabled by default)*/
	for (i = 1; i < ARRAY_SIZE(divs); i++) {
		val = readl(CPU_UCG_CTR(i));
		val |= UCG_CTR_CLK_EN;
		writel(CPU_UCG_CTR(i), val);
		while (!(readl(CPU_UCG_CTR(i)) & UCG_CTR_DIV_LOCK))
			continue;
	}

	writel(CPU_UCG_BYPASS, 7);

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

	for (i = 0; i < ARRAY_SIZE(divs); i++)
		ucg_cfg(CPU_UCG_CTR(i), divs[i]);

	writel(CPU_UCG_SYNC, 7);
	writel(CPU_UCG_BYPASS, 0);

	set_ppolicy(CPU_SYS_PPOLICY, PP_ON);
	/* Setup CPU cores start addresses */
	for (int i = 0; i < 4; i++)
		WRITE_CPU_START_ADDR_REG(CPU_RVBADDR(i), TFA_START_ADDR_PHYS);
	set_ppolicy(CPU_CPU0_PPOLICY, PP_ON);

	return 0;
}

int main(void)
{
	int ret;

	unsigned long *start = (unsigned long *)&__ddrinit_start;
	unsigned long *end = (unsigned long *)&__ddrinit_end;
	uint32_t size = (unsigned long)end - (unsigned long)start;

	/* Once enabled, WDT cannot be disabled again even after
	 * a system reset. Set WDT timeout to the maximum value (if it is
	 * already enabled) as a workaround (2^31 clock cycles, since SBL
	 * doesn't set up the reference clock for WDT0, assuming that
	 * XTI = 27 MHz the timeout value is ~79s) .
	 */
	if (readl(SERV_WDT0_BASE + SERV_WDT_CR) & SERV_WDT_EN) {
		writel(SERV_WDT0_BASE + SERV_WDT_TORR, 0xff);
		writel(SERV_WDT0_BASE + SERV_WDT_CRR, SERV_WDT_CRR_KICK_VALUE);
	}

	/* Initialize and configure the InterConnect clocking system */
	ret = top_set_clock();
	if (ret)
		goto exit;

	/* Relocate ddrinit */
	memcpy((void *)DDRINIT_START_ADDR_VIRT, (void *)start, size);

	void (*start_ddrinit)(void) = (void *)DDRINIT_START_ADDR_VIRT;

	/* Enable ref clks */
	writel(SERV_URB_TOP_CLKGATE, 0x115);

	/* Note that ddrinit does memory mapping:
	 * +----------------------------+-------------------------+
	 * | 64Bit phys addrs           |   32bit virt addrs      |
	 * +----------------------------+-------------------------+
	 * | 0x890400000 - 0x8905FFFFF  | 0xC0000000 - 0xC01FFFFF |
	 * +----------------------------+-------------------------+
	 * | 0x880200000 - 0x88FFFFFFF  | 0xC0200000 - 0xCFFFFFFF |
	 * +----------------------------+-------------------------+
	 */
	start_ddrinit();

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

	/* Initialize and configure the CPU clocking system and run it */
	ret = start_arm_cpu();
	if (ret)
		goto exit;

exit:
	while (1)
		asm volatile("wait");

	return 0;
}
