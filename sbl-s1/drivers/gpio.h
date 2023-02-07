// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __GPIO_H__
#define __GPIO_H__

#include <utils-def-exp.h>
#include <mcom03.h>

// clang-format off
/**
 * @brief Structure of the GPIO registers
 *
 */
typedef struct
{
    volatile unsigned int swporta_dr;     /**< \brief Offset: 0x000 (R/W 32) GPIO Port A Data Register                */
    volatile unsigned int swporta_ddr;    /**< \brief Offset: 0x004 (R/W 32) GPIO Port A Direction Register           */
    volatile unsigned int swporta_ctl;    /**< \brief Offset: 0x008 (R/W 32) GPIO Port A Control Register             */
    volatile unsigned int swportb_dr;     /**< \brief Offset: 0x00C (R/W 32) GPIO Port B Data Register                */
    volatile unsigned int swportb_ddr;    /**< \brief Offset: 0x010 (R/W 32) GPIO Port B Direction Register           */
    volatile unsigned int swportb_ctl;    /**< \brief Offset: 0x014 (R/W 32) GPIO Port B Control Register             */
    volatile unsigned int swportc_dr;     /**< \brief Offset: 0x018 (R/W 32) GPIO Port C Data Register                */
    volatile unsigned int swportc_ddr;    /**< \brief Offset: 0x01C (R/W 32) GPIO Port C Direction Register           */
    volatile unsigned int swportc_ctl;    /**< \brief Offset: 0x020 (R/W 32) GPIO Port C Control Register             */
    volatile unsigned int swportd_dr;     /**< \brief Offset: 0x024 (R/W 32) GPIO Port D Data Register                */
    volatile unsigned int swportd_ddr;    /**< \brief Offset: 0x028 (R/W 32) GPIO Port D Direction Register           */
    volatile unsigned int swportd_ctl;    /**< \brief Offset: 0x02C (R/W 32) GPIO Port D Control Register             */
    volatile unsigned int int_enable;     /**< \brief Offset: 0x030 (R/W 32) GPIO Port A IRQ Enable Register          */
    volatile unsigned int int_mask;       /**< \brief Offset: 0x034 (R/W 32) GPIO Port A IRQ Mask Register            */
    volatile unsigned int int_type_lvl;   /**< \brief Offset: 0x038 (R/W 32) GPIO Port A IRQ Level Register           */
    volatile unsigned int int_polarity;   /**< \brief Offset: 0x03C (R/W 32) GPIO Port A IRQ Polarity Register        */
    volatile unsigned int int_status;     /**< \brief Offset: 0x040 (R/  32) GPIO Port A IRQ Status Register          */
    volatile unsigned int raw_int_status; /**< \brief Offset: 0x044 (R/  32) GPIO Port A Unmasked IRQ Status Register */
    volatile unsigned int debounce;       /**< \brief Offset: 0x048 (R/W 32) GPIO Port A Debounce Enable              */
    volatile unsigned int porta_eoi;      /**< \brief Offset: 0x04C ( /W 32) GPIO Port A IRQ Clear Register           */
    volatile unsigned int ext_porta;      /**< \brief Offset: 0x050 (R/  32) GPIO Port A External Register            */
    volatile unsigned int ext_portb;      /**< \brief Offset: 0x054 (R/  32) GPIO Port B External Register            */
    volatile unsigned int ext_portc;      /**< \brief Offset: 0x058 (R/  32) GPIO Port C External Register            */
    volatile unsigned int ext_portd;      /**< \brief Offset: 0x05C (R/  32) GPIO Port D External Register            */
} gpio_regs_t;
// clang-format on

/* --------  GPIO_SW_PORTX_DR : (R/W 32) GPIO Port X Data Register -------- */
/* bit:  0..31  value   */

#define GPIO_SW_PORTA_DR_OFFSET	    0x000
#define GPIO_SW_PORTA_DR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTB_DR_OFFSET	    0x00C
#define GPIO_SW_PORTB_DR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTC_DR_OFFSET	    0x018
#define GPIO_SW_PORTC_DR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTD_DR_OFFSET	    0x024
#define GPIO_SW_PORTD_DR_RESETVALUE U(0x00000000)

#define GPIO_SW_PORTX_DR_Pos	0
#define GPIO_SW_PORTX_DR_Msk	(U(0xFFFFFFFF) << GPIO_SW_PORTX_DR_Pos)
#define GPIO_SW_PORTX_DR(value) (GPIO_SW_PORTX_DR_Msk & ((value) << GPIO_SW_PORTX_DR_Pos))
#define GPIO_SW_PORTX_DR_MASK	U(0xFFFFFFFF)

/* --------  GPIO_SW_PORTX_DDR : (R/W 32) GPIO Port X Direction Register -------- */
/* bit:  0..31  value   */

#define GPIO_SW_PORTA_DDR_OFFSET     0x004
#define GPIO_SW_PORTA_DDR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTB_DDR_OFFSET     0x010
#define GPIO_SW_PORTB_DDR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTC_DDR_OFFSET     0x01C
#define GPIO_SW_PORTC_DDR_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTD_DDR_OFFSET     0x028
#define GPIO_SW_PORTD_DDR_RESETVALUE U(0x00000000)

#define GPIO_SW_PORTX_DDR_Pos	 0
#define GPIO_SW_PORTX_DDR_Msk	 (U(0xFFFFFFFF) << GPIO_SW_PORTX_DDR_Pos)
#define GPIO_SW_PORTX_DDR(value) (GPIO_SW_PORTX_DDR_Msk & ((value) << GPIO_SW_PORTX_DDR_Pos))

#define GPIO_SW_PORTX_DDR_OUTPUT U(0x1)
#define GPIO_SW_PORTX_DDR_INPUT	 U(0x0)

#define GPIO_SW_PORTX_DDR_MASK U(0xFFFFFFFF)

/* --------  GPIO_SW_PORTX_CTL : (R/W 32) GPIO Port X Control Register -------- */
/* bit:  0..31  value   */

#define GPIO_SW_PORTA_CTL_OFFSET     0x008
#define GPIO_SW_PORTA_CTL_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTB_CTL_OFFSET     0x014
#define GPIO_SW_PORTB_CTL_RESETVALUE U(0x0000FFFF)
#define GPIO_SW_PORTC_CTL_OFFSET     0x0020
#define GPIO_SW_PORTC_CTL_RESETVALUE U(0x00000000)
#define GPIO_SW_PORTD_CTL_OFFSET     0x02C
#define GPIO_SW_PORTD_CTL_RESETVALUE U(0x00000000)

#define GPIO_SW_PORTX_CTL_Pos	 0
#define GPIO_SW_PORTX_CTL_Msk	 (U(0xFFFFFFFF) << GPIO_SW_PORTX_CTL_Pos)
#define GPIO_SW_PORTX_CTL(value) (GPIO_SW_PORTX_CTL_Msk & ((value) << GPIO_SW_PORTX_CTL_Pos))

#define GPIO_SW_PORTX_CTL_SW_MODE U(0x0)
#define GPIO_SW_PORTX_CTL_HW_MODE U(0x1)

#define GPIO_SW_PORTX_CTL_MASK U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_ENABLE : (R/W 32) GPIO Port A IRQ Enable Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_ENABLE_OFFSET	 0x030
#define GPIO_PORTA_INT_ENABLE_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_ENABLE_Pos 0
#define GPIO_PORTA_INT_ENABLE_Msk (U(0xFFFFFFFF) << GPIO_PORTA_INT_ENABLE_Pos)
#define GPIO_PORTA_INT_ENABLE(value) \
	(GPIO_PORTA_INT_ENABLE_Msk & ((value) << GPIO_PORTA_INT_ENABLE_Pos))
#define GPIO_PORTA_INT_ENABLE_MASK U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_MASK : (R/W 32) GPIO Port A IRQ Mask Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_MASK_OFFSET     0x034
#define GPIO_PORTA_INT_MASK_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_MASK_Pos	   0
#define GPIO_PORTA_INT_MASK_Msk	   (U(0xFFFFFFFF) << GPIO_PORTA_INT_MASK_Pos)
#define GPIO_PORTA_INT_MASK(value) (GPIO_PORTA_INT_MASK_Msk & ((value) << GPIO_PORTA_INT_MASK_Pos))
#define GPIO_PORTA_INT_MASK_MASK   U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_LVL : (R/W 32) GPIO Port A IRQ Level Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_LVL_OFFSET     0x038
#define GPIO_PORTA_INT_LVL_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_LVL_Pos	  0
#define GPIO_PORTA_INT_LVL_Msk	  (U(0xFFFFFFFF) << GPIO_PORTA_INT_LVL_Pos)
#define GPIO_PORTA_INT_LVL(value) (GPIO_PORTA_INT_LVL_Msk & ((value) << GPIO_PORTA_INT_LVL_Pos))
#define GPIO_PORTA_INT_LVL_MASK	  U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_POL : (R/W 32) GPIO Port A IRQ Polarity Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_POL_OFFSET     0x03C
#define GPIO_PORTA_INT_POL_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_POL_Pos	  0
#define GPIO_PORTA_INT_POL_Msk	  (U(0xFFFFFFFF) << GPIO_PORTA_INT_POL_Pos)
#define GPIO_PORTA_INT_POL(value) (GPIO_PORTA_INT_POL_Msk & ((value) << GPIO_PORTA_INT_POL_Pos))
#define GPIO_PORTA_INT_POL_MASK	  U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_STAT : (R/  32) GPIO Port A IRQ Status Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_STAT_OFFSET     0x040
#define GPIO_PORTA_INT_STAT_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_STAT_Pos	 0
#define GPIO_PORTA_INT_STAT_MASK U(0xFFFFFFFF)

/* --------  GPIO_PORTA_INT_RAW_STAT : (R/  32) GPIO Port A Unmasked IRQ Status Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_INT_RAW_STAT_OFFSET	   0x044
#define GPIO_PORTA_INT_RAW_STAT_RESETVALUE U(0x00000000)

#define GPIO_PORTA_INT_RAW_STAT_Pos  0
#define GPIO_PORTA_INT_RAW_STAT_MASK U(0xFFFFFFFF)

/* --------  GPIO_PORTA_DEBOUNCE : (R/W 32) GPIO Port A Debounce Enable -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_DEBOUNCE_OFFSET     0x048
#define GPIO_PORTA_DEBOUNCE_RESETVALUE U(0x00000000)

#define GPIO_PORTA_DEBOUNCE_Pos	   0
#define GPIO_PORTA_DEBOUNCE_Msk	   (U(0xFFFFFFFF) << GPIO_PORTA_DEBOUNCE_Pos)
#define GPIO_PORTA_DEBOUNCE(value) (GPIO_PORTA_DEBOUNCE_Msk & ((value) << GPIO_PORTA_DEBOUNCE_Pos))
#define GPIO_PORTA_DEBOUNCE_MASK   U(0xFFFFFFFF)

/* --------  GPIO_PORTA_EOI : ( /W 32) GPIO Port A IRQ Clear Register -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_EOI_OFFSET	  0x04C
#define GPIO_PORTA_EOI_RESETVALUE U(0x00000000)

#define GPIO_PORTA_EOI_Pos    0
#define GPIO_PORTA_EOI_Msk    (U(0xFFFFFFFF) << GPIO_PORTA_EOI_Pos)
#define GPIO_PORTA_EOI(value) (GPIO_PORTA_EOI_Msk & ((value) << GPIO_PORTA_EOI_Pos))
#define GPIO_PORTA_EOI_MASK   U(0xFFFFFFFF)

/* --------  GPIO_PORTX_EXT : (R/  32) GPIO Port X External Register  -------- */
/* bit:  0..31  value   */
#define GPIO_PORTA_EXT_OFFSET	  0x050
#define GPIO_PORTA_EXT_RESETVALUE U(0x00000000)
#define GPIO_PORTB_EXT_OFFSET	  0x054
#define GPIO_PORTB_EXT_RESETVALUE U(0x00000000)
#define GPIO_PORTC_EXT_OFFSET	  0x058
#define GPIO_PORTC_EXT_RESETVALUE U(0x00000000)
#define GPIO_PORTD_EXT_OFFSET	  0x05C
#define GPIO_PORTD_EXT_RESETVALUE U(0x00000000)

#define GPIO_PORTX_EXT_Pos    0
#define GPIO_PORTX_EXT_Msk    (U(0xFFFFFFFF) << GPIO_PORTX_EXT_Pos)
#define GPIO_PORTX_EXT(value) (GPIO_PORTX_EXT_Msk & ((value) << GPIO_PORTX_EXT_Pos))
#define GPIO_PORTX_EXT_MASK   U(0xFFFFFFFF)

/**
 * @brief GPIO pins define
 *
 */
#define GPIO_PIN_0	     U(0) /**< \brief Pin 0 selected    */
#define GPIO_PIN_1	     U(1) /**< \brief Pin 1 selected    */
#define GPIO_PIN_2	     U(2) /**< \brief Pin 2 selected    */
#define GPIO_PIN_3	     U(3) /**< \brief Pin 3 selected    */
#define GPIO_PIN_4	     U(4) /**< \brief Pin 4 selected    */
#define GPIO_PIN_5	     U(5) /**< \brief Pin 5 selected    */
#define GPIO_PIN_6	     U(6) /**< \brief Pin 6 selected    */
#define GPIO_PIN_7	     U(7) /**< \brief Pin 7 selected    */
#define GPIO_PIN_MASK(value) (U(0x1) << (value))
#define GPIO_PINS_MASK	     U(0xFF) /**< \brief All PIN mask      */

/**
 * @brief GPIO ports define
 *
 */
#define GPIO_PORTA U(0x0001) /**< \brief Port A selected    */
#define GPIO_PORTB U(0x0002) /**< \brief Port B selected    */
#define GPIO_PORTC U(0x0004) /**< \brief Port C selected    */
#define GPIO_PORTD U(0x0008) /**< \brief Port D selected    */

#define GPIO_DIR_INPUT	U(0x0)
#define GPIO_DIR_OUTPUT U(0x1)
#define GPIO_MODE_HW	U(0x1)
#define GPIO_MODE_SW	U(0x0)

#define GPIO_PIN_RESET U(0x0)
#define GPIO_PIN_SET   U(0x1)

void gpio0_init(unsigned int port, unsigned int pin, unsigned int mode, unsigned int direction);
void gpio1_init(unsigned int port, unsigned int pin, unsigned int mode, unsigned int direction);

unsigned int gpio0_read_pin(unsigned int port, unsigned int pin);
unsigned int gpio1_read_pin(unsigned int port, unsigned int pin);

void gpio0_write_pin(unsigned int port, unsigned int pin, unsigned int pin_state);
void gpio1_write_pin(unsigned int port, unsigned int pin, unsigned int pin_state);

#endif /* __GPIO_H__ */
