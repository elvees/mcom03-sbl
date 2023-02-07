// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef _UTILS_DEF_EXP_H
#define _UTILS_DEF_EXP_H

/*
 * For those constants to be shared between C and other sources, apply a 'U',
 * 'UL', 'ULL', 'L' or 'LL' suffix to the argument only in C, to avoid
 * undefined or unintended behaviour.
 *
 * The GNU assembler and linker do not support these suffixes (it causes the
 * build process to fail) therefore the suffix is omitted when used in linker
 * scripts and assembler files.
*/
#if defined(__ASSEMBLER__)
#define U(_x)	(_x)
#define UL(_x)	(_x)
#define ULL(_x) (_x)
#define L(_x)	(_x)
#define LL(_x)	(_x)
#else
#define U_(_x)	(_x##U)
#define U(_x)	U_(_x)
#define UL(_x)	(_x##UL)
#define ULL(_x) (_x##ULL)
#define L(_x)	(_x##L)
#define LL(_x)	(_x##LL)

#endif

#endif /* _UTILS_DEF_EXP_H */
