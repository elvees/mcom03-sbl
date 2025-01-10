// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#include <stdio.h>

#include <drivers/mcom03-regs.h>
#include <libs/asm.h>
#include <libs/helpers/helpers.h>
#include <libs/mc.h>

#define EXC_MASK        0x7C
#define EXC_MASK_OFFSET 0x2

struct eframe {
	unsigned long r[32];
};

void dump_stack(const struct eframe *f)
{
	printf("\tat 0x%lx\r\n", f->r[AT]);
	printf("\tv0 0x%lx\r\n", f->r[v0]);
	printf("\tv1 0x%lx\r\n", f->r[v1]);
	printf("\ta0 0x%lx\r\n", f->r[a0]);
	printf("\ta1 0x%lx\r\n", f->r[a1]);
	printf("\ta2 0x%lx\r\n", f->r[a2]);
	printf("\ta3 0x%lx\r\n", f->r[a3]);
	printf("\tt0 0x%lx\r\n", f->r[t0]);
	printf("\tt1 0x%lx\r\n", f->r[t1]);
	printf("\tt2 0x%lx\r\n", f->r[t2]);
	printf("\tt3 0x%lx\r\n", f->r[t3]);
	printf("\tt4 0x%lx\r\n", f->r[t4]);
	printf("\tt5 0x%lx\r\n", f->r[t5]);
	printf("\tt6 0x%lx\r\n", f->r[t6]);
	printf("\tt7 0x%lx\r\n", f->r[t7]);
	printf("\ts0 0x%lx\r\n", f->r[s0]);
	printf("\ts1 0x%lx\r\n", f->r[s1]);
	printf("\ts2 0x%lx\r\n", f->r[s2]);
	printf("\ts3 0x%lx\r\n", f->r[s3]);
	printf("\ts4 0x%lx\r\n", f->r[s4]);
	printf("\ts5 0x%lx\r\n", f->r[s5]);
	printf("\ts6 0x%lx\r\n", f->r[s6]);
	printf("\ts7 0x%lx\r\n", f->r[s7]);
	printf("\tt8 0x%lx\r\n", f->r[t8]);
	printf("\tt9 0x%lx\r\n", f->r[t9]);
	printf("\tk0 0x%lx\r\n", f->r[k0]);
	printf("\tk1 0x%lx\r\n", f->r[k1]);
	printf("\tgp 0x%lx\r\n", f->r[gp]);
	printf("\ts8 0x%lx\r\n", f->r[s8]);
	printf("\tfp 0x%lx\r\n", f->r[fp]);
	printf("\tra 0x%lx\r\n", f->r[ra]);
}

void __dead2 exception_handler(struct eframe *regs)
{
	const char *exc_name;
	unsigned int cp0_cause = mips_read_cp0_register(CP0_CAUSE); // get CP0 cause
	unsigned int exc_code = (cp0_cause & EXC_MASK) >> EXC_MASK_OFFSET;
	switch (exc_code) {
	case EXC_INT:
		exc_name = "Int";
		break;
	case EXC_MOD:
		exc_name = "Mod";
		break;
	case EXC_TLBL:
		exc_name = "TLBL";
		break;
	case EXC_TLBS:
		exc_name = "TLBS";
		break;
	case EXC_ADEL:
		exc_name = "AdEL";
		break;
	case EXC_ADES:
		exc_name = "AdES";
		break;
	case EXC_SYS:
		exc_name = "Sys";
		break;
	case EXC_BP:
		exc_name = "Bp";
		break;
	case EXC_RI:
		exc_name = "RI";
		break;
	case EXC_CPU:
		exc_name = "CpU";
		break;
	case EXC_OV:
		exc_name = "Ov";
		break;
	case EXC_TR:
		exc_name = "Tr";
		break;
	default:
		exc_name = "Unknown";
		break;
	}
	printf("\r\n**************************************************\r\n");
	printf("EXCEPTION \"%s\".\r\n", exc_name);
	if ((exc_code == EXC_ADEL) || (exc_code == EXC_ADES))
		printf("BADVADDR : 0x%08x\r\n", mips_read_cp0_register(CP0_BADVADDR));
	else
		printf("BADVADDR : \"Unknown\"\r\n");
	printf("EPC : 0x%08x\r\n", mips_read_cp0_register(CP0_EPC));
	printf("Registers:\r\n");
	dump_stack(regs);
	printf("**************************************************\r\n");
	while (1)
		; // Unreachable
}
