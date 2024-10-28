// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __SBPARSING_H__
#define __SBPARSING_H__

#include <sys/cdefs.h>
#include "sbstatus_print.h"
#include "sbimage.h"

#define SBIMG_HEADER_MAGIC (0x53424d47)
#define HEADER_SIZE	   (96)

#define AES_KEY_LEN    (16)
#define AES_BLOCK_LEN  (16)
#define SHA_DIGEST_LEN (32)
#define RSA_MOD_LEN    (384)

#define MODULE_SIZE  (x509_cert->rsa_ctx->m->size * COMP_BYTE_SIZE)
#define PUB_EXP_SIZE (x509_cert->rsa_ctx->e->size * COMP_BYTE_SIZE)

#define ALLIGN(var, len)	   ((var) + (((len) - ((var) % (len))) % (len)))
#define COMPLETE_BLOCK_LENGTH(var) ALLIGN((var), AES_BLOCK_LEN)

#define SBIMAGE_TYPE_PAYLOAD_NO_RETURN	  (0)
#define SBIMAGE_TYPE_ENCRYPTION_KEY	  (1)
#define SBIMAGE_TYPE_ROOT_CERTIFICATE	  (2)
#define SBIMAGE_TYPE_NON_ROOT_CERTIFICATE (3)
#define SBIMAGE_TYPE_PAYLOAD_WITH_RETURN  (4)
#define SBIMAGE_TYPE_PAYLOAD_NO_EXEC	  (5)

typedef void *(*memcopy_t)(void *, uintptr_t, size_t);
typedef int (*chck_laddr_t)(unsigned long, unsigned int);
typedef int (*chck_eaddr_t)(unsigned long, unsigned int, unsigned long);
typedef int (*chck_fw_t)(uint8_t);
typedef uint32_t (*set_curr_fw_t)(uint8_t);
typedef int (*read_image_t)(uintptr_t, uintptr_t, size_t);

typedef struct {
	uint16_t force_sign : 1;
	uint16_t force_encrypt : 1;
	uint16_t reserved_0 : 1;
	uint16_t disable_log : 1;
	uint16_t enable_watchdog : 1;
	uint16_t reserved_1 : 11;
} otp_flags;

typedef struct __packed {
	uint16_t zero_count;
	union {
		uint16_t otp_flags;
		otp_flags flagsBits;
	};
	uint8_t sn[4];
	uint8_t duk[AES_KEY_LEN];
	uint8_t rot[SHA_DIGEST_LEN];
	uint8_t crl[32];
	uint8_t crt_protection[32];
	size_t risc0fw_cnt;
} otp_t;

typedef struct {
	chck_laddr_t chck_laddr_func;
	chck_eaddr_t chck_eaddr_func;
	chck_fw_t chck_fw_func;
	set_curr_fw_t set_curr_fw_func;
	memcopy_t cpy_func;
	read_image_t read_image;
	uintptr_t image_offset;
	uintptr_t buffer;
	otp_t otp;
} sb_mem_t;

int sblimg_init(sb_mem_t *sb_ctx);
int sblimg_update(void);
void __dead2 sblimg_finish(int status);
void sblimg_abort(void);
int sblimg_check(void);

#endif /* __SBPARSING_H__ */
