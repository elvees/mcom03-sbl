// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <stdint.h>

#include <libs/errors.h>

#include "env-io.h"

#if ENABLE_ENV_RAM
#include "env-io-ram.h"
#endif

#if ENABLE_ENV_SPI
#include "env-io-spi.h"
#endif

int32_t env_io_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset)
{
	int32_t ret = 0;

	if (cfg && cfg->ops)
		ret = cfg->ops->write(cfg, data, size, offset);

	return ret;
}

int32_t env_io_read(env_io_config_t *cfg, void *data, size_t size, size_t offset)
{
	int32_t ret = 0;

	if (cfg && cfg->ops)
		ret = cfg->ops->read(cfg, data, size, offset);

	return ret;
}

int32_t env_io_clear(env_io_config_t *cfg)
{
	int32_t ret = 0;

	if (cfg && cfg->ops)
		ret = cfg->ops->clear(cfg);

	return ret;
}

int32_t env_io_invalidate(env_io_config_t *cfg)
{
	int32_t ret = 0;

	if (cfg && cfg->ops)
		ret = cfg->ops->invalidate(cfg);

	return ret;
}

int32_t env_io_init(env_io_config_t *cfg, signed long offset, size_t size,
                    env_io_location_t location)
{
	int32_t ret = 0;

	switch (location) {
#if ENABLE_ENV_SPI
	case ENV_IO_SPI:
		ret = env_io_spi_init(cfg, offset, size);
		break;
#endif
#if ENABLE_ENV_RAM
	case ENV_IO_RAM:
		ret = env_io_ram_init(cfg, offset, size);
		break;
#endif
	default:
		ret = -ENOTSUPPORTED;
		break;
	}

	return ret;
}

int32_t env_io_deinit(env_io_config_t *cfg)
{
	int32_t ret = 0;

	if (cfg && cfg->ops)
		ret = cfg->ops->deinit(cfg);

	return ret;
}
