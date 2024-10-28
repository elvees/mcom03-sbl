// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __SBSTATUS_PRINT_H__
#define __SBSTATUS_PRINT_H__

#include <log.h>
#include <mcom03_errors.h>

static inline int sblimg_print_status(int status)
{
	switch (status) {
	case MCOM03_STATUS_SBIMG_BOOT_NO_ERR: {
		NOTICE("%s\n\r", "No Error");
	} break;

	case MCOM03_STATUS_SBIMG_BOOT_LOAD_FINISH: {
		NOTICE("%s\n\r", "Load finished");
	} break;

	case MCOM03_STATUS_SBIMG_BOOT_LOAD_CONTINUE: {
		NOTICE("%s\n\r", "load continue");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_HEADER_ID: {
		ERROR("%s\n\r", "Incorrect header id");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_HEADER_HASH: {
		ERROR("%s\n\r", "Incorrect header hash");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_IMAGE_BAD_TYPE: {
		ERROR("%s\n\r", "Incorrect type");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_IS_NOT_FIRST: {
		ERROR("%s\n\r", "Root certificate isn't firstly");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_X509_ERROR: {
		ERROR("%s\n\r", "X.509 error");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_BAD_HASH: {
		ERROR("%s\n\r", "Incorrect hash of root certificate");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ROOT_CERT_REVOKED: {
		ERROR("%s\n\r", "Root certificate revoked");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_TOO_MUCH_CERTS: {
		ERROR("%s\n\r", "Intermediate certificates number is too much");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_IS_FIRST: {
		ERROR("%s\n\r", "Intermediate certificate is firstly");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_NON_ROOT_CERT_X509_ERROR: {
		ERROR("%s\n\r", "Intermediate certificate X.509 error");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_CERT_CHAIN: {
		ERROR("%s\n\r", "Encryption key: incorrect chain of certificates");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_NO_CERT_CHAIN: {
		ERROR("%s\n\r", "Encryption key: certificate is missed in chain");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_IS_NOT_SIGNED: {
		ERROR("%s\n\r", "Encryption key: isn't signed");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_HASH: {
		ERROR("%s\n\r", "Encryption key: incorrect hash");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_ENC_KEY_BAD_SIGNATURE: {
		ERROR("%s\n\r", "Encryption key: incorrect signature");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_HEADER_ERROR: {
		ERROR("%s\n\r", "Payload: incorrect header");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_CERT_CHAIN: {
		ERROR("%s\n\r", "Payload: incorrect chain of certificates");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_NO_CERT_CHAIN: {
		ERROR("%s\n\r", "Payload: certificate is missed in chain");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_SIGNED: {
		ERROR("%s\n\r", "Payload: isn't signed");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_IS_NOT_ENCRYPTED: {
		ERROR("%s\n\r", "Payload: isn't encrypted");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_HASH: {
		ERROR("%s\n\r", "Payload: incorrect hash");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_SIGNATURE: {
		ERROR("%s\n\r", "Payload: incorrect signature");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_PAYLOAD_BAD_FW_COUNTER: {
		ERROR("%s\n\r", "Payload: incorrect firmware counter");
	} break;

	case MCOM03_ERROR_SBIMG_BOOT_MALLOC_ERROR: {
		ERROR("%s\n\r", "Memory allocation error");
	}

	case MCOM03_SUCCESS: {
		ERROR("%s\n\r", "Operation performed successfully");
	} break;

	case MCOM03_ERROR_INTERNAL: {
		ERROR("%s\n\r", "Internal error");
	} break;

	case MCOM03_ERROR_NO_MEM: {
		ERROR("%s\n\r", "No memory for operation");
	} break;

	case MCOM03_ERROR_NOT_SUPPORTED: {
		ERROR("%s\n\r", "Not supported");
	} break;

	case MCOM03_ERROR_INVALID_PARAM: {
		ERROR("%s\n\r", "Invalid parameter");
	} break;

	case MCOM03_ERROR_INVALID_STATE: {
		ERROR("%s\n\r", "Invalid state, operation disallowed in this state");
	} break;

	case MCOM03_ERROR_INVALID_LENGTH: {
		ERROR("%s\n\r", "Invalid length");
	} break;

	case MCOM03_ERROR_INVALID_FLAGS: {
		ERROR("%s\n\r", "Invalid Flags");
	} break;

	case MCOM03_ERROR_INVALID_DATA: {
		ERROR("%s\n\r", "Invalid Data");
	} break;

	case MCOM03_ERROR_DATA_SIZE: {
		ERROR("%s\n\r", "Data size exceeds limit");
	} break;

	default: {
		ERROR("%s\n\r", "Unknown status");
	} break;
	}
	return 0;
}

#endif /* __SBSTATUS_PRINT_H__ */
