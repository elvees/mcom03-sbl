// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>

#define BASE_ADDR_DDR0_PHY_REG 0xa4000000
#define BASE_ADDR_DDR0_UMCTL2  0xa6000000

#define BASE_ADDR_DDR1_PHY_REG 0xa8000000
#define BASE_ADDR_DDR1_UMCTL2  0xaa000000

#define BASE_ADDR_DDR_SYS_URB  0xac000000
#define BASE_ADDR_DDR_SYS_UCG0 0xac010000
#define BASE_ADDR_DDR_SYS_UCG1 0xac020000

#define DDR_SYS_URB_CTRSECUREREGION           (BASE_ADDR_DDR_SYS_URB + 0x80)
#define DDR_SYS_URB_BASESECUREREGIONLOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x90 + (num * 0x10)))
#define DDR_SYS_URB_BASESECUREREGIONHIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x94 + (num * 0x10)))
#define DDR_SYS_URB_MASKSECUREREGIONLOW(num)  (BASE_ADDR_DDR_SYS_URB + (0x98 + (num * 0x10)))
#define DDR_SYS_URB_MASKSECUREREGIONHIGH(num) (BASE_ADDR_DDR_SYS_URB + (0x9C + (num * 0x10)))

#define DDR_DDRMC_MAX_NUMBER      2
#define DDR_MAX_MEM_REGIONS       4
#define DDR_MEM_REGIONS_VIRT_ADDR 0xC0000000

struct ddrinfo {
	uint64_t dram_size[DDR_DDRMC_MAX_NUMBER];
	uint64_t total_dram_size;
	struct {
		bool enable;
		int channels;
		int size;
	} interleaving;
	int speed[DDR_DDRMC_MAX_NUMBER];
	// RAM configuration
	struct {
		uint64_t start;
		uint64_t size;
	} mem_regions[DDR_MAX_MEM_REGIONS];
};
