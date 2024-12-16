// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#pragma once

#include <libs/mmio.h>
#include <libs/utils-def.h>

// PPolicy
#define PP_OFF      0x01
#define PP_WARM_RST 0x08
#define PP_ON       0x10

#define SET_PPOLICY(addr, value)                                                             \
	({                                                                                   \
		mmio_write_32((uintptr_t)(addr), (value) & 0x1F);                            \
		while ((mmio_read_32((uintptr_t)(addr) + 0x4) & 0x1F) != ((value) & 0x1F)) { \
		}                                                                            \
	})

#define USEC_IN_SEC  1000000ULL
#define MSEC_IN_SEC  1000ULL
#define USEC_IN_MSEC 1000ULL

#define plat_panic_handler(fmt, ...)                                      \
	({                                                                \
		ERROR("%s: %d: " fmt, __FILE__, __LINE__, ##__VA_ARGS__); \
		while (1) {                                               \
			/* ... */                                         \
		}                                                         \
	})

/* platform barriers */
static inline void plat_mb(void)
{
	__asm__ volatile("sync (0)" : : : "memory");
}

static inline void plat_wmb(void)
{
	__asm__ volatile("sync (4)" : : : "memory");
}

static inline void plat_rmb(void)
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
static inline uintptr_t plat_convert_va_to_pa(const void *va)
{
	uintptr_t addr = (uintptr_t)va;
	if (addr >= 0xC0000000UL)
		return addr;
	if (addr >= 0xA0000000UL)
		return (addr - 0xA0000000UL);
	if (addr < 0x80000000UL)
		return (addr + 0x40000000UL);
	return (addr & 0x1FFFFFFFUL);
}

/**
 * @brief Physical to Virtual Address conversion function
 *        for MIPS with Fixed Map Memory setting (Uncached)
 *
 * @param addr - Physical Address
 *
 * @return Virtual Address
 */
static inline void *plat_convert_pa_to_va(uintptr_t pa)
{
	uintptr_t addr = (uintptr_t)pa;
	if (addr < 0x40000000UL)
		return (void *)(addr + 0xA0000000UL);
	if (addr < 0xC0000000UL)
		return (void *)(addr - 0x40000000UL);
	return (void *)(addr);
}

void plat_irq_handler(void);
void plat_disable_irq_global(void);
void plat_enable_irq_global(void);
uintptr_t plat_map64(uint64_t addr64);
void plat_unmap64(uintptr_t addr32);
void *plat_allocate_memory(uint32_t size);
void *plat_reallocate_memory(void *ptr, uint32_t size);
void plat_free_memory(void *ptr);
void plat_time_delay(uint32_t num_msec);
void plat_time_delay_us(uint32_t num_usec);
uint32_t get_cpu_frequency(void);
void set_secure_region(void);
int soc_debug_if_disable(void);
