// SPDX-License-Identifier: MIT
// Copyright 2023-2024 RnD Center "ELVEES", JSC

#ifndef __ERRORS_H__
#define __ERRORS_H__

#include <sys/errno.h>

// Enumerated type for error codes.

#define EINTERNAL           (__ELASTERROR + 1)
#define ENOTSUPPORTED       (__ELASTERROR + 2)
#define EINVALIDPARAM       (__ELASTERROR + 3)
#define EINVALIDSTATE       (__ELASTERROR + 4)
#define EINVALIDLENGTH      (__ELASTERROR + 5)
#define EINVALIDFLAGS       (__ELASTERROR + 6)
#define EINVALIDDATA        (__ELASTERROR + 7)
#define EDATASIZE           (__ELASTERROR + 8)
#define ETIMEOUT            (__ELASTERROR + 9)
#define EFORBIDDEN          (__ELASTERROR + 10)
#define ENULL               (__ELASTERROR + 11)
#define EINVALIDADDR        (__ELASTERROR + 12)
#define EALREADYINITIALIZED (__ELASTERROR + 13)
#define EUNKNOWN            (__ELASTERROR + 14)

#endif // __ERRORS_H__
