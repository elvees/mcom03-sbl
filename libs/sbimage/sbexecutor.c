// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <string.h>
#include <stdlib.h>

#include <mcom03.h>
#include <mcom03_errors.h>
#include <log.h>
#include <platform_helpers.h>

#include <lib/crypto/x509.h>
#include <lib/crypto/sha256.h>
#include <lib/aes_ccm/aes128.h>

#include "sbexecutor.h"

#define CHECK_NULL(ptr)                           \
	do {                                      \
		if ((ptr) == NULL) {              \
			return MCOM03_ERROR_NULL; \
		}                                 \
	} while (0)

#define CHECK_OK(error, cond)          \
	do {                           \
		if ((cond)) {          \
			ret = (error); \
			goto end;      \
		}                      \
	} while (0)

#define CHECK_HEADER(header)                                                         \
	do {                                                                         \
		if (!(header)->flagsBits.skipHeaderHash) {                           \
			if (check_header((header))) {                                \
				ret = MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_HEADER_HASH; \
				goto end;                                            \
			}                                                            \
		}                                                                    \
	} while (0)

#define EXECUTE(addr)                                       \
	do {                                                \
		next_img_t next_image = (next_img_t)(addr); \
		next_image();                               \
	} while (0)

#define SET_CERT_NUMBER(signCertId, num)                                               \
	do {                                                                           \
		for (; (signCertId) != sign_cert_id[(num)]; (num)++) {                 \
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_TOO_MUCH_CERTS, \
				 (num) >= CONFIG_X509_MAX_CERTS);                      \
		}                                                                      \
	} while (0);
;

typedef const volatile int (*next_img_t)(void);

static const volatile void *(*memset_s)(void *, int,
					size_t) = (const volatile void *(*)(void *, int,
									    size_t))memset;

static const uint8_t iv[AES_BLOCK_LEN] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
					   0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
static uint8_t encrypted_key[AES_KEY_LEN];
static uint32_t key_number = 0;
static X509_CTX *x509_root = NULL;
static uint8_t fw_counter = 0;

static uint32_t sign_cert_id[CONFIG_X509_MAX_CERTS] = { 0 };
static X509_CTX *non_root_cert[CONFIG_X509_MAX_CERTS] = { NULL };
static volatile uint8_t cert_index = 0;
static volatile uint8_t end_cert_has_been_handled = 0;

static sb_mem_t sb_mem = { 0 };

static int check_digest(const uint8_t *data, size_t size, const uint8_t *etalon)
{
	CHECK_NULL(data);
	CHECK_NULL(etalon);

	uint8_t digest[SHA_DIGEST_LEN];
	SHA256_Calculate(data, size, digest);

	return (memcmp((const void *)digest, (const void *)etalon, sizeof(digest)));
}

static int check_header(const sbimghdr_t *sbimg)
{
	CHECK_NULL(sbimg);

	sbimghdr_t _sbimg;
	sb_mem.cpy_func((void *)&_sbimg, (size_t)sbimg, HEADER_SIZE);
	memset_s((void *)_sbimg.hDgst, 0, SHA_DIGEST_LEN);
	return check_digest((uint8_t *)(&_sbimg), HEADER_SIZE, (uint8_t *)sbimg->hDgst);
}

static int derrived_key(uint8_t *cek, size_t size)
{
	CHECK_NULL(cek);

	int ret = MCOM03_SUCCESS;

	CHECK_OK(MCOM03_ERROR_DATA_SIZE, size > AES_KEY_LEN);

	uint8_t prekey1[AES_BLOCK_LEN] = { 0x80,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x01,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   (uint8_t)(key_number >> 8),
					   (uint8_t)key_number };

	uint8_t prekey2[AES_BLOCK_LEN] = { sb_mem.otp.sn[0],
					   sb_mem.otp.sn[1],
					   sb_mem.otp.sn[2],
					   sb_mem.otp.sn[3],
					   0x00,
					   0x01,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   0x00,
					   (uint8_t)(key_number >> 8),
					   (uint8_t)key_number,
					   0x00,
					   0x00 };
	uint8_t kek[AES_KEY_LEN];
	{
		uint8_t k1[AES_KEY_LEN];
		AES_ECB_encrypt_wrap(prekey1, sb_mem.otp.duk, k1, AES_KEY_LEN);
		AES_ECB_encrypt_wrap(prekey2, k1, kek, AES_KEY_LEN);
		memset_s(k1, 0, AES_KEY_LEN);
	}
	AES_CBC_decrypt_buffer_wrap(cek, encrypted_key, AES_KEY_LEN, kek, iv);
	memset_s(kek, 0, AES_KEY_LEN);

end:
	return ret;
}

static int decrypt(uint8_t *data, int size)
{
	CHECK_NULL(data);

	int ret = MCOM03_SUCCESS;

	uint8_t cek[AES_KEY_LEN];
	CHECK_OK(MCOM03_ERROR_DATA_SIZE, derrived_key(cek, AES_KEY_LEN));
	AES_CBC_decrypt_buffer_wrap(data, data, size, cek, iv);
	memset_s(cek, 0, AES_KEY_LEN);

end:
	return ret;
}

static int verify(const uint8_t *data, const uint8_t *signature, size_t data_size, int index)
{
	CHECK_NULL(data);
	CHECK_NULL(signature);
	CHECK_NULL(non_root_cert[index]->rsa_ctx);

	int ret = MCOM03_SUCCESS;

	CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
		 (index < 0) || (index > cert_index));

	CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_SIGNATURE,
		 signature_verify(data, signature, data_size, RSA_MOD_LEN,
				  non_root_cert[index]->rsa_ctx));

end:
	return ret;
}

static int image_handle(const sbimghdr_t *sbimg, const uint8_t *signature)
{
	CHECK_NULL(sbimg);

	int ret = MCOM03_SUCCESS;

	enum act { verification, decipher, check, nope };
	enum act chain[] = { nope, nope, nope };

	chain[0] = (sbimg->flagsBits.encrypted) ? decipher : nope;
	chain[1] = (sbimg->flagsBits.checksum) ? check : nope;
	chain[2] = (sbimg->flagsBits.signedObj) ? verification : nope;

	if (sbimg->flagsBits.signOfEncrypted) {
		chain[0] = verification;
		chain[2] = decipher;
	}

	size_t data_size = (sbimg->flagsBits.encrypted) ? COMPLETE_BLOCK_LENGTH(sbimg->plSize) :
							  sbimg->plSize;
	size_t signature_size = (sbimg->flagsBits.signedObj) ? RSA_MOD_LEN : 0;
	sb_mem.cpy_func((void *)sbimg->lAddr,
			(uintptr_t)sb_mem.buffer + HEADER_SIZE + signature_size, data_size);

	for (size_t i = 0; i < sizeof(chain) / sizeof(chain[0]); i++) {
		switch (chain[i]) {
		case verification: {
			CHECK_OK(MCOM03_ERROR_NULL, signature == NULL);

			size_t size = (sbimg->flagsBits.signOfEncrypted) ?
					      COMPLETE_BLOCK_LENGTH(sbimg->plSize) :
					      sbimg->plSize;

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_SIGNATURE,
				 verify((uint8_t *)sbimg->lAddr, (uint8_t *)signature, size,
					cert_index));
		} break;

		case decipher: {
			CHECK_OK(MCOM03_ERROR_INVALID_DATA,
				 decrypt((uint8_t *)sbimg->lAddr,
					 COMPLETE_BLOCK_LENGTH(sbimg->plSize)));
		} break;

		case check: {
			size_t size = (sbimg->flagsBits.signOfEncrypted) ?
					      COMPLETE_BLOCK_LENGTH(sbimg->plSize) :
					      sbimg->plSize;
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_HASH,
				 check_digest((uint8_t *)sbimg->lAddr, size,
					      (uint8_t *)sbimg->plDgst));
		} break;

		default:
			break;
		}
	}

end:
	if (ret)
		memset_s((uint8_t *)sbimg->lAddr, 0, data_size);

	return ret;
}

static void crypto_free(void)
{
	for (int i = 0; i < CONFIG_X509_MAX_CERTS; i++) {
		if (non_root_cert[i] != NULL) {
			non_root_cert[i]->next = NULL;
			x509_free(non_root_cert[i]);
			non_root_cert[i] = NULL;
		}
	}
	if (x509_root != NULL) {
		x509_free(x509_root);
		x509_root = NULL;
	}

	cert_index = 0;
	end_cert_has_been_handled = 0;

	memset_s(sign_cert_id, 0, sizeof(sign_cert_id));
}

int static check_signed_load_requirement(sbimghdr_t *header)
{
	int ret = MCOM03_SUCCESS;

	if (sb_mem.otp.flagsBits.force_sign) {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_SIGNED,
			 !header->flagsBits.signedObj);
	}

end:
	return ret;
}

int static check_encrypted_load_requirement(sbimghdr_t *header)
{
	int ret = MCOM03_SUCCESS;

	if (sb_mem.otp.flagsBits.force_encrypt) {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_ENCRYPTED,
			 !header->flagsBits.encrypted);
	}

end:
	return ret;
}

int sblimg_init(sb_mem_t *sb_ctx)
{
	sb_mem.set_curr_fw_func = sb_ctx->set_curr_fw_func;
	sb_mem.chck_laddr_func = sb_ctx->chck_laddr_func;
	sb_mem.chck_eaddr_func = sb_ctx->chck_eaddr_func;
	sb_mem.chck_fw_func = sb_ctx->chck_fw_func;
	sb_mem.cpy_func = sb_ctx->cpy_func;
	sb_mem.read_image = sb_ctx->read_image;
	sb_mem.image_offset = sb_ctx->image_offset;
	sb_mem.buffer = sb_ctx->buffer;
	sb_mem.cpy_func(&sb_mem.otp, (uintptr_t)&sb_ctx->otp, sizeof(otp_t));

	return MCOM03_STATUS_SBIMG_BOOT_NO_ERR;
}

int sblimg_check(void)
{
	int ret = MCOM03_SUCCESS;
	sbimghdr_t sbimg;

	while (1) {
		CHECK_OK(MCOM03_ERROR_INTERNAL,
			 sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));
		CHECK_OK(MCOM03_ERROR_INVALID_STATE, sbimg.hId != SBIMG_HEADER_MAGIC);

		size_t data_size = sbimg.plSize;
		size_t cipher_size = COMPLETE_BLOCK_LENGTH(data_size);
		size_t pl_size = (sbimg.flagsBits.encrypted) ? cipher_size : data_size;
		size_t sign_size = (sbimg.flagsBits.signedObj) ? RSA_MOD_LEN : 0;
		size_t image_size = HEADER_SIZE + sign_size + data_size;
		image_size = (sbimg.flagsBits.encrypted) ? COMPLETE_BLOCK_LENGTH(image_size) :
							   ALLIGN(image_size, 4);

		CHECK_OK(MCOM03_ERROR_INTERNAL,
			 sb_mem.read_image(sb_mem.buffer, sb_mem.image_offset, image_size));

		const size_t data = sb_mem.buffer + HEADER_SIZE + sign_size;
		const size_t signature = sb_mem.buffer + HEADER_SIZE;

		//TODO: Check that offset doesn't go beyond sbimg size border
		sb_mem.image_offset += image_size;

		CHECK_HEADER(&sbimg);

		int path_len = 0;
		switch (sbimg.flagsBits.objType) {
		case SBIMAGE_TYPE_ROOT_CERTIFICATE: {
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_IS_NOT_FIRST, cert_index != 0);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_MALLOC_ERROR,
				 x509_new((uint8_t *)data, (int *)&pl_size, &x509_root));
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_X509_ERROR,
				 x509_verify_cert(x509_root, x509_root, &path_len));
			CHECK_OK(MCOM03_ERROR_NULL, x509_root == NULL);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_BAD_HASH,
				 memcmp((const void *)sb_mem.otp.rot,
					(const void *)x509_root->sha256_digest, SHA_DIGEST_LEN));
		} break;

		case SBIMAGE_TYPE_NON_ROOT_CERTIFICATE: {
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
				 end_cert_has_been_handled);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_TOO_MUCH_CERTS,
				 cert_index >= CONFIG_X509_MAX_CERTS);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_IS_FIRST, x509_root == NULL);

			for (int i = 0; i < cert_index; i++)
				CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
					 sign_cert_id[i] == sbimg.certId);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_MALLOC_ERROR,
				 x509_new((uint8_t *)data, (int *)&pl_size,
					  &non_root_cert[cert_index]));

			if (non_root_cert[cert_index]->basic_constraint_cA == 0) {
				CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_FW_COUNTER,
					 sb_mem.chck_fw_func((uint8_t)non_root_cert[cert_index]
								     ->trusted_fw_counter));
				end_cert_has_been_handled = 1;
			}

			if (asn1_compare_dn(x509_root->cert_dn,
					    non_root_cert[cert_index]->ca_cert_dn) == 0)
				non_root_cert[cert_index]->next = x509_root;

			if (non_root_cert[cert_index]->next == NULL) {
				for (int i = cert_index - 1; i >= 0; i--) {
					if (asn1_compare_dn(
						    non_root_cert[i]->cert_dn,
						    non_root_cert[cert_index]->ca_cert_dn) == 0) {
						non_root_cert[cert_index]->next = non_root_cert[i];
						break;
					}
				}
			}

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
				 non_root_cert[cert_index]->next == NULL);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
				 x509_verify_cert(non_root_cert[cert_index]->next,
						  non_root_cert[cert_index], &path_len));
			sign_cert_id[cert_index] = sbimg.certId;

			if (!end_cert_has_been_handled)
				cert_index++;
		} break;

		case SBIMAGE_TYPE_ENCRYPTION_KEY: {
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_CERT_CHAIN,
				 !end_cert_has_been_handled);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_IS_NOT_SIGNED,
				 check_signed_load_requirement(&sbimg));

			int num = 0;
			SET_CERT_NUMBER(sbimg.signCertId, num);
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_SIGNATURE,
				 verify((uint8_t *)data, (uint8_t *)signature, data_size, num));

			sb_mem.cpy_func((void *)encrypted_key, (size_t)data, cipher_size);
			key_number = sbimg.aesKeyNum;
		} break;

		case SBIMAGE_TYPE_PAYLOAD_NO_EXEC:
		case SBIMAGE_TYPE_PAYLOAD_NO_RETURN:
		case SBIMAGE_TYPE_PAYLOAD_WITH_RETURN: {
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_CERT_CHAIN,
				 !end_cert_has_been_handled);

			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_SIGNED,
				 check_signed_load_requirement(&sbimg));
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_ENCRYPTED,
				 check_encrypted_load_requirement(&sbimg));

			if (!(sbimg.flagsBits.encrypted && !sbimg.flagsBits.signOfEncrypted)) {
				if (sbimg.flagsBits.signedObj) {
					CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_SIGNATURE,
						 verify((uint8_t *)data, (uint8_t *)signature,
							data_size, cert_index));
				} else {
					CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_HASH,
						 check_digest((uint8_t *)data, pl_size,
							      sbimg.plDgst));
				}
			} else {
				uint8_t digest[SHA256_SIZE];
				SHA256_CTX sha256_ctx;
				{
					uint8_t buf[AES_BLOCKLEN];
					{
						struct AES_ctx aes_ctx;
						{
							uint8_t key[AES_KEY_LEN];
							CHECK_OK(MCOM03_ERROR_DATA_SIZE,
								 derrived_key(key, AES_KEY_LEN));
							AES_init_ctx_iv(&aes_ctx, key, iv);
							memset_s(key, 0, AES_KEY_LEN);
						}
						SHA256_Init(&sha256_ctx);
						size_t cipher_text = data;
						size_t size =
							data_size - (data_size % AES_BLOCKLEN);
						while (size > AES_BLOCKLEN) {
							sb_mem.cpy_func((void *)buf, cipher_text,
									AES_BLOCKLEN);
							AES_CBC_decrypt_buffer(&aes_ctx, buf,
									       AES_BLOCKLEN);
							SHA256_Update(&sha256_ctx, buf,
								      AES_BLOCKLEN);
							cipher_text += AES_BLOCKLEN;
							size -= AES_BLOCKLEN;
						}
						sb_mem.cpy_func((void *)buf, cipher_text, size);
						AES_CBC_decrypt_buffer(&aes_ctx, buf, size);
						memset_s(&aes_ctx, 0, sizeof(struct AES_ctx));
						SHA256_Update(&sha256_ctx, buf, size);
					}
					memset_s(buf, 0, AES_BLOCKLEN);
				}
				SHA256_Final(digest, &sha256_ctx);
				CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_SIGNATURE,
					 signature_verify_hash((uint8_t *)digest,
							       (uint8_t *)signature, RSA_MOD_LEN,
							       non_root_cert[cert_index]->rsa_ctx));
			}

			CHECK_OK(MCOM03_SUCCESS,
				 sbimg.flagsBits.objType == SBIMAGE_TYPE_PAYLOAD_NO_RETURN);
		} break;

		default:
			break;
		}
	}

end:
	crypto_free();

	return ret;
}

int sblimg_update(void)
{
	int ret = MCOM03_STATUS_SBIMG_BOOT_NO_ERR;

	sbimghdr_t sbimg;

	uint8_t *data = NULL;
	uint8_t *signature = NULL;

	CHECK_OK(MCOM03_ERROR_INTERNAL,
		 sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));

	CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_HEADER_ID, sbimg.hId != SBIMG_HEADER_MAGIC);

	size_t data_size = sbimg.plSize;
	size_t cipher_size = COMPLETE_BLOCK_LENGTH(sbimg.plSize);
	size_t pl_size = (sbimg.flagsBits.encrypted) ? cipher_size : data_size;
	size_t sign_size = (sbimg.flagsBits.signedObj) ? RSA_MOD_LEN : 0;

	size_t image_size = HEADER_SIZE + sign_size + data_size;
	image_size = (sbimg.flagsBits.encrypted) ? COMPLETE_BLOCK_LENGTH(image_size) :
						   ALLIGN(image_size, 4);

	CHECK_OK(MCOM03_ERROR_INTERNAL,
		 sb_mem.read_image(sb_mem.buffer, sb_mem.image_offset, image_size));

	if (sbimg.flagsBits.objType == SBIMAGE_TYPE_ROOT_CERTIFICATE ||
	    sbimg.flagsBits.objType == SBIMAGE_TYPE_NON_ROOT_CERTIFICATE ||
	    sbimg.flagsBits.objType == SBIMAGE_TYPE_ENCRYPTION_KEY) {
		data = (uint8_t *)(sb_mem.buffer + HEADER_SIZE + sign_size);
	}

	if (sign_size) {
		signature = (uint8_t *)(sb_mem.buffer + HEADER_SIZE);
	}

	CHECK_HEADER(&sbimg);

	int path_len = 0;
	switch (sbimg.flagsBits.objType) {
	case SBIMAGE_TYPE_ROOT_CERTIFICATE: {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_IS_NOT_FIRST, cert_index != 0);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_MALLOC_ERROR,
			 x509_new(data, (int *)&pl_size, &x509_root));
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_X509_ERROR,
			 x509_verify_cert(x509_root, x509_root, &path_len));
		CHECK_OK(MCOM03_ERROR_NULL, x509_root == NULL);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_BAD_HASH,
			 memcmp((const void *)sb_mem.otp.rot,
				(const void *)x509_root->sha256_digest, SHA_DIGEST_LEN));
	} break;

	case SBIMAGE_TYPE_NON_ROOT_CERTIFICATE: {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
			 end_cert_has_been_handled);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_TOO_MUCH_CERTS,
			 cert_index >= CONFIG_X509_MAX_CERTS);

		for (int i = 0; i < cert_index; i++)
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
				 sign_cert_id[i] == sbimg.certId);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_MALLOC_ERROR,
			 x509_new(data, (int *)&pl_size, &non_root_cert[cert_index]));

		if (non_root_cert[cert_index]->basic_constraint_cA == 0) {
			CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_FW_COUNTER,
				 sb_mem.chck_fw_func(
					 (uint8_t)non_root_cert[cert_index]->trusted_fw_counter));
			fw_counter = (uint8_t)non_root_cert[cert_index]->trusted_fw_counter;
			end_cert_has_been_handled = 1;
		}

		if (asn1_compare_dn(x509_root->cert_dn, non_root_cert[cert_index]->ca_cert_dn) == 0)
			non_root_cert[cert_index]->next = x509_root;

		if (non_root_cert[cert_index]->next == NULL) {
			for (int i = cert_index - 1; i >= 0; i--) {
				if (asn1_compare_dn(non_root_cert[i]->cert_dn,
						    non_root_cert[cert_index]->ca_cert_dn) == 0) {
					non_root_cert[cert_index]->next = non_root_cert[i];
					break;
				}
			}
		}

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
			 non_root_cert[cert_index]->next == NULL);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR,
			 x509_verify_cert(non_root_cert[cert_index]->next,
					  non_root_cert[cert_index], &path_len));
		sign_cert_id[cert_index] = sbimg.certId;

		if (!end_cert_has_been_handled)
			cert_index++;
	} break;

	case SBIMAGE_TYPE_ENCRYPTION_KEY: {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_CERT_CHAIN,
			 !end_cert_has_been_handled);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_IS_NOT_SIGNED,
			 check_signed_load_requirement(&sbimg));

		int num = 0;
		SET_CERT_NUMBER(sbimg.signCertId, num);
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_SIGNATURE,
			 verify((uint8_t *)data, (uint8_t *)signature, data_size, num));

		sb_mem.cpy_func((void *)encrypted_key, (size_t)data, cipher_size);
		key_number = sbimg.aesKeyNum;
	} break;

	case SBIMAGE_TYPE_PAYLOAD_NO_EXEC:
	case SBIMAGE_TYPE_PAYLOAD_NO_RETURN:
	case SBIMAGE_TYPE_PAYLOAD_WITH_RETURN: {
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_CERT_CHAIN,
			 !end_cert_has_been_handled);

		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_SIGNED,
			 check_signed_load_requirement(&sbimg));
		CHECK_OK(MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_ENCRYPTED,
			 check_encrypted_load_requirement(&sbimg));

		CHECK_OK(MCOM03_ERROR_INVALID_DATA,
			 sb_mem.chck_laddr_func(sbimg.lAddr, sbimg.plSize));
		CHECK_OK(MCOM03_ERROR_INVALID_DATA, image_handle(&sbimg, signature));
		CHECK_OK(MCOM03_STATUS_SBIMG_BOOT_LOAD_FINISH,
			 sbimg.flagsBits.objType == SBIMAGE_TYPE_PAYLOAD_NO_RETURN);

		if (sbimg.flagsBits.objType == SBIMAGE_TYPE_PAYLOAD_WITH_RETURN) {
			CHECK_OK(MCOM03_ERROR_INVALID_DATA,
				 sb_mem.chck_eaddr_func(sbimg.lAddr, sbimg.plSize, sbimg.eAddr));
			EXECUTE(sbimg.eAddr);
		}
	} break;

	default:
		break;
	}

	sb_mem.image_offset += image_size;

end:
	return ret;
}

void sblimg_abort(void)
{
	crypto_free();
}

void __dead2 sblimg_finish(int status)
{
	crypto_free();

	int ret = status;

	if (ret == MCOM03_STATUS_SBIMG_BOOT_LOAD_FINISH) {
		sbimghdr_t sbimg;
		CHECK_OK(MCOM03_ERROR_INTERNAL,
			 sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));

		CHECK_OK(MCOM03_ERROR_INVALID_DATA,
			 sb_mem.chck_eaddr_func(sbimg.lAddr, sbimg.plSize, sbimg.eAddr));
		CHECK_OK(MCOM03_ERROR_INTERNAL, sb_mem.set_curr_fw_func(fw_counter));
		EXECUTE(sbimg.eAddr);
	}

end:
	sblimg_print_status(ret);

	plat_panic_handler("We should not be here!!!\r\n");
}
