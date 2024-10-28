// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libs/errors.h>
#include <libs/log.h>
#include <third-party/aes/aes.h>
#include <third-party/crypto/bigint.h>
#include <third-party/crypto/bigint_impl.h>
#include <third-party/crypto/config.h>
#include <third-party/crypto/crypto.h>
#include <third-party/crypto/crypto_misc.h>

#include "sbexecutor.h"
#include "sbimage.h"
#include "sbstatus-print.h"
#include "status.h"

#define CHECK_NULL(ptr)                \
	do {                           \
		if ((ptr) == NULL) {   \
			return -ENULL; \
		}                      \
	} while (0)

#define CHECK_OK(error, cond)             \
	do {                              \
		if ((cond)) {             \
			status = (error); \
			goto end;         \
		}                         \
	} while (0)

#define CHECK_HEADER(header)                                               \
	do {                                                               \
		if (!(header)->flags_bits.skip_header_hash) {              \
			if (check_header((header))) {                      \
				status = ESBIMGBOOT_IMAGE_BAD_HEADER_HASH; \
				goto end;                                  \
			}                                                  \
		}                                                          \
	} while (0)

#define EXECUTE(addr)                                       \
	do {                                                \
		next_img_t next_image = (next_img_t)(addr); \
		next_image();                               \
	} while (0)

#define SET_CERT_NUMBER(sign_cert_id, num)                                \
	do {                                                              \
		for (; (sign_cert_id) != sign_cert_arr[(num)]; (num)++) { \
			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_TOO_MUCH_CERTS, \
			         (num) >= CONFIG_X509_MAX_CA_CERTS);      \
		}                                                         \
	} while (0);
;

typedef const volatile int (*next_img_t)(void);

static const volatile void *(*memset_s)(void *, int,
                                        size_t) = (const volatile void *(*)(void *, int,
                                                                            size_t))memset;

static const uint8_t sig_prefix_sha256[] = { 0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60,
	                                     0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02,
	                                     0x01, 0x05, 0x00, 0x04, 0x20 };
static const uint8_t iv[AES_BLOCK_LEN] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	                                   0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
static uint8_t encrypted_key[AES_KEY_LEN];
static uint32_t key_number = 0;
static X509_CTX *x509_root = NULL;

static uint32_t sign_cert_arr[CONFIG_X509_MAX_CA_CERTS] = { 0 };
static X509_CTX *non_root_cert[CONFIG_X509_MAX_CA_CERTS] = { NULL };
static volatile uint8_t cert_index = 0;
static volatile uint8_t end_cert_has_been_handled = 0;

static sb_mem_t sb_mem = { 0 };

/**
 * Take a signature, decrypt and verify it.
 *
 * 1 - error, 0 - OK, -1 - malloc error
 */
static int sig_verify(BI_CTX *ctx, const uint8_t *sig, int sig_len, bigint *modulus,
                      bigint *pub_exp, bigint **bir)
{
	int i;
	bigint *decrypted_bi, *dat_bi;
	int res = 1;
	uint8_t *block = (uint8_t *)malloc(sig_len);
	if (block == NULL) {
		res = -1;
		return res;
	}

	uint8_t sig_prefix_size = sizeof(sig_prefix_sha256);
	uint8_t hash_len = sig_prefix_sha256[sig_prefix_size - 1];

	/* check length (#A) */
	if (sig_len < 2 + 8 + 1 + sig_prefix_size + hash_len)
		goto err;

	/* decrypt */
	dat_bi = bi_import(ctx, sig, sig_len);
	if (dat_bi == NULL) {
		res = -1;
		goto err;
	}
	ctx->mod_offset = BIGINT_M_OFFSET;

	/* convert to a normal block */
	decrypted_bi = bi_mod_power2(ctx, dat_bi, modulus, pub_exp);
	if (decrypted_bi == NULL) {
		res = -1;
		goto err;
	}

	bi_export(ctx, decrypted_bi, block, sig_len);
	ctx->mod_offset = BIGINT_M_OFFSET;

	/* check the first 2 bytes */
	if (block[0] != 0 || block[1] != 1)
		goto err;

	/* check the padding */
	i = 2; /* start at the first padding byte */
	/* together with (#A), we require at least 8 bytes of padding */
	while (i < sig_len - 1 - sig_prefix_size - hash_len)
		if (block[i++] != 0xFF)
			goto err;

	/* check end of padding */
	if (block[i++] != 0)
		goto err;

	/* check the ASN.1 metadata */
	if (memcmp(block + i, sig_prefix_sha256, sig_prefix_size))
		goto err;

	/* now we can get the hash we need */
	*bir = bi_import(ctx, block + i + sig_prefix_size, hash_len);
	if (*bir == NULL) {
		res = -1;
		goto err;
	}
	res = 0;
err:
	free(block);
	/* save a few bytes of memory */
	bi_clear_cache(ctx);
	return res;
}

// Perform certificate verification checks
static int sbimg_x509_cert_verify(const X509_CTX *ca_cert, const X509_CTX *cert,
                                  int *pathLenConstraint)
{
	int ret = X509_OK;
	bigint *cert_sig;
	X509_CTX *next_cert = NULL;
	BI_CTX *ctx = NULL;
	bigint *mod = NULL, *expn = NULL;
	int match_ca_cert = 0;
	uint8_t is_self_signed = 0;

	if (cert == NULL) {
		ret = X509_VFY_ERROR_NO_TRUSTED_CERT;
		goto end_verify;
	}

	/* a self-signed certificate that is not in the CA store - use this
	   to check the signature */
	if (asn1_compare_dn(cert->ca_cert_dn, cert->cert_dn) == 0) {
		is_self_signed = 1;
		ctx = cert->rsa_ctx->bi_ctx;
		mod = cert->rsa_ctx->m;
		expn = cert->rsa_ctx->e;
	}

	if (cert->basic_constraint_present) {
		/* If the cA boolean is not asserted,
		   then the keyCertSign bit in the key usage extension MUST NOT be
		   asserted. */
		if (!cert->basic_constraint_cA &&
		    IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN)) {
			ret = X509_VFY_ERROR_BASIC_CONSTRAINT;
			goto end_verify;
		}

		/* The pathLenConstraint field is meaningful only if the cA boolean is
		   asserted and the key usage extension, if present, asserts the
		   keyCertSign bit. In this case, it gives the maximum number of
		   non-self-issued intermediate certificates that may follow this
		   certificate in a valid certification path. */
		if (cert->basic_constraint_cA &&
		    (!cert->key_usage_present ||
		     IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN)) &&
		    (cert->basic_constraint_pathLenConstraint + 1) < *pathLenConstraint) {
			ret = X509_VFY_ERROR_BASIC_CONSTRAINT;
			goto end_verify;
		}
	}

	next_cert = cert->next;

	/* last cert in the chain - look for a trusted cert */
	if (next_cert == NULL) {
		if (ca_cert != NULL) {
			/* the extension is present but the cA boolean is not
			   asserted, then the certified public key MUST NOT be used
			   to verify certificate signatures. */
			if (cert->basic_constraint_present && !ca_cert->basic_constraint_cA) {
				ret = X509_VFY_ERROR_NO_TRUSTED_CERT;
				goto end_verify;
			}

			if (asn1_compare_dn(cert->ca_cert_dn, ca_cert->cert_dn) == 0) {
				/* use this CA certificate for signature verification */
				match_ca_cert = true;
				ctx = ca_cert->rsa_ctx->bi_ctx;
				mod = ca_cert->rsa_ctx->m;
				expn = ca_cert->rsa_ctx->e;
			}
		}

		/* couldn't find a trusted cert (& let self-signed errors
		   be returned) */
		if (!match_ca_cert && !is_self_signed) {
			ret = X509_VFY_ERROR_NO_TRUSTED_CERT;
			goto end_verify;
		}
	} else if (asn1_compare_dn(cert->ca_cert_dn, next_cert->cert_dn) != 0) {
		/* check the chain */
		ret = X509_VFY_ERROR_INVALID_CHAIN;
		goto end_verify;
	} else /* use the next certificate in the chain for signature verify */
	{
		ctx = next_cert->rsa_ctx->bi_ctx;
		mod = next_cert->rsa_ctx->m;
		expn = next_cert->rsa_ctx->e;
	}

	/* cert is self signed */
	if (!match_ca_cert && is_self_signed) {
		ret = X509_VFY_ERROR_SELF_SIGNED;
		goto end_verify;
	}

	/* check the signature */
	bigint *mod_clone = bi_clone(ctx, mod);
	if (mod_clone == NULL)
		return X509_MALLOC_ERROR;
	bigint *expn_clone = bi_clone(ctx, expn);
	if (expn_clone == NULL)
		return X509_MALLOC_ERROR;

	int sig_ver_res =
		sig_verify(ctx, cert->signature, cert->sig_len, mod_clone, expn_clone, &cert_sig);

	if ((sig_ver_res == 0) && cert_sig && cert->digest) {
		if (bi_compare(cert_sig, cert->digest) != 0)
			ret = X509_VFY_ERROR_BAD_SIGNATURE;

		bi_free(ctx, cert_sig);
	} else if (sig_ver_res == -1) {
		ret = X509_MALLOC_ERROR;
	} else {
		ret = X509_VFY_ERROR_BAD_SIGNATURE;
	}

end_verify:
	return ret;
}

static int signature_verify_hash(const uint8_t *sha256_dgst, const uint8_t *signature,
                                 int signature_size, RSA_CTX *rsa_ctx)
{
	bigint *payload_digest = 0;
	int verify_result = 0;

	payload_digest = bi_import(rsa_ctx->bi_ctx, sha256_dgst, SHA256_SIZE);
	if (payload_digest == NULL)
		return -1;

	BI_CTX *ctx = rsa_ctx->bi_ctx;
	bigint *mod = rsa_ctx->m;
	bigint *expn = rsa_ctx->e;
	bigint *cert_sig = NULL;
	bigint *mod_clone = bi_clone(ctx, mod);
	if (mod_clone == NULL)
		return -1;
	bigint *expn_clone = bi_clone(ctx, expn);
	if (expn_clone == NULL)
		return -1;
	int sig_ver_res =
		sig_verify(ctx, signature, signature_size, mod_clone, expn_clone, &cert_sig);

	if ((sig_ver_res == 0) && (cert_sig != NULL) && payload_digest) {
		int cmp_status = (bi_compare(cert_sig, payload_digest) != 0);

		bi_free(ctx, cert_sig);

		if (cmp_status)
			verify_result = 1;
	} else if (sig_ver_res == -1) {
		verify_result = -1;
	} else {
		verify_result = 1;
	}
	bi_free(ctx, payload_digest);
	bi_clear_cache(ctx);
	return verify_result;
}

/* Return value: 0 - signature is good, otherwise - 1, '-1' - malloc error */
static int signature_verify(const uint8_t *data, const uint8_t *signature, int data_size,
                            int signature_size, RSA_CTX *rsa_ctx)
{
	uint8_t sha256_dgst[SHA256_SIZE];
	SHA256_CTX sha256_ctx;

	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, data, data_size);
	SHA256_Final(sha256_dgst, &sha256_ctx);

	return signature_verify_hash(sha256_dgst, signature, signature_size, rsa_ctx);
}

static int check_digest(const uint8_t *data, size_t size, const uint8_t *etalon)
{
	CHECK_NULL(data);
	CHECK_NULL(etalon);

	uint8_t digest[SHA_DIGEST_LEN];
	SHA256_CTX sha256_ctx;

	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, data, size);
	SHA256_Final(digest, &sha256_ctx);

	return (memcmp((const void *)digest, (const void *)etalon, sizeof(digest)));
}

static int check_header(const sbimghdr_t *sbimg)
{
	CHECK_NULL(sbimg);

	sbimghdr_t _sbimg;
	sb_mem.cpy_func((void *)&_sbimg, (size_t)sbimg, HEADER_SIZE);
	memset_s((void *)_sbimg.h_dgst, 0, SHA_DIGEST_LEN);
	return check_digest((uint8_t *)(&_sbimg), HEADER_SIZE, (uint8_t *)sbimg->h_dgst);
}

static int derrived_key(uint8_t *cek, size_t size)
{
	CHECK_NULL(cek);

	int status = 0;

	CHECK_OK(-EDATASIZE, size > AES_KEY_LEN);

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

	uint8_t k1[AES_KEY_LEN];
	AES_ECB_encrypt_wrap(prekey1, sb_mem.otp.duk, k1, AES_KEY_LEN);
	AES_ECB_encrypt_wrap(prekey2, k1, kek, AES_KEY_LEN);
	memset_s(k1, 0, AES_KEY_LEN);

	AES_CBC_decrypt_buffer_wrap(cek, encrypted_key, AES_KEY_LEN, kek, iv);
	memset_s(kek, 0, AES_KEY_LEN);

end:
	return status;
}

static int decrypt(uint8_t *data, int size)
{
	CHECK_NULL(data);

	int status = 0;

	uint8_t cek[AES_KEY_LEN];
	CHECK_OK(-EDATASIZE, derrived_key(cek, AES_KEY_LEN));
	AES_CBC_decrypt_buffer_wrap(data, data, size, cek, iv);
	memset_s(cek, 0, AES_KEY_LEN);

end:
	return status;
}

static int verify(const uint8_t *data, const uint8_t *signature, size_t data_size, int index)
{
	CHECK_NULL(data);
	CHECK_NULL(signature);
	CHECK_NULL(non_root_cert[index]->rsa_ctx);

	int status = 0;

	CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR, (index < 0) || (index > cert_index));

	CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_SIGNATURE,
	         signature_verify(data, signature, data_size, RSA_MOD_LEN,
	                          non_root_cert[index]->rsa_ctx));

end:
	return status;
}

static int image_handle(const sbimghdr_t *sbimg, const uint8_t *signature)
{
	CHECK_NULL(sbimg);

	int status = 0;
	size_t size;

	enum act { verification, decipher, check, nope };
	enum act chain[] = { nope, nope, nope };

	chain[0] = (sbimg->flags_bits.encrypted) ? decipher : nope;
	chain[1] = (sbimg->flags_bits.checksum) ? check : nope;
	chain[2] = (sbimg->flags_bits.signed_obj) ? verification : nope;

	if (sbimg->flags_bits.sign_of_encrypted) {
		chain[0] = verification;
		chain[2] = decipher;
	}

	size_t data_size = (sbimg->flags_bits.encrypted) ? COMPLETE_BLOCK_LENGTH(sbimg->pl_size) :
	                                                   sbimg->pl_size;
	size_t signature_size = (sbimg->flags_bits.signed_obj) ? RSA_MOD_LEN : 0;
	sb_mem.cpy_func((void *)sbimg->l_addr,
	                (uintptr_t)sb_mem.buffer + HEADER_SIZE + signature_size, data_size);

	for (size_t i = 0; i < sizeof(chain) / sizeof(chain[0]); i++) {
		switch (chain[i]) {
		case verification:
			CHECK_OK(-ENULL, signature == NULL);

			size = (sbimg->flags_bits.sign_of_encrypted) ?
			               COMPLETE_BLOCK_LENGTH(sbimg->pl_size) :
			               sbimg->pl_size;

			CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_SIGNATURE,
			         verify((uint8_t *)sbimg->l_addr, (uint8_t *)signature, size,
			                cert_index));
			break;

		case decipher:
			CHECK_OK(-EINVALIDDATA, decrypt((uint8_t *)sbimg->l_addr,
			                                COMPLETE_BLOCK_LENGTH(sbimg->pl_size)));
			break;

		case check:
			size = (sbimg->flags_bits.sign_of_encrypted) ?
			               COMPLETE_BLOCK_LENGTH(sbimg->pl_size) :
			               sbimg->pl_size;
			CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_HASH,
			         check_digest((uint8_t *)sbimg->l_addr, size,
			                      (uint8_t *)sbimg->pl_dgst));
			break;

		default:
			break;
		}
	}

end:
	if (status)
		memset_s((uint8_t *)sbimg->l_addr, 0, data_size);

	return status;
}

static void crypto_free(void)
{
	for (int i = 0; i < CONFIG_X509_MAX_CA_CERTS; i++)
		if (non_root_cert[i]) {
			non_root_cert[i]->next = NULL;
			x509_free(non_root_cert[i]);
			non_root_cert[i] = NULL;
		}

	if (x509_root) {
		x509_free(x509_root);
		x509_root = NULL;
	}

	cert_index = 0;
	end_cert_has_been_handled = 0;

	memset_s(sign_cert_arr, 0, sizeof(sign_cert_arr));
}

int static check_signed_load_requirement(sbimghdr_t *header)
{
	int status = 0;

	if (sb_mem.otp.flags_bits.force_sign)
		CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_SIGNED, !header->flags_bits.signed_obj);

end:
	return status;
}

int static check_encrypted_load_requirement(sbimghdr_t *header)
{
	int status = 0;

	if (sb_mem.otp.flags_bits.force_encrypt)
		CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_ENCRYPTED, !header->flags_bits.encrypted);

end:
	return status;
}

int sblimg_init(sb_mem_t *sb_ctx)
{
	sb_mem.chck_laddr_func = sb_ctx->chck_laddr_func;
	sb_mem.chck_eaddr_func = sb_ctx->chck_eaddr_func;
	sb_mem.cpy_func = sb_ctx->cpy_func;
	sb_mem.read_image = sb_ctx->read_image;
	sb_mem.image_offset = sb_ctx->image_offset;
	sb_mem.buffer = sb_ctx->buffer;
	sb_mem.cpy_func(&sb_mem.otp, (uintptr_t)&sb_ctx->otp, sizeof(otp_t));

	return ESBIMGBOOT_NO_ERR;
}

int sblimg_check(void)
{
	int status = 0;
	sbimghdr_t sbimg;

	while (1) {
		CHECK_OK(-EINTERNAL,
		         sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));
		CHECK_OK(-EINVALIDSTATE, sbimg.h_id != SBIMG_HEADER_MAGIC);

		size_t data_size = sbimg.pl_size;
		size_t cipher_size = COMPLETE_BLOCK_LENGTH(data_size);
		size_t pl_size = (sbimg.flags_bits.encrypted) ? cipher_size : data_size;
		size_t sign_size = (sbimg.flags_bits.signed_obj) ? RSA_MOD_LEN : 0;
		size_t image_size = HEADER_SIZE + sign_size + data_size;
		image_size = (sbimg.flags_bits.encrypted) ? COMPLETE_BLOCK_LENGTH(image_size) :
		                                            ALLIGN(image_size, 4);

		CHECK_OK(-EINTERNAL,
		         sb_mem.read_image(sb_mem.buffer, sb_mem.image_offset, image_size));

		const size_t data = sb_mem.buffer + HEADER_SIZE + sign_size;
		const size_t signature = sb_mem.buffer + HEADER_SIZE;

		// TODO: Check that offset doesn't go beyond sbimg size border
		sb_mem.image_offset += image_size;

		CHECK_HEADER(&sbimg);

		int path_len = 0;
		switch (sbimg.flags_bits.obj_type) {
		case SBIMAGE_TYPE_ROOT_CERTIFICATE:
			CHECK_OK(ESBIMGBOOT_ROOT_CERT_IS_NOT_FIRST, cert_index != 0);

			CHECK_OK(ESBIMGBOOT_MALLOC_ERR,
			         x509_new((uint8_t *)data, (int *)&pl_size, &x509_root));
			CHECK_OK(ESBIMGBOOT_ROOT_CERT_X509_ERR,
			         sbimg_x509_cert_verify(x509_root, x509_root, &path_len));
			CHECK_OK(-ENULL, x509_root == NULL);

			CHECK_OK(ESBIMGBOOT_ROOT_CERT_BAD_HASH,
			         memcmp((const void *)sb_mem.otp.rot,
			                (const void *)x509_root->sha256_digest, SHA_DIGEST_LEN));
			break;

		case SBIMAGE_TYPE_NON_ROOT_CERTIFICATE:
			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR, end_cert_has_been_handled);

			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_TOO_MUCH_CERTS,
			         cert_index >= CONFIG_X509_MAX_CA_CERTS);

			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_IS_FIRST, x509_root == NULL);

			for (int i = 0; i < cert_index; i++)
				CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
				         sign_cert_arr[i] == sbimg.cert_id);

			CHECK_OK(ESBIMGBOOT_MALLOC_ERR, x509_new((uint8_t *)data, (int *)&pl_size,
			                                         &non_root_cert[cert_index]));

			if (!non_root_cert[cert_index]->basic_constraint_cA)
				end_cert_has_been_handled = 1;

			if (!asn1_compare_dn(x509_root->cert_dn,
			                     non_root_cert[cert_index]->ca_cert_dn))
				non_root_cert[cert_index]->next = x509_root;

			if (!non_root_cert[cert_index]->next)
				for (int i = cert_index - 1; i >= 0; i--)
					if (!asn1_compare_dn(
						    non_root_cert[i]->cert_dn,
						    non_root_cert[cert_index]->ca_cert_dn)) {
						non_root_cert[cert_index]->next = non_root_cert[i];
						break;
					}

			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
			         non_root_cert[cert_index]->next == NULL);

			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
			         sbimg_x509_cert_verify(non_root_cert[cert_index]->next,
			                                non_root_cert[cert_index], &path_len));
			sign_cert_arr[cert_index] = sbimg.cert_id;

			if (!end_cert_has_been_handled)
				cert_index++;
			break;

		case SBIMAGE_TYPE_ENCRYPTION_KEY:
			CHECK_OK(ESBIMGBOOT_ENC_KEY_BAD_CERT_CHAIN, !end_cert_has_been_handled);

			CHECK_OK(ESBIMGBOOT_ENC_KEY_IS_NOT_SIGNED,
			         check_signed_load_requirement(&sbimg));

			int num = 0;
			SET_CERT_NUMBER(sbimg.sign_cert_id, num);
			CHECK_OK(ESBIMGBOOT_ENC_KEY_BAD_SIGNATURE,
			         verify((uint8_t *)data, (uint8_t *)signature, data_size, num));

			sb_mem.cpy_func((void *)encrypted_key, (size_t)data, cipher_size);
			key_number = sbimg.aes_key_num;
			break;

		case SBIMAGE_TYPE_PAYLOAD_NO_EXEC:
		case SBIMAGE_TYPE_PAYLOAD_NO_RETURN:
		case SBIMAGE_TYPE_PAYLOAD_WITH_RETURN:
			CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_CERT_CHAIN, !end_cert_has_been_handled);

			CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_SIGNED,
			         check_signed_load_requirement(&sbimg));
			CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_ENCRYPTED,
			         check_encrypted_load_requirement(&sbimg));

			if (!(sbimg.flags_bits.encrypted && !sbimg.flags_bits.sign_of_encrypted)) {
				if (sbimg.flags_bits.signed_obj) {
					CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_SIGNATURE,
					         verify((uint8_t *)data, (uint8_t *)signature,
					                data_size, cert_index));
				} else {
					CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_HASH,
					         check_digest((uint8_t *)data, pl_size,
					                      sbimg.pl_dgst));
				}
			} else {
				uint8_t digest[SHA256_SIZE];
				SHA256_CTX sha256_ctx;

				uint8_t buf[AES_BLOCKLEN];

				struct AES_ctx aes_ctx;

				uint8_t key[AES_KEY_LEN];
				CHECK_OK(-EDATASIZE, derrived_key(key, AES_KEY_LEN));
				AES_init_ctx_iv(&aes_ctx, key, iv);
				memset_s(key, 0, AES_KEY_LEN);

				SHA256_Init(&sha256_ctx);
				size_t cipher_text = data;
				size_t size = data_size - (data_size % AES_BLOCKLEN);
				while (size > AES_BLOCKLEN) {
					sb_mem.cpy_func((void *)buf, cipher_text, AES_BLOCKLEN);
					AES_CBC_decrypt_buffer(&aes_ctx, buf, AES_BLOCKLEN);
					SHA256_Update(&sha256_ctx, buf, AES_BLOCKLEN);
					cipher_text += AES_BLOCKLEN;
					size -= AES_BLOCKLEN;
				}
				sb_mem.cpy_func((void *)buf, cipher_text, size);
				AES_CBC_decrypt_buffer(&aes_ctx, buf, size);
				memset_s(&aes_ctx, 0, sizeof(struct AES_ctx));
				SHA256_Update(&sha256_ctx, buf, size);

				memset_s(buf, 0, AES_BLOCKLEN);

				SHA256_Final(digest, &sha256_ctx);
				CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_SIGNATURE,
				         signature_verify_hash((uint8_t *)digest,
				                               (uint8_t *)signature, RSA_MOD_LEN,
				                               non_root_cert[cert_index]->rsa_ctx));
			}

			CHECK_OK(0, sbimg.flags_bits.obj_type == SBIMAGE_TYPE_PAYLOAD_NO_RETURN);
			break;

		default:
			break;
		}
	}

end:
	crypto_free();

	return status;
}

int sblimg_update(void)
{
	int status = ESBIMGBOOT_NO_ERR;

	sbimghdr_t sbimg;

	uint8_t *data = NULL;
	uint8_t *signature = NULL;

	CHECK_OK(-EINTERNAL,
	         sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));

	CHECK_OK(ESBIMGBOOT_IMAGE_BAD_HEADER_ID, sbimg.h_id != SBIMG_HEADER_MAGIC);

	size_t data_size = sbimg.pl_size;
	size_t cipher_size = COMPLETE_BLOCK_LENGTH(sbimg.pl_size);
	size_t pl_size = (sbimg.flags_bits.encrypted) ? cipher_size : data_size;
	size_t sign_size = (sbimg.flags_bits.signed_obj) ? RSA_MOD_LEN : 0;

	size_t image_size = HEADER_SIZE + sign_size + data_size;
	image_size = (sbimg.flags_bits.encrypted) ? COMPLETE_BLOCK_LENGTH(image_size) :
	                                            ALLIGN(image_size, 4);

	CHECK_OK(-EINTERNAL, sb_mem.read_image(sb_mem.buffer, sb_mem.image_offset, image_size));

	if (sbimg.flags_bits.obj_type == SBIMAGE_TYPE_ROOT_CERTIFICATE ||
	    sbimg.flags_bits.obj_type == SBIMAGE_TYPE_NON_ROOT_CERTIFICATE ||
	    sbimg.flags_bits.obj_type == SBIMAGE_TYPE_ENCRYPTION_KEY)
		data = (uint8_t *)(sb_mem.buffer + HEADER_SIZE + sign_size);

	if (sign_size)
		signature = (uint8_t *)(sb_mem.buffer + HEADER_SIZE);

	CHECK_HEADER(&sbimg);

	int path_len = 0;
	switch (sbimg.flags_bits.obj_type) {
	case SBIMAGE_TYPE_ROOT_CERTIFICATE:
		CHECK_OK(ESBIMGBOOT_ROOT_CERT_IS_NOT_FIRST, cert_index != 0);

		CHECK_OK(ESBIMGBOOT_MALLOC_ERR, x509_new(data, (int *)&pl_size, &x509_root));
		CHECK_OK(ESBIMGBOOT_ROOT_CERT_X509_ERR,
		         sbimg_x509_cert_verify(x509_root, x509_root, &path_len));
		CHECK_NULL(x509_root);

		CHECK_OK(ESBIMGBOOT_ROOT_CERT_BAD_HASH,
		         memcmp((const void *)sb_mem.otp.rot,
		                (const void *)x509_root->sha256_digest, SHA_DIGEST_LEN));
		break;

	case SBIMAGE_TYPE_NON_ROOT_CERTIFICATE:
		CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR, end_cert_has_been_handled);

		CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_TOO_MUCH_CERTS,
		         cert_index >= CONFIG_X509_MAX_CA_CERTS);

		for (int i = 0; i < cert_index; i++)
			CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
			         sign_cert_arr[i] == sbimg.cert_id);

		CHECK_OK(ESBIMGBOOT_MALLOC_ERR,
		         x509_new(data, (int *)&pl_size, &non_root_cert[cert_index]));

		if (!non_root_cert[cert_index]->basic_constraint_cA)
			end_cert_has_been_handled = 1;

		if (!asn1_compare_dn(x509_root->cert_dn, non_root_cert[cert_index]->ca_cert_dn))
			non_root_cert[cert_index]->next = x509_root;

		if (!non_root_cert[cert_index]->next)
			for (int i = cert_index - 1; i >= 0; i--)
				if (!asn1_compare_dn(non_root_cert[i]->cert_dn,
				                     non_root_cert[cert_index]->ca_cert_dn)) {
					non_root_cert[cert_index]->next = non_root_cert[i];
					break;
				}

		CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
		         non_root_cert[cert_index]->next == NULL);

		CHECK_OK(ESBIMGBOOT_NON_ROOT_CERT_X509_ERR,
		         sbimg_x509_cert_verify(non_root_cert[cert_index]->next,
		                                non_root_cert[cert_index], &path_len));
		sign_cert_arr[cert_index] = sbimg.cert_id;

		if (!end_cert_has_been_handled)
			cert_index++;
		break;

	case SBIMAGE_TYPE_ENCRYPTION_KEY:
		CHECK_OK(ESBIMGBOOT_ENC_KEY_BAD_CERT_CHAIN, !end_cert_has_been_handled);

		CHECK_OK(ESBIMGBOOT_ENC_KEY_IS_NOT_SIGNED, check_signed_load_requirement(&sbimg));

		int num = 0;
		SET_CERT_NUMBER(sbimg.sign_cert_id, num);
		CHECK_OK(ESBIMGBOOT_ENC_KEY_BAD_SIGNATURE,
		         verify((uint8_t *)data, (uint8_t *)signature, data_size, num));

		sb_mem.cpy_func((void *)encrypted_key, (size_t)data, cipher_size);
		key_number = sbimg.aes_key_num;
		break;

	case SBIMAGE_TYPE_PAYLOAD_NO_EXEC:
	case SBIMAGE_TYPE_PAYLOAD_NO_RETURN:
	case SBIMAGE_TYPE_PAYLOAD_WITH_RETURN:
		CHECK_OK(ESBIMGBOOT_PAYLOAD_BAD_CERT_CHAIN, !end_cert_has_been_handled);

		CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_SIGNED, check_signed_load_requirement(&sbimg));
		CHECK_OK(ESBIMGBOOT_PAYLOAD_IS_NOT_ENCRYPTED,
		         check_encrypted_load_requirement(&sbimg));

		CHECK_OK(-EINVALIDDATA, sb_mem.chck_laddr_func(sbimg.l_addr, sbimg.pl_size));
		CHECK_OK(-EINVALIDDATA, image_handle(&sbimg, signature));
		CHECK_OK(ESBIMGBOOT_LOAD_FINISH,
		         sbimg.flags_bits.obj_type == SBIMAGE_TYPE_PAYLOAD_NO_RETURN);

		if (sbimg.flags_bits.obj_type == SBIMAGE_TYPE_PAYLOAD_WITH_RETURN) {
			CHECK_OK(-EINVALIDDATA,
			         sb_mem.chck_eaddr_func(sbimg.l_addr, sbimg.pl_size, sbimg.e_addr));
			EXECUTE(sbimg.e_addr);
		}
		break;

	default:
		break;
	}

	sb_mem.image_offset += image_size;

end:
	return status;
}

void sblimg_abort(void)
{
	crypto_free();
}

void __dead2 sblimg_finish(int status)
{
	crypto_free();

	if (status == ESBIMGBOOT_LOAD_FINISH) {
		sbimghdr_t sbimg;

		CHECK_OK(-EINTERNAL,
		         sb_mem.read_image((uintptr_t)&sbimg, sb_mem.image_offset, HEADER_SIZE));
		CHECK_OK(-EINVALIDDATA,
		         sb_mem.chck_eaddr_func(sbimg.l_addr, sbimg.pl_size, sbimg.e_addr));
		EXECUTE(sbimg.e_addr);
	}

end:
	sblimg_print_return_code(status);

	panic_handler("We should not be here!!!\n");
}
