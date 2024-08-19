// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __CPU_H__
#define __CPU_H__

#define CPU_CORE_MAX_NUMBER 0x04

typedef struct {
	volatile unsigned int a53cpu_ppolicy;
	volatile unsigned int a53cpu_pstatus;
	volatile unsigned int reserv[2];
} cpu_core_t;

typedef struct {
	volatile unsigned int cpu_rvbaddrh;
	volatile unsigned int cpu_rvbaddrl;
} cpu_rvbaddr_t;

typedef struct {
	cpu_core_t cpu_core[4];
	volatile unsigned int a53sys_ppolicy;
	volatile unsigned int a53sys_pstatus;
	volatile unsigned int reserv4[2];
	volatile unsigned int pllcnfg;
	volatile unsigned int plldiag;
	volatile unsigned int reserv5[2];
	volatile unsigned int a53cpu_pcnfg;
	volatile unsigned int a53sys_pcnfg;
	volatile unsigned int reserv6[38];
	volatile unsigned int a53_rstcnfg;
	volatile unsigned int a53_acpcfg;
	volatile unsigned int gic500_status;
	volatile unsigned int cpu_eventimask;
	volatile unsigned int cpu_l2cnfg;
	volatile unsigned int cpu_startcfg;
	cpu_rvbaddr_t cpu_rvbaddr[4];
} cpu_urb_regs_t;

int cpu_set_clock(void);
int cpu_start_arm0_core(const unsigned long long int entrypoint);
int cpu_disable_arm_core(void);

#endif /* __CPU_H__ */
