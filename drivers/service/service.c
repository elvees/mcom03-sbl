// Copyright 2023 RnD Center "ELVEES", JSC
// SPDX-License-Identifier: MIT

#include "ucg.h"
#include "service-urb.h"

service_urb_regs_t *service_get_urb_registers(void)
{
	return (service_urb_regs_t *)BASE_ADDR_SERVICE_URB;
}

mcom_err_t service_set_top_clkgate(service_urb_regs_t *urb, uint32_t top_clkgate)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	urb->top_clkgate = top_clkgate;
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_top_clkgate(service_urb_regs_t *urb, uint32_t *top_clkgate)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;
	if (top_clkgate == NULL)
		return MCOM03_ERROR_NULL;

	*top_clkgate = urb->top_clkgate;
	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_ls_periph0_deassert(service_urb_regs_t *urb, uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate |= SERVICE_TOP_CLK_GATE_LSPERIPH0;
	urb->top_clkgate = top_clkgate;

	ucg_regs_t *interconnect_ucg0 = ucg_get_top_registers(0);
	ucg_enable_bp(interconnect_ucg0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0);

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);
	ucg_enable_bp(interconnect_ucg1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM);

	urb->lsperiph0_subs_ppolicy = SERVICE_PPOLICY_PP_ON;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->lsperiph0_subs_pstatus == SERVICE_PPOLICY_PP_ON) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0);
	ucg_sync_and_disable_bp(interconnect_ucg1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM);

	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_ls_periph0_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((reset_mode != SERVICE_PPOLICY_PP_OFF) || (reset_mode != SERVICE_PPOLICY_PP_WARM_RST))
		return MCOM03_ERROR_INVALID_PARAM;

	ucg_regs_t *interconnect_ucg0 = ucg_get_top_registers(0);
	ucg_enable_bp(interconnect_ucg0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0);

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);
	ucg_enable_bp(interconnect_ucg1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM);

	urb->lsperiph0_subs_ppolicy = reset_mode;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->lsperiph0_subs_pstatus == reset_mode) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg0, TOP_UCG0_CHANNEL_DDR_LSPERIPH0);
	ucg_sync_and_disable_bp(interconnect_ucg1, TOP_UCG1_CHANNEL_AXI_SLOW_COMM);

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate &= ~(SERVICE_TOP_CLK_GATE_LSPERIPH0);
	urb->top_clkgate = top_clkgate;

	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_ls_periph1_deassert(service_urb_regs_t *urb, uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate |= SERVICE_TOP_CLK_GATE_LSPERIPH1;
	urb->top_clkgate = top_clkgate;

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);

	uint32_t bp_mask = (TOP_UCG1_CHANNEL_AXI_SLOW_COMM | TOP_UCG1_CHANNEL_DDR_LSPERIPH1);

	ucg_enable_bp(interconnect_ucg1, bp_mask);

	urb->lsperiph1_subs_ppolicy = SERVICE_PPOLICY_PP_ON;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->lsperiph1_subs_pstatus == SERVICE_PPOLICY_PP_ON) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg1, bp_mask);

	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_ls_periph1_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                           uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((reset_mode != SERVICE_PPOLICY_PP_OFF) || (reset_mode != SERVICE_PPOLICY_PP_WARM_RST))
		return MCOM03_ERROR_INVALID_PARAM;

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);

	uint32_t bp_mask = (TOP_UCG1_CHANNEL_AXI_SLOW_COMM | TOP_UCG1_CHANNEL_DDR_LSPERIPH1);

	ucg_enable_bp(interconnect_ucg1, bp_mask);

	urb->lsperiph1_subs_ppolicy = reset_mode;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->lsperiph1_subs_pstatus == reset_mode) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg1, bp_mask);

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate &= ~(SERVICE_TOP_CLK_GATE_LSPERIPH1);
	urb->top_clkgate = top_clkgate;

	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_hs_periph_deassert(service_urb_regs_t *urb, uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate |= SERVICE_TOP_CLK_GATE_HSPERIPH;
	urb->top_clkgate = top_clkgate;

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);

	uint32_t bp_mask = (TOP_UCG1_CHANNEL_AXI_SLOW_COMM | TOP_UCG1_CHANNEL_AXI_FAST_COMM |
	                    TOP_UCG1_CHANNEL_DDR_HSPERIPH);

	ucg_enable_bp(interconnect_ucg1, bp_mask);

	urb->hsperiph_subs_ppolicy = SERVICE_PPOLICY_PP_ON;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->hsperiph_subs_pstatus == SERVICE_PPOLICY_PP_ON) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg1, bp_mask);

	return MCOM03_SUCCESS;
}

mcom_err_t service_reset_hs_periph_assert(service_urb_regs_t *urb, uint32_t reset_mode,
                                          uint32_t timeout)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((reset_mode != SERVICE_PPOLICY_PP_OFF) || (reset_mode != SERVICE_PPOLICY_PP_WARM_RST))
		return MCOM03_ERROR_INVALID_PARAM;

	ucg_regs_t *interconnect_ucg1 = ucg_get_top_registers(1);

	uint32_t bp_mask = (TOP_UCG1_CHANNEL_AXI_SLOW_COMM | TOP_UCG1_CHANNEL_AXI_FAST_COMM |
	                    TOP_UCG1_CHANNEL_DDR_HSPERIPH);

	ucg_enable_bp(interconnect_ucg1, bp_mask);

	urb->hsperiph_subs_ppolicy = reset_mode;

	uint32_t is_timeout = (timeout) ? (1) : (0);
	for (;;) {
		if (urb->hsperiph_subs_pstatus == reset_mode) {
			break;
		}
		if (is_timeout) {
			__asm__ volatile("nop");
			timeout--;
			if (timeout == 0) {
				return MCOM03_ERROR_TIMEOUT;
			}
		}
	}
	ucg_sync_and_disable_bp(interconnect_ucg1, bp_mask);

	uint32_t top_clkgate = urb->top_clkgate;
	top_clkgate &= ~(SERVICE_TOP_CLK_GATE_HSPERIPH);
	urb->top_clkgate = top_clkgate;

	return MCOM03_SUCCESS;
}

mcom_err_t service_get_boot_setup(service_urb_regs_t *urb, uint32_t *boot)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;
	if (boot == NULL)
		return MCOM03_ERROR_NULL;

	*boot = urb->boot;
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_timestamp(service_urb_regs_t *urb, uint64_t *timestamp)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;
	if (timestamp == NULL)
		return MCOM03_ERROR_NULL;

	*timestamp = urb->tscount_high;
	*timestamp <<= 32;
	*timestamp |= urb->tscount_low;
	return MCOM03_SUCCESS;
}

mcom_err_t service_set_tp_dbgen(service_urb_regs_t *urb, bool dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if (dbg_enable == true) {
		urb->tp_dbgen = SERVICE_DBG_RISC0_ENABLE;
	} else {
		urb->tp_dbgen = SERVICE_DBG_RISC0_DISABLE;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_tp_dbgen(service_urb_regs_t *urb, bool *dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((urb->tp_dbgen & SERVICE_DBG_RISC0_ENABLE) != 0) {
		*dbg_enable = true;
	} else {
		*dbg_enable = false;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_set_sdr_dbgen(service_urb_regs_t *urb, bool dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if (dbg_enable == true) {
		urb->sdr_dbgen = SERVICE_DBG_ALL_ENABLE;
	} else {
		urb->sdr_dbgen = SERVICE_DBG_SDR_DISABLE;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_sdr_dbgen(service_urb_regs_t *urb, bool *dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((urb->sdr_dbgen & SERVICE_DBG_ALL_ENABLE) != 0) {
		*dbg_enable = true;
	} else {
		*dbg_enable = false;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_set_sp_dbgen(service_urb_regs_t *urb, bool dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if (dbg_enable == true) {
		urb->sp_dbgen = SERVICE_DBG_CPU_SP_ALL_ENABLE;
	} else {
		urb->sp_dbgen = SERVICE_DBG_CPU_SP_DISABLE;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_sp_dbgen(service_urb_regs_t *urb, bool *dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((urb->sp_dbgen & SERVICE_DBG_CPU_SP_ALL_ENABLE) != 0) {
		*dbg_enable = true;
	} else {
		*dbg_enable = false;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_set_s_dbgen(service_urb_regs_t *urb, bool dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if (dbg_enable == true) {
		urb->s_dbgen = SERVICE_DBG_CPU_S_ALL_ENABLE;
	} else {
		urb->s_dbgen = SERVICE_DBG_CPU_S_DISABLE;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_s_dbgen(service_urb_regs_t *urb, bool *dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((urb->s_dbgen & SERVICE_DBG_CPU_S_ALL_ENABLE) != 0) {
		*dbg_enable = true;
	} else {
		*dbg_enable = false;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_set_ust_dbgen(service_urb_regs_t *urb, bool dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if (dbg_enable == true) {
		urb->ust_dbgen = SERVICE_DBG_UST_ALL_ENABLE;
	} else {
		urb->ust_dbgen = SERVICE_DBG_UST_DISABLE;
	}
	return MCOM03_SUCCESS;
}

mcom_err_t service_get_ust_dbgen(service_urb_regs_t *urb, bool *dbg_enable)
{
	if (urb == NULL)
		return MCOM03_ERROR_NULL;

	if ((urb->ust_dbgen & SERVICE_DBG_UST_ALL_ENABLE) != 0) {
		*dbg_enable = true;
	} else {
		*dbg_enable = false;
	}
	return MCOM03_SUCCESS;
}
