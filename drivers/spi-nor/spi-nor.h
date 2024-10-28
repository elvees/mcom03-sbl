// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#include <stdint.h>

typedef struct {
	char *name;
	uint32_t page_size;
	uint32_t sector_size;
	uint32_t size_in_bytes;
	uint8_t addr_bytes;
	uint32_t flags;
} nor_flash_t;

int spi_nor_init(void);
int spi_nor_write(const void *buffer, uint32_t address, uint32_t size);
int spi_nor_read(void *buffer, uint32_t address, uint32_t size);
int spi_nor_erase(uint32_t address, uint32_t sector_count);
uint32_t spi_nor_get_size(void);
uint32_t spi_nor_get_sector_size(void);
