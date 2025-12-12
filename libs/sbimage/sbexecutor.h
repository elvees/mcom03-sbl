// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>

#include <drivers/otp/otp.h>
#include <libs/platform-def-common.h>
#include <third-party/crypto/bigint_impl.h>

#define SBIMG_HEADER_MAGIC 0x53424D47
#define HEADER_SIZE        96

#define AES_BLOCK_LEN 16
#define RSA_MOD_LEN   384

#define MODULE_SIZE  (x509_cert->rsa_ctx->m->size * COMP_BYTE_SIZE)
#define PUB_EXP_SIZE (x509_cert->rsa_ctx->e->size * COMP_BYTE_SIZE)

#define ALIGN(var, len)            ((var) + (((len) - ((var) % (len))) % (len)))
#define COMPLETE_BLOCK_LENGTH(var) ALIGN((var), AES_BLOCK_LEN)

#define SBIMAGE_TYPE_PAYLOAD_NO_RETURN    0
#define SBIMAGE_TYPE_ENCRYPTION_KEY       1
#define SBIMAGE_TYPE_ROOT_CERTIFICATE     2
#define SBIMAGE_TYPE_NON_ROOT_CERTIFICATE 3
#define SBIMAGE_TYPE_PAYLOAD_WITH_RETURN  4
#define SBIMAGE_TYPE_PAYLOAD_NO_EXEC      5

typedef void *(*memcopy_t)(void *, uintptr_t, size_t);
typedef int (*chck_laddr_t)(uintptr_t, uint32_t);
typedef int (*chck_eaddr_t)(uintptr_t, uint32_t, uintptr_t);
typedef int (*chck_img_t)(const void *, size_t);
typedef int (*read_img_t)(void *, signed long, size_t);

typedef struct {
	chck_laddr_t chck_laddr_func;
	chck_eaddr_t chck_eaddr_func;
	chck_img_t chck_img;
	memcopy_t cpy_func;
	read_img_t read_img_func;
	uintptr_t image_offset;
	otp_t *otp;
} sb_mem_t;

int sblimg_init(sb_mem_t *sb_ctx);
int sblimg_update(void);
void __dead2 sblimg_finish(int status);
void sblimg_abort(void);
int sblimg_check(void);
