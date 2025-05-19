// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libs/errors.h>
#include <libs/utils-def.h>

#include "env-io-ram.h"
#include "env-io.h"

#if ENABLE_ENV_RAM

static int32_t env_io_ram_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset)
{
	if (!cfg)
		return -ENULL;

	offset += cfg->offset;

	memcpy((void *)offset, data, size);

	return 0;
}

static int32_t env_io_ram_read(env_io_config_t *cfg, void *data, size_t size, size_t offset)
{
	if (!cfg)
		return -ENULL;

	offset += cfg->offset;

	memcpy(data, (void *)offset, size);

	return 0;
}

static int32_t env_io_ram_clear(env_io_config_t *cfg)
{
	if (!cfg)
		return -ENULL;

	memset((void *)cfg->offset, 0, cfg->size);

	return 0;
}

static int32_t env_io_ram_invalidate(env_io_config_t *cfg)
{
	return env_io_ram_clear(cfg);
}

static int32_t env_io_ram_deinit(env_io_config_t *cfg)
{
	if (!cfg)
		return -ENULL;

	memset((void *)cfg, 0, sizeof(cfg));

	return 0;
}

static env_io_ops_t env_io_ram_ops = { .write = env_io_ram_write,
	                               .read = env_io_ram_read,
	                               .clear = env_io_ram_clear,
	                               .invalidate = env_io_ram_invalidate,
	                               .deinit = env_io_ram_deinit };

int32_t env_io_ram_init(env_io_config_t *cfg, signed long offset, size_t size)
{
	if (!cfg)
		return -ENULL;

	if (offset < 0)
		return -EFORBIDDEN;
	else
		cfg->offset = (uint32_t)offset;

	cfg->size = size;

	cfg->ops = &env_io_ram_ops;
	cfg->location = ENV_IO_RAM;

	return 0;
}

#endif
