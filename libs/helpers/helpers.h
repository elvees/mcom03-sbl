// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <libs/mmio.h>
#include <libs/utils-def.h>

// PPolicy
#define PP_OFF      0x01
#define PP_WARM_RST 0x08
#define PP_ON       0x10
#define PP_MASK     0x1F

#define SET_PPOLICY(addr, value)                                                             \
	({                                                                                   \
		mmio_write_32((uintptr_t)(addr), (value) & 0x1F);                            \
		while ((mmio_read_32((uintptr_t)(addr) + 0x4) & 0x1F) != ((value) & 0x1F)) { \
		}                                                                            \
	})

// platform barriers
static inline void mem_barrier(void)
{
	__asm__ volatile("sync (0)" : : : "memory");
}

static inline void wmem_barrier(void)
{
	__asm__ volatile("sync (4)" : : : "memory");
}

static inline void rmem_barrier(void)
{
	__asm__ volatile("sync (19)" : : : "memory");
}

/**
 * @brief Virtual to Physical Address conversion function
 *        for MIPS with Fixed Map Memory setting
 *
 * @param addr - Virtual Address
 *
 * @return Physical Address
 */
static inline uintptr_t convert_va_to_pa(const void *va)
{
	uintptr_t addr = (uintptr_t)va;
	if (addr >= UL(0xC0000000))
		return addr;
	if (addr >= UL(0xA0000000))
		return (addr - UL(0xA0000000));
	if (addr < UL(0x80000000))
		return (addr + UL(0x40000000));
	return (addr & UL(0x1FFFFFFF));
}

/**
 * @brief Physical to Virtual Address conversion function
 *        for MIPS with Fixed Map Memory setting (Uncached)
 *
 * @param addr - Physical Address
 *
 * @return Virtual Address
 */
static inline void *convert_pa_to_va(uintptr_t pa)
{
	uintptr_t addr = (uintptr_t)pa;
	if (addr < UL(0x40000000))
		return (void *)(addr + UL(0xA0000000));
	if (addr < UL(0xC0000000))
		return (void *)(addr - UL(0x40000000));
	return (void *)(addr);
}

int set_ppolicy(uintptr_t reg, uint32_t new_policy, uint32_t bp0_mask, uint32_t bp1_mask,
                uint32_t timeout_us, bool is_delay_required);
int setup_ddr_regions(void);
