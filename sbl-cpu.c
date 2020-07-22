/*
 * Copyright 2020 RnD Center "ELVEES", JSC
 */

#include <stdint.h>
#include <string.h>

#define DDRINIT_START_ADDR	0x0
#define TFA_START_ADDR		0xC0000000
#define UBOOT_START_ADDR	0xC0080000

#define WRITE_CPU_START_ADDR_REG(reg, val) \
	do { \
		writel((reg), ((uint64_t)(val)) >> 32); \
		writel((reg) + 4, ((uint64_t)(val)) & 0xffffffff); \
	} while (0)

extern unsigned long __ddrinit_start, __ddrinit_end;
extern unsigned long __tfa_start, __tfa_end;
extern unsigned long __uboot_start, __uboot_end;

static inline void writel(unsigned long addr, uint32_t value)
{
	(*(volatile uint32_t *)addr) = value;
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

int main(void)
{
	uint32_t size;
	void (*start_ddrinit)(void) = (void *)DDRINIT_START_ADDR;
	void (*start_tfa)(void) = (void *)TFA_START_ADDR;
	unsigned long *start = (unsigned long *)&__ddrinit_start;
	unsigned long *end = (unsigned long *)&__ddrinit_end;

	/* Relocate ddrinit */
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)DDRINIT_START_ADDR, (void *)start, size);

	start_ddrinit();

	/* Relocate TF-A */
	start = (unsigned long *)&__tfa_start;
	end = (unsigned long *)&__tfa_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)TFA_START_ADDR, (void *)start, size);

	/* Setup CPU cores start addresses */
	WRITE_CPU_START_ADDR_REG(0x1000118, TFA_START_ADDR);
	WRITE_CPU_START_ADDR_REG(0x1000120, TFA_START_ADDR);
	WRITE_CPU_START_ADDR_REG(0x1000128, TFA_START_ADDR);
	WRITE_CPU_START_ADDR_REG(0x1000130, TFA_START_ADDR);

	/* Relocate U-Boot */
	start = (unsigned long *)&__uboot_start;
	end = (unsigned long *)&__uboot_end;
	size = (unsigned long)end - (unsigned long)start;
	memcpy((void *)UBOOT_START_ADDR, (void *)start, size);

	start_tfa();

	return 0;
}
