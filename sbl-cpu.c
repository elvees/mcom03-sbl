/*
 * Copyright 2020-2021 RnD Center "ELVEES", JSC
 */

#include <stdint.h>
#include <string.h>

#define BIT(n) (1 << (n))
#define GENMASK(hi, lo) (((1 << ((hi) - (lo) + 1)) - 1) << (lo))
#define __bf_shf(x) (__builtin_ffs(x) - 1)
#define FIELD_PREP(_mask, _val) ({ ((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask); })
#define FIELD_GET(_mask, _reg) ({ (typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask)); })
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define COMM_UCG_CTR(i, j) (0x1801000 + (i) * 0x1000 + (j) * 4)
#define COMM_UCG_BP(i) (0x1801040 + (i) * 0x1000)
#define COMM_PLL 0x1800000

#define PLL_CFG_SEL GENMASK(7, 0)
#define PLL_CFG_MAN BIT(9)
#define PLL_CFG_OD GENMASK(13, 10)
#define PLL_CFG_NF GENMASK(26, 14)
#define PLL_CFG_NR GENMASK(30, 27)
#define PLL_CFG_LOCK BIT(31)

#define UCG_CTR_LPI_EN BIT(0)
#define UCG_CTR_CLK_EN BIT(1)
#define UCG_CTR_CLK_EN_STS GENMASK(4, 2)
#define UCG_CTR_QACTIVE_CTL_EN BIT(6)
#define UCG_CTR_QFSM_STATE GENMASK(9, 7)
#define UCG_CTR_DIV_COEFF GENMASK(29, 10)
#define UCG_CTR_DIV_LOCK BIT(30)

#define SERV_URB_CPU_PPOLICY 0x1f000000
#define SERV_URB_HSPERIPH_SUBS_PPOLICY 0x1f000020
#define SERV_URB_TOP_CLKGATE 0x1f001008

#define CPU_CPU0_PPOLICY 0x1000000
#define CPU_SYS_PPOLICY 0x1000040
#define CPU_RVBADDR(x) (0x1000118 + (x * 8))

#define PP_ON 0x10

#define TFA_MAGIC_ADDR 0xC0800000
#define TFA_MAGIC_VALUE 0xdeadc0de

#define DDRINIT_START_ADDR	0
#define TFA_START_ADDR_VIRT	0xC0000000
#define TFA_START_ADDR_PHYS	0x80000000
#define UBOOT_START_ADDR	0xC0080000

#define WRITE_CPU_START_ADDR_REG(reg, val) \
	do { \
		writel((reg), ((uint64_t)(val)) >> 32); \
		writel((reg) + 4, ((uint64_t)(val)) & 0xffffffff); \
	} while (0)

extern unsigned long __ddrinit_start, __ddrinit_end;
extern unsigned long __tfa_start, __tfa_end;
extern unsigned long __uboot_start, __uboot_end;

struct ucg_channel {
	int ucg_id;
	int chan_id;
	int div;
};

static struct ucg_channel comm_ucg_channels[] = {
	{0, 0, 6},	/* DP		198 MHz */
	{0, 1, 8},	/* VPU		148.5 MHz */
	{0, 2, 4},	/* GPU		297 MHz */
	{0, 3, 12},	/* ISP		99 MHz */
	{0, 4, 4},	/* CPU		297 MHz */
	{0, 5, 4},	/* ACP		297 MHz */
	{0, 6, 12},	/* LSP0		99 MHz */
	{0, 7, 4},	/* COH_COMM	297 MHz */

	/* To work around MCOM03SW-1192 the following frequency ratio must be met:
	 * SLOW_COMM_FREQ < 1/2 * min(LSP0_SYS_FREQ, LSP1_SYS_FREQ, DDR_SYS_FREQ).
	 */
	{1, 0, 30},	/* SLOW_COMM	39.6 MHz */
	{1, 2, 8},	/* FAST_COMM	148.5 MHz */
	{1, 4, 4},	/* DSP		297 MHz */
	{1, 5, 4},	/* PCIe		297 MHz */
	{1, 6, 12},	/* LSP1		99 MHz */
	{1, 7, 8},	/* SERVICE	148.5 MHz */
	{1, 8, 6},	/* HSP		198 MHz */
};

static inline void writel(unsigned long addr, uint32_t value)
{
	(*(volatile uint32_t *)addr) = value;
}

static inline uint32_t readl(unsigned long addr)
{
	return *(volatile uint32_t *)addr;
}

static uint64_t usec_to_tick(int usec)
{
	return (uint64_t)usec * 27ULL;
}

static uint32_t platform_get_timer_count()
{
	uint32_t res;
	__asm__ __volatile__("mfc0 %0, $9"
			     : "=r" (res));
	return res;
}

static uint64_t get_ticks(void)
{
	static uint32_t timebase_h, timebase_l;
	uint32_t now = platform_get_timer_count();

	/* Increment tbh if tbl has rolled over */
	if (now < timebase_l)
		timebase_h++;
	timebase_l = now;
	return ((uint64_t)timebase_h << 32) | timebase_l;
}

static void delay_usec(int usec)
{
	uint64_t tmp;

	/* Get current timestamp */
	tmp = get_ticks() + usec_to_tick(usec);

	/* Loop till event */
	while (get_ticks() < tmp + 1)
		 /*NOP*/;
}

void* memcpy(void *dest, const void *src, size_t count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* While all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
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

static void comm_ucg_cfg(void)
{
	uint32_t val;

	/* Enable bypass for all channels */
	writel(COMM_UCG_BP(0), 0xff);
	writel(COMM_UCG_BP(1), 0x1f5);

	/* Setup PLL to 1188 MHz, assuming that XTI = 27 MHz. Use NR = 0 to
	 * minimize PLL output jitter.
	 */
	val = FIELD_PREP(PLL_CFG_SEL, 1) |
	      FIELD_PREP(PLL_CFG_MAN, 1) |
	      FIELD_PREP(PLL_CFG_OD, 1) |
	      FIELD_PREP(PLL_CFG_NF, 87) |
	      FIELD_PREP(PLL_CFG_NR, 0);
	writel(COMM_PLL, val);

	while (!(readl(COMM_PLL) & 0x80000000))
		continue;

	/* Set dividers */
	for (int i = 0; i < ARRAY_SIZE(comm_ucg_channels); i++) {
		unsigned long chan_addr = COMM_UCG_CTR(comm_ucg_channels[i].ucg_id,
						       comm_ucg_channels[i].chan_id);

		val = FIELD_PREP(UCG_CTR_DIV_COEFF, comm_ucg_channels[i].div);
		writel(chan_addr, val);
		while (!(readl(chan_addr) & UCG_CTR_DIV_LOCK))
			continue;

		val |= UCG_CTR_CLK_EN;
		writel(chan_addr, val);
		while (!(readl(chan_addr) & UCG_CTR_DIV_LOCK))
			continue;
	}

	/* Disable bypass */
	writel(COMM_UCG_BP(0), 0x0);
	writel(COMM_UCG_BP(1), 0x0);
}

#ifdef MIPS32
static void set_ppolicy(unsigned long addr, uint32_t value)
{
	value &= 0x1f;
	writel(addr, value);
	while ((readl(addr + 0x4) & 0x1f) != value) {
	}
}

static void start_arm_cpu(void)
{
	/* Enable HSPERIPH */
	set_ppolicy(SERV_URB_HSPERIPH_SUBS_PPOLICY, PP_ON);
	set_ppolicy(SERV_URB_CPU_PPOLICY, PP_ON);
	writel(SERV_URB_TOP_CLKGATE, 0x115);

	/* Enable clk_core, clk_dbus and clk_sys without dividers */
	writel(0x1080000, 0x2);
	writel(0x1080004, 0x2);
	writel(0x1080008, 0x2);

	set_ppolicy(CPU_SYS_PPOLICY, PP_ON);
	WRITE_CPU_START_ADDR_REG(CPU_RVBADDR(0), DDRINIT_START_ADDR);
	set_ppolicy(CPU_CPU0_PPOLICY, PP_ON);

	delay_usec(1500000);
}

static void kick_arm_cpu(void)
{
	writel(TFA_MAGIC_ADDR, TFA_MAGIC_VALUE);

	/* Drop magic to prevent autostart TFA after reset */
	delay_usec(1000);
	writel(TFA_MAGIC_ADDR, 0);

	while (1)
		asm volatile ("wait");
}
#endif

int main(void)
{
	unsigned long *start = (unsigned long *)&__ddrinit_start;
	unsigned long *end = (unsigned long *)&__ddrinit_end;
	uint32_t size = (unsigned long)end - (unsigned long)start;

	comm_ucg_cfg();

	/* Relocate ddrinit */
	memcpy((void *)DDRINIT_START_ADDR, (void *)start, size);

#ifdef MIPS32
	start_arm_cpu();
#else
	void (*start_ddrinit)(void) = (void *)DDRINIT_START_ADDR;
	start_ddrinit();
#endif

	/* Relocate TF-A */
	start = (unsigned long *)&__tfa_start;
	end = (unsigned long *)&__tfa_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)TFA_START_ADDR_VIRT, (void *)start, size);

	/* Setup CPU cores start addresses */
	for (int i = 0; i < 4; i++)
		WRITE_CPU_START_ADDR_REG(CPU_RVBADDR(i), TFA_START_ADDR_PHYS);

	/* Relocate U-Boot */
	start = (unsigned long *)&__uboot_start;
	end = (unsigned long *)&__uboot_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_START_ADDR, (void *)start, size);

#ifdef MIPS32
	kick_arm_cpu();
#else
	void (*start_tfa)(void) = (void *)TFA_START_ADDR_VIRT;
	start_tfa();
#endif
	return 0;
}
