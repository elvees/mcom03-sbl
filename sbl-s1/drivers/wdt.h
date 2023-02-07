// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef WDT_H
#define WDT_H

#include <mcom03-errors.h>
#include <stdint.h>
#include <stddef.h>

#define WDT0 (0U)
#define WDT1 (1U)

#define WDT_RST_MODE (0)
#define WDT_IRQ_MODE (1)

#define WDT_RST_PULSE_LEN_2   (0)
#define WDT_RST_PULSE_LEN_4   (1)
#define WDT_RST_PULSE_LEN_8   (2)
#define WDT_RST_PULSE_LEN_16  (3)
#define WDT_RST_PULSE_LEN_32  (4)
#define WDT_RST_PULSE_LEN_64  (5)
#define WDT_RST_PULSE_LEN_128 (6)
#define WDT_RST_PULSE_LEN_256 (7)

#define WDT_OFF (0)
#define WDT_ON	(1)

#define WDT_MAX_TIMEOUT_COEF (0xFUL)
#define WDT_MIN_TIMEOUT_COEF (0x0UL)

#define WDT_MAX_TIMEOUT (0x7FFFFFFFUL)
#define WDT_MIN_TIMEOUT (0x0UL)

#define WDT_NUM_TOPS \
	16 /* There are sixteen TOPs (timeout periods) that can be set in the watchdog. */

//  регистра WDT_CR
// bits 31:5       -                  Резерв
// bits 4:2        RPL                Продолжительность импульса системного сброса в тактах опорной
//                                частоты:
//                                000 – 2 такта
//                                001 – 4 такта
//                                010 – 8 тактов
//                                011 – 16 тактов
//                                100 – 32 такта
//                                101 – 64 такта
//                                110 – 128 тактов
//                                111 – 256 тактов
// bit 1          RMOD               Включение прерывания.
//                                0: после таймаута генерируется системный сброс.
//                                1: после таймаута генерируется прерывание. Если к следующему таймауту прерывание не будет сброшено, то генерируется системный сброс.
// bit 0          WDT_EN             Бит включения WDT. Пока WDT не включен, его счетчик не декрементируется, а значит не генерирует прерывание и системный сброс. Будучи однажды включённым, WDT выключается только после генерации системного сброса.
#define WDT_CR_OFFSET	  0UL
#define WDT_CR_MASK	  0x1FUL
#define WDT_CR_RESETVALUE 0x2UL

#define WDT_CR_RPL_POS	    2UL
#define WDT_CR_RPL_MASK	    (0x7UL << WDT_CR_RPL_POS)
#define WDT_CR_RPL(val)	    (((val)&0x7UL) << WDT_CR_RPL_POS)
#define WDT_CR_RPL_GET(reg) (((reg) >> WDT_CR_RPL_POS) & 0x7UL)

#define WDT_CR_RMOD_POS	     1UL
#define WDT_CR_RMOD_MASK     (0x1UL << WDT_CR_RMOD_POS)
#define WDT_CR_RMOD(val)     (((val)&0x1UL) << WDT_CR_RMOD_POS)
#define WDT_CR_RMOD_GET(reg) (((reg) >> WDT_CR_RMOD_POS) & 0x1UL)

#define WDT_CR_WDT_EN_POS      0UL
#define WDT_CR_WDT_EN_MASK     (0x1UL << WDT_CR_WDT_EN_POS)
#define WDT_CR_WDT_EN(val)     (((val)&0x1UL) << WDT_CR_WDT_EN_POS)
#define WDT_CR_WDT_EN_GET(reg) (((reg) >> WDT_CR_WDT_EN_POS) & 0x1UL)

// регистра WDT_TORR
// bits 31:8       -              Резерв
// bits 7:4                   TOP_INT        Период таймаута для инициализации.
//                            Значение этого поля будет записано в TOP после включения таймера.
//                            Запись в регистр возможна после системного сброса, но до включения WDT.
// bits 3:0                   TOP            Период таймаута.
//                            Поле хранит значение, с которого начинает отсчёт таймер.
//                            Изменение этого поля будет иметь эффект только после сброса таймера.
//                            Период таймаута рассчитывается следующим образом.
//                            T = 2(16 + i), где T – период, выраженный в тактах опорной частоты, а i – значение поля TOP.
#define WDT_TORR_OFFSET 0x4UL
#define WDT_TORR_MASK	0xFF
#define WDT_TORR_RESETVALUE

#define WDT_TORR_TOP_INT_POS	  4UL
#define WDT_TORR_TOP_INT_MASK	  (0xFUL << WDT_TORR_TOP_INT_POS)
#define WDT_TORR_TOP_INT(val)	  (((val)&0xFUL) << WDT_TORR_TOP_INT_POS)
#define WDT_TORR_TOP_INT_GET(reg) (((reg) >> WDT_TORR_TOP_INT_POS) & 0xFUL)

#define WDT_TORR_TOP_POS      (0UL)
#define WDT_TORR_TOP_MASK     (0xFUL << WDT_TORR_TOP_POS)
#define WDT_TORR_TOP(val)     (((val)&0xFUL) << WDT_TORR_TOP_POS)
#define WDT_TORR_TOP_GET(reg) (((reg) >> WDT_TORR_TOP_POS) & 0xFUL)

// регистра WDT_CCVR
// bits 31:0       CCVR           Регистр содержит текущее значение счетчика WDT.
#define WDT_CCVR_OFFSET	    0x8UL
#define WDT_CCVR_RESETVALUE 0x7FFFFFFFUL
#define WDT_CCVR_POS	    0UL
#define WDT_CCVR_MASK	    (0xFFFFFFFFUL << WDT_CCVR_POS)
#define WDT_CCVR(val)	    (((val)&0xFFFFFFFFUL) << WDT_CCVR_POS)
#define WDT_CCVR_GET(reg)   (((reg) >> WDT_CCVR_POS) & 0xFFFFFFFFUL)

// регистра WDT_CRR
// bits 31:8        -             Резерв
// bits 7:0        CRR            Для перезапуска счётчика необходимо записать 0x76 в это поле. Также, это сбросит прерывание от WDT.
#define WDT_CRR_OFFSET	   0xCUL
#define WDT_CRR_RESETVALUE 0UL
#define WDT_CRR_POS	   0UL
#define WDT_CRR_MASK	   (0xFFUL << WDT_CRR_POS)
#define WDT_CRR(val)	   (((val)&0xFFUL) << WDT_CRR_POS)
#define WDT_CRR_GET(reg)   (((reg) >> WDT_CRR_POS) & 0xFFUL)

// регистра WDT_STAT
// bits 31:1     -              Резерв
// bits 0          ISR            Статус прерывания WDT:
//                            1 – прерывание активно.
//                            0 – прерывания не активно
#define WDR_STAT_OFFSET	      0x10UL
#define WDT_STAT_RESETVALUE   0x0UL
#define WDT_STAT_ISR_POS      0UL
#define WDT_STAT_ISR_MASK     (0x1UL << WDT_STAT_ISR_POS)
#define WDT_STAT_ISR(val)     (((val)&0x1UL) << WDT_STAT_ISR_POS)
#define WDT_STAT_ISR_GET(reg) (((reg) >> WDT_STAT_ISR_POS) & 0x1UL)

// регистра WDT_EOI
// bits 31:1       -               Резерв
// bits 0          ICR             Чтение этого регистра сбрасывает прерывание WDT.
#define WDT_EOI_OFFSET	     0x14UL
#define WDT_EOI_RESETVALUE   0x0UL
#define WDT_EOI_ICR_POS	     0UL
#define WDT_EOI_ICR_MASK     (0x1UL << WDT_EOI_ICR_POS)
#define WDT_EOI_ICR(val)     (((val)&0x1UL) << WDT_EOI_ICR_POS)
#define WDT_EOI_ICR_GET(reg) (((reg) >> WDT_EOI_ICR_POS) & 0x1UL)

#define WDT_CRR_RESET_VALUE 0x76U

#define WDT_RPL_PCLK_CYCLES2   (0x0)
#define WDT_RPL_PCLK_CYCLES4   (0x1)
#define WDT_RPL_PCLK_CYCLES8   (0x2)
#define WDT_RPL_PCLK_CYCLES16  (0x3)
#define WDT_RPL_PCLK_CYCLES32  (0x4)
#define WDT_RPL_PCLK_CYCLES64  (0x5)
#define WDT_RPL_PCLK_CYCLES128 (0x6)
#define WDT_RPL_PCLK_CYCLES256 (0x7)

typedef struct {
	volatile uint32_t
		WDT_CR; ///> \brief Offset: 0x0000 (R/W 32) Control Register Reset Value: Register fields WDT_DFLT_RPL (3 bits)...
	volatile uint32_t
		WDT_TORR; ///> \brief Offset: 0x0004 (R/W 32) Timeout Range Register Reset Value: WDT_TOP_RST...
	volatile uint32_t
		WDT_CCVR; ///> \brief Offset: 0x0008 (R/  32) Current Counter Value Register. Reset Value: WDT_CNT_RST...
	volatile uint32_t
		WDT_CRR; ///> \brief Offset: 0x000C ( /W 32) Counter Restart Register Reset Value: 0x0...
	volatile uint32_t
		WDT_STAT; ///> \brief Offset: 0x0010 (R/  32) Interrupt Status Register Reset Value: 0x0...
	volatile uint32_t
		WDT_EOI; ///> \brief Offset: 0x0014 (R/  32) Interrupt Clear Register Reset Value: 0x0...
} wdt_regs_t;

typedef struct {
	uint32_t id;
	wdt_regs_t *regs;
	uint32_t wdt_freq;
	uint32_t rpl; // Reset pulse length
	uint32_t rmod; // IRQ mode
	uint32_t timeout;
	uint32_t pretimeout;
	uint32_t min_timeout;
	uint32_t max_timeout;
	uint32_t status; // Run or stop wdt status
} wdt_dev_t;

int wdt_is_enabled(wdt_dev_t *wdt_dev);
mcom_err_t wdt_init(wdt_dev_t *wdt_dev);
mcom_err_t wdt_reset(wdt_dev_t *wdt_dev);
mcom_err_t wdt_stop(wdt_dev_t *wdt_dev);
mcom_err_t wdt_start(wdt_dev_t *wdt_dev);
mcom_err_t wdt_set_timeout(wdt_dev_t *wdt_dev, uint32_t timeout);
mcom_err_t wdt_reset_irq(wdt_dev_t *wdt_dev);

uint32_t wdt_get_timeleft(wdt_dev_t *wdt_dev);
uint32_t wdt_get_timeout(wdt_dev_t *wdt_dev);
uint32_t wdt_get_min_timeout(wdt_dev_t *wdt_dev);
uint32_t wdt_get_max_timeout(wdt_dev_t *wdt_dev);

#endif /* WDT_H */
