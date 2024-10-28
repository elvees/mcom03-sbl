// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __ENV_IO_H__
#define __ENV_IO_H__

typedef struct {
	uint32_t offset;
	uint32_t size;
} env_io_config_t;

int32_t env_io_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset);
int32_t env_io_read(env_io_config_t *cfg, void *data, size_t size, size_t offset);
int32_t env_io_erase(env_io_config_t *cfg);
int32_t env_io_init(env_io_config_t *cfg, signed long offset, size_t size);

#endif /* __ENV_IO_H__ */
