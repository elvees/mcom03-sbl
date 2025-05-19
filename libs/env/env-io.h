// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#ifndef ENABLE_ENV_SPI
#define ENABLE_ENV_SPI 1
#endif

typedef enum { ENV_IO_UNKNOWN = 0x00, ENV_IO_SPI, ENV_IO_RAM } env_io_location_t;

typedef struct {
	uint32_t crc; // CRC32 over data bytes
	char data[0]; // Environment data
} env_io_t;

typedef struct env_io_ops env_io_ops_t;

typedef struct {
	signed long offset;
	uint32_t size;
	env_io_location_t location;
	env_io_ops_t *ops;
} env_io_config_t;

typedef struct env_io_ops {
	int32_t (*write)(env_io_config_t *cfg, const void *data, size_t size, size_t offset);
	int32_t (*read)(env_io_config_t *cfg, void *data, size_t size, size_t offset);
	int32_t (*clear)(env_io_config_t *cfg);
	int32_t (*invalidate)(env_io_config_t *cfg);
	int32_t (*deinit)(env_io_config_t *cfg);
} env_io_ops_t;

int32_t env_io_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset);
int32_t env_io_read(env_io_config_t *cfg, void *data, size_t size, size_t offset);
int32_t env_io_clear(env_io_config_t *cfg);
int32_t env_io_invalidate(env_io_config_t *cfg);
int32_t env_io_init(env_io_config_t *cfg, signed long offset, size_t size,
                    env_io_location_t location);
int32_t env_io_deinit(env_io_config_t *cfg);

#ifdef __cplusplus
}
#endif
