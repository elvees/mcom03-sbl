// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __UTILS_DEF_H__
#define __UTILS_DEF_H__

#define __bf_shf(x) (__builtin_ffsll(x) - 1)

/* Compute the number of elements in the given array */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define IS_POWER_OF_TWO(x) (((x) & ((x)-1)) == 0)

#define SIZE_FROM_LOG2_WORDS(n) (4 << (n))

#define BIT_32(nr) (U(1) << (nr))
#define BIT_64(nr) (ULL(1) << (nr))

#ifdef __aarch64__
#define BIT BIT_64
#else
#define BIT BIT_32
#endif

/*
 * Create a contiguous bitmask starting at bit position @l and ending at
 * position @h. For example
 * GENMASK_64(39, 21) gives us the 64bit vector 0x000000ffffe00000.
 */
#if defined(__LINKER__) || defined(__ASSEMBLER__)
#define GENMASK_32(h, l) (((0xFFFFFFFF) << (l)) & (0xFFFFFFFF >> (32 - 1 - (h))))

#define GENMASK_64(h, l) ((~0 << (l)) & (~0 >> (64 - 1 - (h))))
#else
#define GENMASK_32(h, l) (((~UINT32_C(0)) << (l)) & (~UINT32_C(0) >> (32 - 1 - (h))))

#define GENMASK_64(h, l) (((~UINT64_C(0)) << (l)) & (~UINT64_C(0) >> (64 - 1 - (h))))
#endif

#ifdef __aarch64__
#define GENMASK GENMASK_64
#else
#define GENMASK GENMASK_32
#endif

#define FIELD_PREP(_mask, _val) ({ ((__typeof__(_mask))(_val) << __bf_shf(_mask)) & (_mask); })
#define FIELD_GET(_mask, _reg)	({ (__typeof__(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask)); })

/*
 * This variant of div_round_up can be used in macro definition but should not
 * be used in C code as the `div` parameter is evaluated twice.
 */
#define DIV_ROUND_UP_2EVAL(n, d) (((n) + (d)-1) / (d))

#define div_round_up(val, div)                              \
	__extension__({                                     \
		__typeof__(div) _div = (div);               \
		((val) + _div - (__typeof__(div))1) / _div; \
	})

#define MIN(x, y)                       \
	__extension__({                 \
		__typeof__(x) _x = (x); \
		__typeof__(y) _y = (y); \
		(void)(&_x == &_y);     \
		_x < _y ? _x : _y;      \
	})

#define MAX(x, y)                       \
	__extension__({                 \
		__typeof__(x) _x = (x); \
		__typeof__(y) _y = (y); \
		(void)(&_x == &_y);     \
		_x > _y ? _x : _y;      \
	})

/*
 * The round_up() macro rounds up a value to the given boundary in a
 * type-agnostic yet type-safe manner. The boundary must be a power of two.
 * In other words, it computes the smallest multiple of boundary which is
 * greater than or equal to value.
 *
 * round_down() is similar but rounds the value down instead.
 */
#define round_boundary(value, boundary) ((__typeof__(value))((boundary)-1))

#define round_up(value, boundary) ((((value)-1) | round_boundary(value, boundary)) + 1)

#define round_down(value, boundary) ((value) & ~round_boundary(value, boundary))

/*
 * Evaluates to 1 if (ptr + inc) overflows, 0 otherwise.
 * Both arguments must be unsigned pointer values (i.e. uintptr_t).
 */
#define check_uptr_overflow(_ptr, _inc) ((_ptr) > (UINTPTR_MAX - (_inc)))

/*
 * Evaluates to 1 if (u32 + inc) overflows, 0 otherwise.
 * Both arguments must be 32-bit unsigned integers (i.e. effectively uint32_t).
 */
#define check_u32_overflow(_u32, _inc) ((_u32) > (UINT32_MAX - (_inc)))

/* Register size of the current architecture. */
#ifdef __aarch64__
#define REGSZ U(8)
#else
#define REGSZ U(4)
#endif

/*
 * Test for the current architecture version to be at least the version
 * expected.
 */
#define ARM_ARCH_AT_LEAST(_maj, _min) \
	((ARM_ARCH_MAJOR > (_maj)) || ((ARM_ARCH_MAJOR == (_maj)) && (ARM_ARCH_MINOR >= (_min))))

/*
 * Import an assembly or linker symbol as a C expression with the specified
 * type
 */
#define IMPORT_SYM(type, sym, name) \
	extern char sym[];          \
	static const __attribute__((unused)) type name = (type)sym;

/*
 * When the symbol is used to hold a pointer, its alignment can be asserted
 * with this macro. For example, if there is a linker symbol that is going to
 * be used as a 64-bit pointer, the value of the linker symbol must also be
 * aligned to 64 bit. This macro makes sure this is the case.
 */
#define ASSERT_SYM_PTR_ALIGN(sym) assert(((size_t)(sym) % __alignof__(*(sym))) == 0)

#define COMPILER_BARRIER() __asm__ volatile("" ::: "memory")

/* Compiler builtin of GCC >= 9 and planned in llvm */
#ifdef __HAVE_SPECULATION_SAFE_VALUE
#define SPECULATION_SAFE_VALUE(var) __builtin_speculation_safe_value(var)
#else
#define SPECULATION_SAFE_VALUE(var) var
#endif

#define COMPILE_TIME_ASSERT(expr)	COMPILE_TIME_ASSERT1(expr, __COUNTER__)
#define COMPILE_TIME_ASSERT1(expr, cnt) COMPILE_TIME_ASSERT2(expr, cnt)
#define COMPILE_TIME_ASSERT2(expr, cnt) \
	typedef char ct_assert_##cnt[(expr) ? 1 : -1] __attribute__((unused))

/** Align to the nearest lower address.
 *
 * @param s Address or size to be aligned.
 * @param a Size of alignment, must be a power of 2.
 */
#define ALIGN_DOWN(s, a) ((s) & ~((a)-1))

/** Align to the nearest higher address.
 *
 * @param s Address or size to be aligned.
 * @param a Size of alignment, must be a power of 2.
 */
#define ALIGN_UP(s, a) (((s) + ((a)-1)) & ~((a)-1))

/** Check alignment.
 *
 * @param s Address or size to be checked for alignment.
 * @param a Size of alignment, must be a power of 2.
 */
#define IS_ALIGNED(s, a) (ALIGN_UP((s), (a)) == (s))

/** Round up to the nearest higher boundary.
 *
 * @param n		Number to be aligned.
 * @param b		Boundary, arbitrary unsigned number.
 */
#define ROUND_UP(n, b) (((n) / (b) + ((n) % (b) != 0)) * (b))

#if defined(__ASSEMBLER__)
#define U(_x)	(_x)
#define UL(_x)	(_x)
#define ULL(_x) (_x)
#define L(_x)	(_x)
#define LL(_x)	(_x)
#else
#define U(_x)	(_x##U)
#define UL(_x)	(_x##UL)
#define ULL(_x) (_x##ULL)
#define L(_x)	(_x##L)
#define LL(_x)	(_x##LL)
#endif

#endif /* _UTILS_DEF_H_ */
