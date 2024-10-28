// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

typedef struct {
	// Data type that contain image (Table 2.4)
	unsigned int obj_type : 3;
	/**
	 * 0: Integrity checking is off.
	 * 1: Integrity checking is on.
	 */
	unsigned int checksum : 1;
	/**
	 * 0: Data are not encrypted.
	 * 1: Data are encrypted.
	 */
	unsigned int encrypted : 1;
	/**
	 * 0: Payload digest field contain unencrypted data hash.
	 * 1: Payload digest field contain encrypted data hash.
	 */
	unsigned int sign_of_encrypted : 1;
	/**
	 * 0: Object is not signed.
	 * 1: Object is signed.
	 */
	unsigned int signed_obj : 1;
	// 1: In bs_en=0 mode skip header checksum verification
	unsigned int skip_header_hash : 1;
	// Reserved
	unsigned int reserved : 24;
} flags_t;

typedef struct {
	// Image type ID always 0x53424d47 ("SBMG")
	unsigned int h_id;
	// Data size in bytes leading after this header
	unsigned int pl_size;
	// Loading data address
	unsigned int l_addr;
	// Program enter address (if image contain executing program)
	unsigned int e_addr;
	union {
		unsigned int flags;
		// Flags Field
		flags_t flags_bits;
	};
	// Key number for decription (lowest 16 bits are used)
	unsigned int aes_key_num;
	// ID number if it is certificate
	unsigned int cert_id;
	// Certificate ID number which is used for data signature (if it is used)
	unsigned int sign_cert_id;
	// Data hash leading after that header (hash function SHA-256)
	unsigned char pl_dgst[32];
	// Header hash (hash function SHA-256). While calculating this field equals 0
	unsigned char h_dgst[32];
} sbimghdr_t;
