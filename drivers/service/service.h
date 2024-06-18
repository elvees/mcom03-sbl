// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __SERVICE_URB_H__
#define __SERVICE_URB_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include <mcom03.h>
#include <mcom03-errors.h>

typedef struct {
	volatile unsigned int cpu_ppolicy;
	volatile unsigned int cpu_pstatus;
	volatile unsigned int sdr_ppolicy;
	volatile unsigned int sdr_pstatus;
	volatile unsigned int media_ppolicy;
	volatile unsigned int media_pstatus;
	volatile unsigned int core_ppolicy;
	volatile unsigned int core_pstatus;
	volatile unsigned int hsperiph_subs_ppolicy;
	volatile unsigned int hsperiph_subs_pstatus;
	volatile unsigned int lsperiph0_subs_ppolicy;
	volatile unsigned int lsperiph0_subs_pstatus;
	volatile unsigned int lsperiph1_subs_ppolicy;
	volatile unsigned int lsperiph1_subs_pstatus;
	volatile unsigned int ddr_subs_ppolicy;
	volatile unsigned int ddr_subs_pstatus;
	volatile unsigned int top_subs_ppolicy;
	volatile unsigned int top_subs_pstatus;
	volatile unsigned int risc0_ppolicy;
	volatile unsigned int risc0_pstatus;
	volatile unsigned int reserved0[1004];
	volatile unsigned int service_subs_pllcnfg;
	volatile unsigned int service_subs_plldiag;
	volatile unsigned int top_clkgate;
	volatile unsigned int reserved1[1021];
	volatile unsigned int boot;
	volatile unsigned int qspi0_xip_en_req;
	volatile unsigned int qspi0_xip_en_out;
	volatile unsigned int qspi0_xip_cfg;
	volatile unsigned int axicore_dlock;
	volatile unsigned int axisys_dlock;
	volatile unsigned int tscount_high;
	volatile unsigned int tscount_low;
	volatile unsigned int extint_cfg;
	volatile unsigned int pads_ren;
	volatile unsigned int reserved2[1014];
	volatile unsigned int fuse0;
	volatile unsigned int fuse1;
	volatile unsigned int otp_mode;
	volatile unsigned int otp_flag;
	volatile unsigned int fuse0_ecc;
	volatile unsigned int fuse1_ecc;
	volatile unsigned int otp_ecc;
	volatile unsigned int reserved3[1];
	volatile unsigned int axicore_internal_priv;
	volatile unsigned int axisys_internal_priv;
	volatile unsigned int reserved4[2];
	volatile unsigned int tp_dbgen;
	volatile unsigned int sdr_dbgen;
	volatile unsigned int sp_dbgen;
	volatile unsigned int s_dbgen;
	volatile unsigned int ust_dbgen;
} service_urb_regs_t;

/**
 * @brief The function returns a pointer
 *        to the URB register address
 *        of the Service subsystem
 *
 * @param ucg_id - UCG number
 *
 * @return Pointer to the UCG register address
 *         of the Service subsystem
 */
service_urb_regs_t *service_get_urb_registers(void);

/**
 * @brief Function for setting the top_clkgate register values
 *        of the Service subsystem
 *
 * @param urb The pointer to urb registers service subs
 * @param top_clkgate value of top_clkgate register.
 *
 * @return Error value
 */
mcom_err_t service_set_top_clkgate(service_urb_regs_t *urb, uint32_t top_clkgate);

/**
 * @brief Function for getting the values of the top_clkgate register
 *        of the Service subsystem
 *
 * @param urb The pointer to urb registers service subs subs
 * @param top_clkgate Value of top_clkgate register
 *
 * @return Error value
 */
mcom_err_t service_get_top_clkgate(service_urb_regs_t *urb, uint32_t *top_clkgate);

/**
 * @brief The function clocks and releases the ls_periph 0 subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph0_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal ls_periph 0
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph0_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout);

/**
 * @brief The function clocks and releases the ls_periph 1 subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph1_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal ls_periph 1
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_ls_periph1_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout);

/**
 * @brief The function clocks and releases the hs_periph subsystem reset
 * @param urb The pointer to urb registers service subs subs
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_hs_periph_deassert(service_urb_regs_t *urb, uint32_t timeout);

/**
 * @brief The function removes the clock signal
 *            and sets the subsystem reset signal hs_periph
 *
 * @param urb The pointer to urb registers
 * @param reset_mode The reset mode (0x01 - Power Off, 0x08 - Warm Reset)
 * @param timeout The timeout for set values
 *
 * @return Error value
 */
mcom_err_t service_reset_hs_periph_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                          uint32_t timeout);

/**
 * @brief The function return state of boot register.
 *
 * @param urb The pointer to urb registers
 * @param boot Value boot register
 *
 * @return Error value
 */
mcom_err_t service_get_boot_setup(service_urb_regs_t *urb, uint32_t *boot);

/**
 * @brief Function for getting the timestamp value from the counter register
 *
 * @param urb The pointer to urb registers
 * @param timestamp Value timestamp
 *
 * @return Error value
 */
mcom_err_t service_get_timestamp(service_urb_regs_t *urb, uint64_t *timestamp);

/**
 * @brief The function set state of register tp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_tp_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register tp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable) *
 *
 * @return The state of tp_dbgen register.
 */
mcom_err_t service_get_tp_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register sdr_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_sdr_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register sdr_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_sdr_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register sp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_sp_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register sp_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_sp_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register s_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_s_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register s_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_s_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

/**
 * @brief The function set state of register ust_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_set_ust_dbgen(service_urb_regs_t *urb, bool dbg_enable);

/**
 * @brief The function get state of register ust_dbgen
 *
 * @param urb The pointer to urb registers service subs
 * @param dbg_enable Debug status (true - enable, false - disable)
 *
 * @return Error value
 */
mcom_err_t service_get_ust_dbgen(service_urb_regs_t *urb, bool *dbg_enable);

#endif /* __SERVICE_URB_H__ */
