// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <drivers/nvm/nvm.h>
#include <lib/utils_def.h>
#include "env_io.h"

static int32_t env_io_read_after_write(env_io_config_t *cfg, const uint8_t *data, size_t size,
				       size_t offset)
{
	uint8_t scratch[16];

	if (cfg == NULL)
		return MCOM03_ERROR_NULL;

	while (size) {
		uint32_t portion_size = MIN((size_t)16, size);
		int32_t rc = nvm_read(&nvm_device, &scratch[0], offset, portion_size);
		if (rc != MCOM03_SUCCESS)
			return rc;

		if (memcmp(&scratch[0], data, portion_size))
			return MCOM03_ERROR_INVALID_DATA;

		size -= portion_size;
		data += portion_size;
		offset += portion_size;
	}
	return 0;
}

int32_t env_io_write(env_io_config_t *cfg, const void *data, size_t size, size_t offset)
{
	if (cfg == NULL)
		return MCOM03_ERROR_NULL;

	offset += cfg->offset;
	int32_t rc = nvm_write(&nvm_device, data, offset, size);
	if (rc != MCOM03_SUCCESS)
		return rc;

	return env_io_read_after_write(cfg, (const uint8_t *)data, size, offset);
}

int32_t env_io_read(env_io_config_t *cfg, void *data, size_t size, size_t offset)
{
	if (cfg == NULL)
		return MCOM03_ERROR_NULL;

	offset += cfg->offset;
	return nvm_read(&nvm_device, data, offset, size);
}

int32_t env_io_erase(env_io_config_t *cfg)
{
	if (cfg == NULL)
		return MCOM03_ERROR_NULL;

	uint32_t count = ALIGN_UP(cfg->size, nvm_device.nor_flash->flash_info.sector_size) /
			 nvm_device.nor_flash->flash_info.sector_size;

	return nvm_erase(&nvm_device, cfg->offset, count);
}

int32_t env_io_init(env_io_config_t *cfg, signed long offset, size_t size)
{
	if (cfg == NULL)
		return MCOM03_ERROR_NULL;

	if (offset < 0)
		cfg->offset =
			(uint32_t)((signed long)nvm_device.nor_flash->flash_info.size_in_bytes +
				   offset);
	else
		cfg->offset = (uint32_t)offset;

	cfg->size = size;

	return 0;
}
