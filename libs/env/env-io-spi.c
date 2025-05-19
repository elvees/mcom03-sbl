// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <drivers/spi-nor/spi-nor.h>
#include <libs/errors.h>
#include <libs/utils-def.h>

#include "env-io-spi.h"
#include "env-io.h"

#if ENABLE_ENV_SPI

static int32_t env_io_read_after_write(env_io_config_t *cfg, const uint8_t *data, size_t size,
                                       size_t offset)
{
	uint8_t scratch[16];

	if (!cfg)
		return -ENULL;

	while (size) {
		uint32_t portion_size = MIN((size_t)16, size);
		int32_t rc = spi_nor_read(&scratch[0], offset, portion_size);
		if (rc)
			return rc;

		if (memcmp(&scratch[0], data, portion_size))
			return -EINVALIDDATA;

		size -= portion_size;
		data += portion_size;
		offset += portion_size;
	}
	return 0;
}

static int32_t env_io_spi_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset)
{
	int32_t rc;

	if (!cfg)
		return -ENULL;

	offset += cfg->offset;

	rc = spi_nor_write(data, offset, size);
	if (rc)
		return rc;

	return env_io_read_after_write(cfg, (const uint8_t *)data, size, offset);
}

static int32_t env_io_spi_read(env_io_config_t *cfg, void *data, size_t size, size_t offset)
{
	if (!cfg)
		return -ENULL;

	offset += cfg->offset;

	return spi_nor_read(data, offset, size);
}

static int32_t env_io_spi_clear(env_io_config_t *cfg)
{
	uint32_t sector_size;

	if (!cfg)
		return -ENULL;

	sector_size = spi_nor_get_sector_size();
	if (!sector_size)
		return -EINVALIDDATA;

	uint32_t count = ALIGN_UP(cfg->size, sector_size) / sector_size;

	return spi_nor_erase(cfg->offset, count);
}

static int32_t env_io_spi_invalidate(env_io_config_t *cfg)
{
	return env_io_spi_clear(cfg);
}

static int32_t env_io_spi_deinit(env_io_config_t *cfg)
{
	if (!cfg)
		return -ENULL;

	memset((void *)cfg, 0, sizeof(cfg));

	return 0;
}

static env_io_ops_t env_io_spi_ops = { .write = env_io_spi_write,
	                               .read = env_io_spi_read,
	                               .clear = env_io_spi_clear,
	                               .invalidate = env_io_spi_invalidate,
	                               .deinit = env_io_spi_deinit };

int32_t env_io_spi_init(env_io_config_t *cfg, signed long offset, size_t size)
{
	uint32_t flash_size;

	if (!cfg)
		return -ENULL;

	if (offset < 0) {
		flash_size = spi_nor_get_size();
		if (!flash_size)
			return -EINVALIDDATA;

		cfg->offset = (signed long)flash_size + offset;
	} else {
		cfg->offset = offset;
	}

	cfg->size = size;

	cfg->ops = &env_io_spi_ops;
	cfg->location = ENV_IO_SPI;

	return 0;
}

#endif
