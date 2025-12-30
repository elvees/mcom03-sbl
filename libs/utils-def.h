// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

// Compute the number of elements in the given array
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define BIT(nr)   (U(1) << (nr))
#define BIT64(nr) (ULL(1) << (nr))

/**
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK(19, 9) gives us the 32bit vector 0x000ffe00.
 */
#define GENMASK(h, l)   (((~UINT32_C(0)) << (l)) & (~UINT32_C(0) >> (32 - 1 - (h))))
#define GENMASK64(h, l) (((~UINT64_C(0)) << (l)) & (~UINT64_C(0) >> (64 - 1 - (h))))

#define FIELD_PREP(_mask, _val) ({ ((__typeof__(_mask))(_val) << __bf_shf(_mask)) & (_mask); })
#define FIELD_GET(_mask, _reg)  ({ (__typeof__(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask)); })

#define COMPARE_TYPE(x, y) (void)(&(x) == &(y));

#define for_each_llist(node, head) \
	COMPARE_TYPE(node, head);  \
	for (node = head; node; node = node->next)

#define MIN(x, y)                       \
	__extension__({                 \
		__typeof__(x) _x = (x); \
		__typeof__(y) _y = (y); \
		COMPARE_TYPE(_x, _y);   \
		_x < _y ? _x : _y;      \
	})

#define MAX(x, y)                       \
	__extension__({                 \
		__typeof__(x) _x = (x); \
		__typeof__(y) _y = (y); \
		COMPARE_TYPE(_x, _y);   \
		_x > _y ? _x : _y;      \
	})

#define COMPILE_TIME_ASSERT(expr)       COMPILE_TIME_ASSERT1(expr, __COUNTER__)
#define COMPILE_TIME_ASSERT1(expr, cnt) COMPILE_TIME_ASSERT2(expr, cnt)
#define COMPILE_TIME_ASSERT2(expr, cnt) \
	typedef char ct_assert_##cnt[(expr) ? 1 : -1] __attribute__((unused))

/**
 * @brief Aligns to the nearest lower address
 *
 * @param s - Address or size to be aligned
 * @param a - Size of alignment, must be a power of 2
 */
#define ALIGN_DOWN(s, a) ((s) & ~((a) - 1))

/**
 * @brief Aligns to the nearest higher address
 *
 * @param s - Address or size to be aligned
 * @param a - Size of alignment, must be a power of 2
 */
#define ALIGN_UP(s, a) (((s) + ((a) - 1)) & ~((a) - 1))

/**
 * @brief Checks alignment
 *
 * @param s - Address or size to be checked for alignment
 * @param a - Size of alignment, must be a power of 2
 */
#define IS_ALIGNED(s, a) (ALIGN_UP((s), (a)) == (s))

#define U(_x)   (_x##U)
#define UL(_x)  (_x##UL)
#define ULL(_x) (_x##ULL)
#define L(_x)   (_x##L)
#define LL(_x)  (_x##LL)

#define USEC_IN_SEC  ULL(1000000)
#define MSEC_IN_SEC  ULL(1000)
#define USEC_IN_MSEC ULL(1000)

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A)    STRINGIZE_NX(A)
