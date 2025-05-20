// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <libs/utils-def.h>

#define BASE_ADDR_CPU_URB            0xa1000000
#define BASE_ADDR_CPU_UCG            0xa1080000
#define BASE_ADDR_CPU_GIC500         0xa1100000
#define BASE_ADDR_CPU_GIC500_REDISTR 0xa1180000

// CPU Subs UCG Channels
#define CPU_UCG_ALL_CH_MASK      GENMASK(2, 0)
#define CPU_UCG_SYNC_MASK        CPU_UCG_ALL_CH_MASK
#define CPU_UCG_CHANNEL_CLK_CORE 1
#define CPU_UCG_CHANNEL_CLK_DBUS 2
#define CPU_UCG_CHANNEL_CLK_SYS  0

#define CPU_CORE_MAX_NUMBER 0x04

typedef struct {
	volatile unsigned int a53cpu_ppolicy;
	volatile unsigned int a53cpu_pstatus;
	volatile unsigned int reserved0[2];
} cpu_core_t;

typedef struct {
	volatile unsigned int cpu_rvbaddrh;
	volatile unsigned int cpu_rvbaddrl;
} cpu_rvbaddr_t;

typedef struct {
	cpu_core_t cpu_core[4];
	volatile unsigned int a53sys_ppolicy;
	volatile unsigned int a53sys_pstatus;
	volatile unsigned int reserved1[2];
	volatile unsigned int pllcnfg;
	volatile unsigned int plldiag;
	volatile unsigned int reserved2[2];
	volatile unsigned int a53cpu_pcnfg;
	volatile unsigned int a53sys_pcnfg;
	volatile unsigned int reserved3[38];
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
