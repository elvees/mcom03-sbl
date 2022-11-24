// Copyright 2020-2021 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <string.h>
#include "mips/m32c0.h"

#define BIT(n)			(1 << (n))
#define GENMASK(hi, lo)		(((1 << ((hi) - (lo) + 1)) - 1) << (lo))
#define __bf_shf(x)		(__builtin_ffs(x) - 1)
#define FIELD_PREP(_mask, _val) ({ ((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask); })
#define FIELD_GET(_mask, _reg)	({ (typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask)); })
#define ARRAY_SIZE(x)		(sizeof(x) / sizeof((x)[0]))

#define COMM_UCG_CTR(i, j) (0xA1801000 + (i)*0x1000 + (j)*4)
#define COMM_UCG_BP(i)	   (0xA1801040 + (i)*0x1000)
#define COMM_PLL	   0xA1800000

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

#define SERV_URB_CPU_PPOLICY	       0xBF000000
#define SERV_URB_HSPERIPH_SUBS_PPOLICY 0xBF000020
#define SERV_URB_TOP_CLKGATE	       0xBF001008
#define SERV_WDT0_BASE		       0xBF080000
#define SERV_WDT_CR		       0x0
#define SERV_WDT_TORR		       0x4
#define SERV_WDT_CRR		       0xC
#define SERV_WDT_CRR_KICK_VALUE	       0x76
#define SERV_WDT_EN		       BIT(0)

#define SERV_RISC0_CSR 0xBFD08000

#define CPU_CPU0_PPOLICY 0xA1000000
#define CPU_SYS_PPOLICY	 0xA1000040
#define CPU_RVBADDR(x)	 (0xA1000118 + (x * 8))
#define CPU_UCG_BYPASS	 0xA1080040
#define CPU_UCG_SYNC	 0xA1080044
#define CPU_UCG_CTR(i)	 (0xA1080000 + (i)*4)
#define CPU_PLL		 0xA1000050

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

static struct ucg_channel comm_ucg_channels[] = {
	{ 0, 0, 6 }, /* DP		198 MHz */
	{ 0, 1, 8 }, /* VPU		148.5 MHz */
	{ 0, 2, 4 }, /* GPU		297 MHz */
	{ 0, 3, 12 }, /* ISP		99 MHz */
	{ 0, 4, 4 }, /* CPU		297 MHz */
	{ 0, 5, 4 }, /* ACP		297 MHz */
	{ 0, 6, 12 }, /* LSP0		99 MHz */
	{ 0, 7, 4 }, /* COH_COMM	297 MHz */

	/* To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{ 1, 0, 30 }, /* SLOW_COMM	39.6 MHz */
	{ 1, 2, 8 }, /* FAST_COMM	148.5 MHz */
	{ 1, 4, 4 }, /* DSP		297 MHz */
	{ 1, 5, 4 }, /* PCIe		297 MHz */
	{ 1, 6, 12 }, /* LSP1		99 MHz */
	{ 1, 7, 8 }, /* SERVICE	148.5 MHz */
	{ 1, 8, 6 }, /* HSP		198 MHz */
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

static void pll_cfg(unsigned long addr, int od, int nf)
{
	uint32_t val;

	val = FIELD_PREP(PLL_CFG_SEL, 1) | FIELD_PREP(PLL_CFG_MAN, 1) | FIELD_PREP(PLL_CFG_OD, od) |
	      FIELD_PREP(PLL_CFG_NF, nf) | FIELD_PREP(PLL_CFG_NR, 0);
	writel(addr, val);

	while (!(readl(addr) & PLL_CFG_LOCK))
		continue;
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

static void comm_ucg_cfg(void)
{
	/* Enable bypass for all channels */
	writel(COMM_UCG_BP(0), 0xff);
	writel(COMM_UCG_BP(1), 0x1f5);

	/* Setup PLL to 1188 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	pll_cfg(COMM_PLL, 1, 87);

	/* Set dividers */
	for (int i = 0; i < ARRAY_SIZE(comm_ucg_channels); i++)
		ucg_cfg(COMM_UCG_CTR(comm_ucg_channels[i].ucg_id, comm_ucg_channels[i].chan_id),
			comm_ucg_channels[i].div);

	/* Disable bypass */
	writel(COMM_UCG_BP(0), 0x0);
	writel(COMM_UCG_BP(1), 0x0);
}

static void set_ppolicy(unsigned long addr, uint32_t value)
{
	value &= 0x1f;
	writel(addr, value);
	while ((readl(addr + 0x4) & 0x1f) != value) {
	}
}

static void start_arm_cpu(void)
{
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

	/* Setup PLL to 1161 MHz, assuming that XTI = 27 MHz */
	pll_cfg(CPU_PLL, 1, 85);

	for (i = 0; i < ARRAY_SIZE(divs); i++)
		ucg_cfg(CPU_UCG_CTR(i), divs[i]);

	writel(CPU_UCG_SYNC, 7);
	writel(CPU_UCG_BYPASS, 0);

	set_ppolicy(CPU_SYS_PPOLICY, PP_ON);
	/* Setup CPU cores start addresses */
	for (int i = 0; i < 4; i++)
		WRITE_CPU_START_ADDR_REG(CPU_RVBADDR(i), TFA_START_ADDR_PHYS);
	set_ppolicy(CPU_CPU0_PPOLICY, PP_ON);
}

int main(void)
{
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

	comm_ucg_cfg();

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

	start_arm_cpu();

	while (1)
		asm volatile("wait");

	return 0;
}
