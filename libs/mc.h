// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __MC_H__
#define __MC_H__

/*
 * Read C0 coprocessor register.
 */
#define mips_read_c0_register(reg)                                             \
	({                                                                     \
		unsigned __value;                                              \
		__asm__ volatile("mfc0   %0, $%1" : "=r"(__value) : "K"(reg)); \
		__value;                                                       \
	})

/*
 * Write C0 coprocessor register.
 */
#define mips_write_c0_register(reg, value)                                \
	do {                                                              \
		__asm__ volatile("mtc0   %z0, $%1 \n nop \n  nop \n  nop" \
				 :                                        \
				 : "r"((unsigned int)(value)), "K"(reg)); \
	} while (0)

/*--------------------------------------
 * Coprocessor 0 registers.
 */
#define C0_INDEX    0 /* индекс доступа к TLB */
#define C0_RANDOM   1 /* индекс TLB для команды Write Random */
#define C0_ENTRYLO0 2 /* строки для чётных страниц TLB */
#define C0_ENTRYLO1 3 /* строки для нечётных страниц TLB */
#define C0_CONTEXT  4 /* указатель на таблицу PTE */
#define C0_PAGEMASK 5 /* маска размера страниц TLB */
#define C0_WIRED    6 /* граница привязанных строк TLB */
#define C0_BADVADDR 8 /* виртуальный адрес последнего исключения */
#define C0_COUNT    9 /* таймер */
#define C0_ENTRYHI  10 /* информация соответствия виртуального адреса */
#define C0_COMPARE  11 /* предельное значение для прерывания по таймеру */
#define C0_STATUS   12 /* режимы функционирования процессора */
#define C0_CAUSE    13 /* причина последнего исключения */
#define C0_EPC	    14 /* адрес возврата из исключения */
#define C0_PRID	    15 /* идентификатор процессора */
#define C0_CONFIG   16 /* информация о возможностях процессора */
#define C0_LLADDR   17 /* физический адрес последней команды LL */
#define C0_ERROREPC 30 /* адрес возврата из исключения ошибки */

#define mips_get_core_id() (0)

/*
 * EXCEPTION TABLE
 */
#define EXC_INT	 0 /* Int  Прерывание */
#define EXC_MOD	 1 /* Mod  TLB-исключение модификации */
#define EXC_TLBL 2 /* TLBL TLB-исключение (загрузка или вызов команды) */
#define EXC_TLBS 3 /* TLBS TLB-исключение (сохранение) */
#define EXC_ADEL 4 /* AdEL Прерывание по ошибке адресации (загрузка или вызов команды) */
#define EXC_ADES 5 /* AdES Прерывание по ошибке адресации (сохранение) */
#define EXC_SYS	 8 /* Sys  Системное  исключение */
#define EXC_BP	 9 /* Bp   Исключение Breakpoint */
#define EXC_RI	 10 /* RI   Исключение зарезервированной команды */
#define EXC_CPU	 11 /* CpU  Исключение недоступности сопроцессора */
#define EXC_OV	 12 /* Ov   Исключение целочисленного переполнения */
#define EXC_TR	 13 /* Tr   Исключение Trap  */

#endif /* __MC_H__ */
