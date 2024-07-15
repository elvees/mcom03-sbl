// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __URB_CPU_SUBS_H__
#define __URB_CPU_SUBS_H__

#define CPU_SUBS_PPOLICY_PP_ON	     0x10
#define CPU_SUBS_PPOLICY_PP_WARM_RST 0x08
#define CPU_SUBS_PPOLICY_PP_OFF	     0x01

#define CPU_CORE_MAX_NUMBER 0x04

typedef struct {
	volatile unsigned int a53cpu_ppolicy; // Регистр установки состояния ядра CPU (RW)
	volatile unsigned int a53cpu_pstatus; // Регистр статуса состояния ядра CPU (RO)
	volatile unsigned int reserv[2];
} cpu_core_t;

typedef struct {
	volatile unsigned int cpu_rvbaddrh; // Регистр вектора сброса CPU старшая часть (RW)
	volatile unsigned int cpu_rvbaddrl; // Регистр вектора сброса CPU младшая часть (RW)
} cpu_rvbaddr_t;

typedef struct {
	cpu_core_t cpu_core[4];
	volatile unsigned int a53sys_ppolicy; // Регистр установки состояния кластерной части CPU (RW)
	volatile unsigned int a53sys_pstatus; // Регистр статуса состояния кластерной части CPU (RO)
	volatile unsigned int reserv4[2];
	volatile unsigned int cpu_subs_pllcnfg; // Регистр управления PLL подсистемы (RW)
	volatile unsigned int cpu_subs_plldiag; // Регистр диагностики PLL подсистемы (RW)
	volatile unsigned int reserv5[2];
	volatile unsigned int
		a53cpu_pcnfg; // Регистр конфигурации процедур включения и выключения доменов cpu (RW)
	volatile unsigned int
		a53sys_pcnfg; // Регистр конфигурации процедур включения и выключения домена sys (RW)
	volatile unsigned int reserv6[38];
	volatile unsigned int a53_rstcnfg; // Регистр конфигурации сброса CPU (RW)
	volatile unsigned int a53_acpcfg; // Регистр конфигурации ACP порта CPU (RW)
	volatile unsigned int gic500_status; // Регистр статуса GIC500 (RO)
	volatile unsigned int cpu_eventimask; // Регист маски события для CPU (RW)
	volatile unsigned int cpu_l2cnfg; // Регистр конфигурации L2 кэш CPU (RW)
	volatile unsigned int cpu_startcfg; // Регистр конфигурации старта CPU (RW)
	cpu_rvbaddr_t cpu_rvbaddr[4];
} cpu_subs_urb_regs_t;

cpu_subs_urb_regs_t *getCPUSubsURBRegisters();

#endif /* __URB_CPU_SUBS_H__ */
