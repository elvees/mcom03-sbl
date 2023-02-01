// Copyright 2020-2021 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <string.h>
#include "mips/m32c0.h"
#include "bitops.h"
#include "ucg.h"
#include "pll.h"
#include "regs.h"

#define PP_OFF	    0x01
#define PP_WARM_RST 0x08
#define PP_ON	    0x10

#define DDRINIT_START_ADDR_VIRT 0xA0000000
#define TFA_START_ADDR_VIRT	0xC0300000
#define TFA_START_ADDR_PHYS	0x880300000
#define BL32_START_ADDR_VIRT	0xC1380000
#define UBOOT_START_ADDR_VIRT	0xC0080000

#define DDRMC_MAX_NUMBER      2U
#define MAX_MEM_REGIONS	      4U
#define MEM_REGIONS_VIRT_ADDR 0xC0000000

#define SECURE_REGIONS_PHYS_ADDR_START 0x880000000ULL
#define SECURE_REGIONS_PHYS_ADDR_SIZE  0x10000000ULL
#define SECURE_REGIONS_PHYS_ADDR_MASK  (~(SECURE_REGIONS_PHYS_ADDR_SIZE - 1))
#define SECURE_REGIONS_PHYS_ADDR_END \
	(SECURE_REGIONS_PHYS_ADDR_START + SECURE_REGIONS_PHYS_ADDR_SIZE)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

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

struct ddrinfo {
	uint64_t dram_size[DDRMC_MAX_NUMBER];
	uint64_t total_dram_size;
	struct {
		bool enable;
		int channels;
		int size;
	} interleaving;
	int speed[DDRMC_MAX_NUMBER];
	/* RAM configuration */
	struct {
		uint64_t start;
		uint64_t size;
	} mem_regions[MAX_MEM_REGIONS];
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

	/* Relocate ddrinit */
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
	writel(BASE_SECURE_REGION_LOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_START);
	writel(BASE_SECURE_REGION_HIGH(0), (uint32_t)(SECURE_REGIONS_PHYS_ADDR_START >> 32ULL));
	writel(MASK_SECURE_REGION_LOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_MASK);
	writel(MASK_SECURE_REGION_HIGH(0), (uint32_t)(SECURE_REGIONS_PHYS_ADDR_MASK >> 32ULL));
	writel(CTR_SECURE_REGION, 0x1);

	struct ddrinfo *info = (struct ddrinfo *)MEM_REGIONS_VIRT_ADDR;

	/* TODO: Currently we are support one secure region started from the beginning of
	 * DDR High. The region size must be a power of 2. It is required to modify of
	 * ddrinfo struct to add several regions dynamically.
	 */
	for (int i = 0; i < MAX_MEM_REGIONS; ++i) {
		uint64_t start = info->mem_regions[i].start;
		uint64_t end = info->mem_regions[i].start + info->mem_regions[i].size;
		if ((SECURE_REGIONS_PHYS_ADDR_START >= start) &&
		    (SECURE_REGIONS_PHYS_ADDR_END <= end)) {
			info->mem_regions[i].start = SECURE_REGIONS_PHYS_ADDR_END;
			info->mem_regions[i].size = end - SECURE_REGIONS_PHYS_ADDR_END;
		}
	}

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
