// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <string.h>

#include <libs/errors.h>
#include <libs/log.h>

#include "status.h"

static inline int sblimg_print_error(int status)
{
	switch (status) {
	case EINTERNAL:
		ERROR("%s\n", "Internal error");
		break;

	case ENOTSUPPORTED:
		ERROR("%s\n", "Not supported");
		break;

	case EINVALIDPARAM:
		ERROR("%s\n", "Invalid parameter");
		break;

	case EINVALIDSTATE:
		ERROR("%s\n", "Invalid state, operation disallowed in this state");
		break;

	case EINVALIDLENGTH:
		ERROR("%s\n", "Invalid length");
		break;

	case EINVALIDFLAGS:
		ERROR("%s\n", "Invalid Flags");
		break;

	case EINVALIDDATA:
		ERROR("%s\n", "Invalid Data");
		break;

	case EDATASIZE:
		ERROR("%s\n", "Data size exceeds limit");
		break;

	case ETIMEOUT:
		ERROR("%s\n", "Operation timed out");
		break;

	case EFORBIDDEN:
		ERROR("%s\n", "Operation is forbidden");
		break;

	case ENULL:
		ERROR("%s\n", "Null pointer");
		break;

	case EINVALIDADDR:
		ERROR("%s\n", "Bad memory address");
		break;

	case EALREADYINITIALIZED:
		ERROR("%s\n", "Module already initialized");
		break;

	case EUNKNOWN:
		ERROR("%s\n", "Unknown error");
		break;

	default:
		ERROR("%s\n", "Unknown status");
		break;
	}
	return 0;
}

static inline int sblimg_print_status(int status)
{
	switch (status) {
	case ESBIMGBOOT_NO_ERR:
		NOTICE("%s\n", "No Error");
		break;

	case ESBIMGBOOT_LOAD_FINISH:
		NOTICE("%s\n", "Load finished");
		break;

	case ESBIMGBOOT_LOAD_CONTINUE:
		NOTICE("%s\n", "load continue");
		break;

	case ESBIMGBOOT_IMAGE_BAD_HEADER_ID:
		ERROR("%s\n", "Incorrect header id");
		break;

	case ESBIMGBOOT_IMAGE_BAD_HEADER_HASH:
		ERROR("%s\n", "Incorrect header hash");
		break;

	case ESBIMGBOOT_IMAGE_BAD_TYPE:
		ERROR("%s\n", "Incorrect type");
		break;

	case ESBIMGBOOT_ROOT_CERT_IS_NOT_FIRST:
		ERROR("%s\n", "Root certificate isn't firstly");
		break;

	case ESBIMGBOOT_ROOT_CERT_X509_ERR:
		ERROR("%s\n", "X.509 error");
		break;

	case ESBIMGBOOT_ROOT_CERT_BAD_HASH:
		ERROR("%s\n", "Incorrect hash of root certificate");
		break;

	case ESBIMGBOOT_ROOT_CERT_REVOKED:
		ERROR("%s\n", "Root certificate revoked");
		break;

	case ESBIMGBOOT_NON_ROOT_CERT_TOO_MUCH_CERTS:
		ERROR("%s\n", "Intermediate certificates number is too much");
		break;

	case ESBIMGBOOT_NON_ROOT_CERT_IS_FIRST:
		ERROR("%s\n", "Intermediate certificate is firstly");
		break;

	case ESBIMGBOOT_NON_ROOT_CERT_X509_ERR:
		ERROR("%s\n", "Intermediate certificate X.509 error");
		break;

	case ESBIMGBOOT_ENC_KEY_BAD_CERT_CHAIN:
		ERROR("%s\n", "Encryption key: incorrect chain of certificates");
		break;

	case ESBIMGBOOT_ENC_KEY_NO_CERT_CHAIN:
		ERROR("%s\n", "Encryption key: certificate is missed in chain");
		break;

	case ESBIMGBOOT_ENC_KEY_IS_NOT_SIGNED:
		ERROR("%s\n", "Encryption key: isn't signed");
		break;

	case ESBIMGBOOT_ENC_KEY_BAD_HASH:
		ERROR("%s\n", "Encryption key: incorrect hash");
		break;

	case ESBIMGBOOT_ENC_KEY_BAD_SIGNATURE:
		ERROR("%s\n", "Encryption key: incorrect signature");
		break;

	case ESBIMGBOOT_PAYLOAD_HEADER_ERR:
		ERROR("%s\n", "Payload: incorrect header");
		break;

	case ESBIMGBOOT_PAYLOAD_BAD_CERT_CHAIN:
		ERROR("%s\n", "Payload: incorrect chain of certificates");
		break;

	case ESBIMGBOOT_PAYLOAD_NO_CERT_CHAIN:
		ERROR("%s\n", "Payload: certificate is missed in chain");
		break;

	case ESBIMGBOOT_PAYLOAD_IS_NOT_SIGNED:
		ERROR("%s\n", "Payload: isn't signed");
		break;

	case ESBIMGBOOT_PAYLOAD_IS_NOT_ENCRYPTED:
		ERROR("%s\n", "Payload: isn't encrypted");
		break;

	case ESBIMGBOOT_PAYLOAD_BAD_HASH:
		ERROR("%s\n", "Payload: incorrect hash");
		break;

	case ESBIMGBOOT_PAYLOAD_BAD_SIGNATURE:
		ERROR("%s\n", "Payload: incorrect signature");
		break;

	case ESBIMGBOOT_PAYLOAD_BAD_FW_COUNTER:
		ERROR("%s\n", "Payload: incorrect firmware counter");
		break;

	case ESBIMGBOOT_MALLOC_ERR:
		ERROR("%s\n", "Memory allocation error");
		break;

	default:
		ERROR("%s\n", "Unknown status");
		break;
	}
	return 0;
}

static inline void sblimg_print_return_code(int status)
{
	if (!status)
		NOTICE("%s\n", "Operation performed successfully");
	else if (status < -__ELASTERROR)
		sblimg_print_error(-status);
	else if (status >= ESBIMGBOOTBASE)
		sblimg_print_status(status);
	else
		ERROR("%s\n", strerror(status));
}
