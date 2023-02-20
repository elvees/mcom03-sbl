// SPDX-License-Identifier: MIT
// Copyright 2020-2021 RnD Center "ELVEES", JSC

#ifndef _BITOPS_H
#define _BITOPS_H

#define BIT(n)		(1U << (n))
#define GENMASK(hi, lo) (((1U << ((hi) - (lo) + 1U)) - 1U) << (lo))

#define __bf_shf(x) (__builtin_ffs(x) - 1)

#define FIELD_PREP(_mask, _val) ({ ((typeof(_mask))(_val) << __bf_shf(_mask)) & (_mask); })
#define FIELD_GET(_mask, _reg)	({ (typeof(_mask))(((_reg) & (_mask)) >> __bf_shf(_mask)); })

#endif /* _BITOPS_H */
