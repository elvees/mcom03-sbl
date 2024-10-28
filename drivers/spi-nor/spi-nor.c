// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <drivers/qspi/qspi.h>
#include <libs/errors.h>
#include <libs/log.h>
#include <libs/utils-def.h>

#include "spi-nor.h"

#define CMD_WREN       0x06 // write enable command
#define CMD_WRDI       0x04 // write disable command
#define CMD_RDSR1      0x05 // read status register-1 command
#define CMD_RDID       0x9F // read ID command (JEDEC Manufacturer ID and JEDEC CFI)
#define CMD_PP         0x02 // page program command (3- or 4-byte address)
#define CMD_4PP        0x12 // page program command (4-byte address)
#define CMD_SE         0xD8 // erase 64 kB or 256 kB sector command (3- or 4-byte address)
#define CMD_4SE        0xDC // erase 64 kB or 256 kB sector command (4-byte address)
#define CMD_READ       0x03 // read command (3- or 4-byte address)
#define CMD_FAST_READ  0x0B // fast read command (3- or 4-byte address)
#define CMD_4FAST_READ 0x0C // fast read command (4-byte address)
#define CMD_4READ      0x13 // read command (4-byte address)
#define CMD_BULK_ERASE 0x60 // bulk erase command
#define CMD_RDCR1      0x35 // read configuration register-1 command
#define CMD_CLSR1      0x30 // clear status register-1 command - Erase/Program Fail Reset

// Status Register 1 (SR1)
#define SR1_WIP  BIT(0)
#define SR1_WEL  BIT(1)
#define SR1_SRWD BIT(7)

// Configuration Register 1 (CR1)
#define CR1_TBPARM BIT(2)

// CR1 register has on Cypress flashes
#define FLAG_HAS_CR1 BIT(0)

// SR1 register can contains P_ERR and E_ERR bits
#define FLAG_HAS_ERR_BITS BIT(1)

// Some Cypress flashes has different erase sector sizes at begin/end of flash memory
#define FLAG_DIFFERENT_SECTORS BIT(2)

// With FLAG_DIFFERENT_SECTORS small erase sectors (4K) can be at begin or at end of memory
#define FLAG_4KSECT_BEGIN BIT(3)
#define FLAG_4KSECT_END   BIT(4)

#define KiB 1024
#define MiB (1024 * KiB)

#define SPI_NOR_ERASE_CHIP 0

struct spi_nor_id {
	char *name;
	uint32_t id;
	uint16_t id_ext;
	uint32_t size;
	uint32_t sector_size;
	uint32_t page_size;
	uint32_t flags;
};

static struct spi_nor_id spi_nor_ids[] = {
	{ "M25P32", 0x202016, 0, 4 * MiB, 64 * KiB, 256, 0 },
	{ "S25FL128S", 0x012018, 0x0080, 16 * MiB, 256 * KiB, 256,
	  FLAG_HAS_CR1 | FLAG_HAS_ERR_BITS },
	{ "S25FL128S", 0x012018, 0x0180, 16 * MiB, 64 * KiB, 256,
	  FLAG_HAS_CR1 | FLAG_HAS_ERR_BITS | FLAG_DIFFERENT_SECTORS },
	{ "S25FL256S", 0x010219, 0x0080, 32 * MiB, 256 * KiB, 256,
	  FLAG_HAS_CR1 | FLAG_HAS_ERR_BITS },
	{ "S25FL256S", 0x010219, 0x0180, 32 * MiB, 64 * KiB, 256,
	  FLAG_HAS_CR1 | FLAG_HAS_ERR_BITS | FLAG_DIFFERENT_SECTORS },
	{ "W25Q16JW-IM", 0xEF8015, 0, 2 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q16JW-IQ/JQ", 0xEF6015, 0, 2 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q32", 0xEF4016, 0, 4 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q128JV-IN/IQ/JQ", 0xEF4018, 0, 16 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q128JV-IM/JM", 0xEF7018, 0, 16 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q128FW/JW", 0xEF6018, 0, 16 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q256JV-IQ/IN/JQ", 0xEF4019, 0, 32 * MiB, 64 * KiB, 256, 0 },
	{ "W25Q256JV-IM/JM", 0xEF7019, 0, 32 * MiB, 64 * KiB, 256, 0 },
};

static nor_flash_t nor_flash;

static int __spi_nor_jedec_id(uint8_t *jedec_id, uint32_t size)
{
	uint8_t cmd = CMD_RDID;

	if (!jedec_id)
		return -ENULL;

	return qspi_xfer(&cmd, 1, jedec_id, size);
}

static int spi_nor_read_cr1(uint8_t *cr1)
{
	uint8_t cmd = CMD_RDCR1;

	if (!cr1)
		return -ENULL;

	return qspi_xfer(&cmd, 1, cr1, 1);
}

static int spi_nor_read_status1(uint8_t *sr1)
{
	uint8_t cmd = CMD_RDSR1;

	if (!sr1)
		return -ENULL;

	return qspi_xfer(&cmd, 1, sr1, 1);
}

static int spi_nor_write_enable(void)
{
	uint8_t cmd = CMD_WREN;

	return qspi_xfer(&cmd, 1, NULL, 0);
}

static int spi_nor_write_disable(void)
{
	uint8_t cmd = CMD_WRDI;

	return qspi_xfer(&cmd, 1, NULL, 0);
}

static int spi_nor_clear_sr1_err(void)
{
	uint8_t cmd = CMD_CLSR1;

	return qspi_xfer(&cmd, 1, NULL, 0);
}

static int spi_nor_read_wip(uint32_t timeout)
{
	int ret;
	uint32_t counter = 0;
	uint8_t rx_buff = SR1_WIP;

	while (rx_buff & SR1_WIP) {
		if (timeout && (counter++ >= timeout))
			break;

		ret = spi_nor_read_status1(&rx_buff);
		if (ret)
			return ret;
	}

	if (rx_buff & SR1_WIP)
		return -ETIMEOUT;

	return 0;
}

static int spi_nor_erase_sector(uint32_t addr, uint32_t sector_count)
{
	int ret = -EINVALIDSTATE;
	int addr_bytes;
	uint8_t cmd[5];

	if (nor_flash.flags & FLAG_HAS_ERR_BITS) {
		ret = spi_nor_clear_sr1_err();
		if (ret)
			return ret;
	}

	addr_bytes = nor_flash.addr_bytes;
	for (uint32_t i = 0; i < sector_count; i++) {
		if (addr_bytes == 4) {
			cmd[0] = CMD_4SE;
			cmd[1] = (uint8_t)(addr >> 24);
			cmd[2] = (uint8_t)(addr >> 16);
			cmd[3] = (uint8_t)(addr >> 8);
			cmd[4] = (uint8_t)(addr >> 0);
		} else {
			cmd[0] = CMD_SE;
			cmd[1] = (uint8_t)(addr >> 16);
			cmd[2] = (uint8_t)(addr >> 8);
			cmd[3] = (uint8_t)(addr >> 0);
		}
		addr += nor_flash.sector_size;

		ret = spi_nor_write_enable();
		if (ret)
			return ret;

		ret = qspi_xfer(cmd, addr_bytes + 1, NULL, 0);
		if (ret)
			return ret;

		ret = spi_nor_read_wip(0);
		if (ret)
			return ret;

		ret = spi_nor_write_disable();
		if (ret)
			return ret;
	}

	return ret;
}

static int spi_nor_erase_chip(void)
{
	int ret;
	uint8_t cmd = CMD_BULK_ERASE;

	if (nor_flash.flags & FLAG_HAS_ERR_BITS) {
		ret = spi_nor_clear_sr1_err();
		if (ret)
			return ret;
	}

	ret = spi_nor_write_enable();
	if (ret)
		return ret;

	ret = qspi_xfer(&cmd, 1, NULL, 0);
	if (ret)
		return ret;

	ret = spi_nor_read_wip(0);
	if (ret)
		return ret;

	ret = spi_nor_write_disable();
	if (ret)
		return ret;

	return 0;
}

static int spi_nor_write_page(uint8_t *buff, uint32_t addr, uint32_t page_size)
{
	int ret;
	uint8_t cmd[5];

	if (!buff)
		return -ENULL;

	if (nor_flash.addr_bytes == 4) {
		cmd[0] = CMD_4PP;
		cmd[1] = (uint8_t)(addr >> 24);
		cmd[2] = (uint8_t)(addr >> 16);
		cmd[3] = (uint8_t)(addr >> 8);
		cmd[4] = (uint8_t)(addr);
	} else {
		cmd[0] = CMD_PP;
		cmd[1] = (uint8_t)(addr >> 16);
		cmd[2] = (uint8_t)(addr >> 8);
		cmd[3] = (uint8_t)(addr);
	}

	ret = spi_nor_write_enable();
	if (ret)
		return ret;

	// Toggle SS pin to start transmission
	qspi_ss_ctrl(true);

	ret = qspi_write(cmd, nor_flash.addr_bytes + 1);
	if (ret)
		return ret;

	ret = qspi_write(buff, page_size);
	if (ret)
		return ret;

	// Toggle SS pin to end transmission
	qspi_ss_ctrl(false);

	ret = spi_nor_read_wip(0);
	if (ret)
		return ret;

	ret = spi_nor_write_disable();
	if (ret)
		return ret;

	return 0;
}

static int __spi_nor_init(void)
{
	struct spi_nor_id *spi_nor_id = NULL;
	int ret;
	uint32_t id;
	uint16_t id_ext;
	uint8_t jedecid[6];
	uint8_t cr1;

	ret = __spi_nor_jedec_id(jedecid, sizeof(jedecid));
	if (ret)
		return ret;

	id = (jedecid[0] << 16) | (jedecid[1] << 8) | jedecid[2];
	id_ext = (jedecid[4] << 8) | jedecid[5];

	for (int i = 0; i < ARRAY_SIZE(spi_nor_ids); i++) {
		if (spi_nor_ids[i].id == id &&
		    (spi_nor_ids[i].id_ext == 0 || spi_nor_ids[i].id_ext == id_ext)) {
			spi_nor_id = &spi_nor_ids[i];
			break;
		}
	}
	INFO("SPI NOR ID: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x (%s)\n", jedecid[0], jedecid[1],
	     jedecid[2], jedecid[3], jedecid[4], jedecid[5],
	     spi_nor_id ? spi_nor_id->name : "unknown");
	if (spi_nor_id) {
		nor_flash.name = spi_nor_id->name;
		nor_flash.size_in_bytes = spi_nor_id->size;
		nor_flash.page_size = spi_nor_id->page_size;
		nor_flash.sector_size = spi_nor_id->sector_size;
		nor_flash.addr_bytes = spi_nor_id->size > 0x1000000 ? 4 : 3;
		nor_flash.flags = spi_nor_id->flags;
	} else {
		return -ENOTSUPPORTED;
	}

	if ((nor_flash.flags & (FLAG_HAS_CR1 | FLAG_DIFFERENT_SECTORS)) ==
	    (FLAG_HAS_CR1 | FLAG_DIFFERENT_SECTORS)) {
		ret = spi_nor_read_cr1(&cr1);
		if (ret)
			return ret;
		nor_flash.flags |= (cr1 & CR1_TBPARM) ? FLAG_4KSECT_END : FLAG_4KSECT_BEGIN;
	}

	return 0;
}

int spi_nor_init(void)
{
	int ret;

	ret = qspi_init();
	if (ret) {
		ERROR("qspi init failed. Code : %d\n", ret);
		return ret;
	}

	ret = __spi_nor_init();
	if (ret) {
		ERROR("__spi_nor_init failed. Code : %d\n", ret);
		return ret;
	}

	ret = spi_nor_read_wip(UINT16_MAX);
	if (ret) {
		ERROR("spi_nor_read_wip failed. Code : %d\n", ret);
		return ret;
	}

	return 0;
}

int spi_nor_write(const void *buffer, uint32_t address, uint32_t size)
{
	int ret;
	uint8_t *buff_temp = (uint8_t *)buffer;
	uint32_t page_offset = (nor_flash.page_size - 1) & address;

	if (!buff_temp)
		return -ENULL;

	if (!size)
		return 0;

	if ((address + size) > nor_flash.size_in_bytes)
		return -EINVALIDPARAM;

	if (page_offset) {
		int write_on_this_page = nor_flash.page_size - page_offset;
		if (write_on_this_page > size)
			write_on_this_page = size;
		ret = spi_nor_write_page(buff_temp, address, write_on_this_page);
		if (ret)
			return ret;

		buff_temp += write_on_this_page;
		address += write_on_this_page;
		size -= write_on_this_page;
	}

	while (size >= nor_flash.page_size) {
		// TODO: add "wait or don't wait" flag
		ret = spi_nor_write_page(buff_temp, address, nor_flash.page_size);
		if (ret)
			return ret;

		buff_temp += nor_flash.page_size;
		address += nor_flash.page_size;
		size -= nor_flash.page_size;
	}

	if (size) {
		ret = spi_nor_write_page(buff_temp, address, size);
		if (ret)
			return ret;
	}

	return 0;
}

int spi_nor_read(void *buffer, uint32_t address, uint32_t size)
{
	uint8_t cmd[5];

	if (!buffer)
		return -ENULL;

	if (nor_flash.addr_bytes == 4) {
		cmd[0] = CMD_4READ;
		cmd[1] = (uint8_t)(address >> 24);
		cmd[2] = (uint8_t)(address >> 16);
		cmd[3] = (uint8_t)(address >> 8);
		cmd[4] = (uint8_t)(address);
	} else {
		cmd[0] = CMD_READ;
		cmd[1] = (uint8_t)(address >> 16);
		cmd[2] = (uint8_t)(address >> 8);
		cmd[3] = (uint8_t)(address);
	}

	return qspi_xfer(cmd, nor_flash.addr_bytes + 1, (uint8_t *)buffer, size);
}

int spi_nor_erase(uint32_t address, uint32_t sector_count)
{
	int ret;

	if (sector_count == SPI_NOR_ERASE_CHIP) {
		ret = spi_nor_erase_chip();
		if (ret)
			return ret;
	} else {
		ret = spi_nor_erase_sector(address, sector_count);
		if (ret)
			return ret;
	}

	return 0;
}

uint32_t spi_nor_get_size(void)
{
	return nor_flash.size_in_bytes;
}

uint32_t spi_nor_get_sector_size(void)
{
	return nor_flash.sector_size;
}
