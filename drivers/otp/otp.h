// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

#include <libs/platform-def-common.h>
#include <libs/utils-def.h>

typedef struct {
	uint32_t fuse1;
	uint32_t fuse0;
	union {
		uint32_t flags;
		struct {
			uint32_t reserved0 : 16;
			uint32_t force_sign : 1;
			uint32_t force_encrypt : 1;
			uint32_t reserved1 : 1;
			uint32_t disable_log : 1;
			uint32_t enable_watchdog : 1;
			uint32_t reserved2 : 11;
		} flags_bits;
	};
	uint32_t serial;
	uint8_t duk[AES_KEY_LEN];
	uint8_t rotpk[SHA_DIGEST_LEN];
	uint32_t reserved3;
	uint32_t crls[7];
	uint32_t reserved4;
	uint32_t crls_protection[7];
	uint32_t fuse1_redundant;
	uint32_t fuse0_redundant;
	uint8_t reserved5[52];
	uint8_t fw_lockable[68];
	uint8_t fw_nolock[256];
} otp_t;

otp_t *otp_get_dump(void);
void otp_clean_dump(void);
