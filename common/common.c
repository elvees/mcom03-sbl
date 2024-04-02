// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdbool.h>
#include <mips/m32c0.h>
#include "common.h"

#define DDRMC_MAX_NUMBER      2U
#define MAX_MEM_REGIONS       4U
#define MEM_REGIONS_VIRT_ADDR 0xC0000000

#define DDR_SYS_URB_BASE             0xac000000
#define CTR_SECURE_REGION            (DDR_SYS_URB_BASE + 0x80)
#define BASE_SECURE_REGION_LOW(num)  (DDR_SYS_URB_BASE + (0x90 + (num * 0x10)))
#define BASE_SECURE_REGION_HIGH(num) (DDR_SYS_URB_BASE + (0x94 + (num * 0x10)))
#define MASK_SECURE_REGION_LOW(num)  (DDR_SYS_URB_BASE + (0x98 + (num * 0x10)))
#define MASK_SECURE_REGION_HIGH(num) (DDR_SYS_URB_BASE + (0x9C + (num * 0x10)))

#define SECURE_REGIONS_PHYS_ADDR_START 0x880000000ULL
#define SECURE_REGIONS_PHYS_ADDR_SIZE  0x10000000ULL
#define SECURE_REGIONS_PHYS_ADDR_MASK  (~(SECURE_REGIONS_PHYS_ADDR_SIZE - 1))
#define SECURE_REGIONS_PHYS_ADDR_END \
	(SECURE_REGIONS_PHYS_ADDR_START + SECURE_REGIONS_PHYS_ADDR_SIZE)

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

static inline void writel(unsigned long addr, uint32_t value)
{
	(*(volatile uint32_t *)addr) = value;
}

void set_secure_region(void)
{
	/* Mark the first 256 MB of DDR High as Secure.
	 * This code is provided as example and doesn't affect on security levels at VS_EN = 1
	 * but can be useful for test purpose in case of boot with VS_EN = 0.
	 */
	writel(BASE_SECURE_REGION_LOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_START);
	writel(BASE_SECURE_REGION_HIGH(0),
	       (uint32_t)(SECURE_REGIONS_PHYS_ADDR_START >> 32ULL) & 0xF);
	writel(MASK_SECURE_REGION_LOW(0), (uint32_t)SECURE_REGIONS_PHYS_ADDR_MASK);
	writel(MASK_SECURE_REGION_HIGH(0),
	       (uint32_t)(SECURE_REGIONS_PHYS_ADDR_MASK >> 32ULL) & 0xF);
	writel(CTR_SECURE_REGION, 0x1);

	/* TODO: Currently we are support one secure region started from the beginning of
	 * DDR High. The region size must be a power of 2. It is required to modify of
	 * ddrinfo struct to add several regions dynamically.
	 */
	struct ddrinfo *info = (struct ddrinfo *)MEM_REGIONS_VIRT_ADDR;
	for (int i = 0; i < MAX_MEM_REGIONS; ++i) {
		uint64_t start = info->mem_regions[i].start;
		uint64_t end = info->mem_regions[i].start + info->mem_regions[i].size;
		if ((SECURE_REGIONS_PHYS_ADDR_START >= start) &&
		    (SECURE_REGIONS_PHYS_ADDR_END <= end)) {
			info->mem_regions[i].start = SECURE_REGIONS_PHYS_ADDR_END;
			info->mem_regions[i].size = end - SECURE_REGIONS_PHYS_ADDR_END;
		}
	}
}
