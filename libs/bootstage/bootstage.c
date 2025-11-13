// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <drivers/timer/timer.h>
#include <libs/log.h>
#include <libs/platform-def-common.h>

#include "bootstage.h"

struct bootstage_record {
	uint64_t time_us;
	uint64_t reserved; /* Reserved for duration time */
	enum bootstage_id id;
};

struct bootstage_data {
	uint32_t rec_count;
	struct bootstage_record record[PLAT_BOOTSTAGE_RECORD_COUNT];
};

enum {
	BOOTSTAGE_VERSION = 0x0100,
	BOOTSTAGE_MAGIC = 0x45FBE3D6,
};

struct bootstage_hdr {
	uint32_t version; /* BOOTSTAGE_VERSION */
	uint32_t count; /* Number of records */
	uint32_t size; /* Total data size (non-zero if valid) */
	uint32_t magic; /* Magic number */
};

static struct bootstage_data bootstage = { 0 };

/**
 * Find record with Bootstage ID
 *
 * @param data	Pointer to bootstage records struct
 * @param id	Bootstage ID to find
 */
static struct bootstage_record *find_id(struct bootstage_data *data, enum bootstage_id id)
{
	struct bootstage_record *rec;
	struct bootstage_record *end;

	for (rec = data->record, end = rec + data->rec_count; rec < end; rec++)
		if (rec->id == id)
			return rec;

	return NULL;
}

/**
 * Append data to a memory buffer
 *
 * Write data to the buffer if there is space. Whether there is space or not,
 * the buffer pointer is incremented.
 *
 * @param ptrp	Pointer to buffer, updated by this function
 * @param end	Pointer to end of buffer
 * @param data	Data to write to buffer
 * @param size	Size of data
 */
static void append_data(char **ptrp, char *end, const void *data, int size)
{
	char *ptr = *ptrp;

	*ptrp += size;
	if (*ptrp > end)
		return;

	memcpy(ptr, data, size);
}

/**
 * Add a new bootstage record
 *
 * @param id	Bootstage ID to use
 * @param mark	Time to record in this record, in microseconds
 *
 * Return: recorded time stamp
 */
static uint64_t bootstage_add_record(enum bootstage_id id, uint64_t mark)
{
	struct bootstage_data *data = &bootstage;
	struct bootstage_record *rec;

	/* Only record the first event for each */
	rec = find_id(data, id);
	if (!rec) {
		if (data->rec_count < PLAT_BOOTSTAGE_RECORD_COUNT) {
			rec = &data->record[data->rec_count++];
			rec->time_us = mark;
			rec->id = id;
		} else {
			WARN("Bootstage space exhausted\n");
		}
	}

	return mark;
}

uint64_t bootstage_mark(enum bootstage_id id)
{
	return bootstage_add_record(id, timer_get_us());
}

int64_t bootstage_get_timestamp(enum bootstage_id id)
{
	struct bootstage_data *data = &bootstage;
	struct bootstage_record *rec;

	rec = find_id(data, id);
	if (rec)
		return (int64_t)rec->time_us;

	return -ENOENT;
}

int bootstage_export(void *base, int size)
{
	const struct bootstage_data *data = &bootstage;
	struct bootstage_hdr *hdr = (struct bootstage_hdr *)base;
	const struct bootstage_record *rec;
	char *ptr = base, *end = ptr + size;
	int i;

	if (size == -1)
		end = (char *)(~(uintptr_t)0);

	if (hdr + 1 > (struct bootstage_hdr *)end) {
		VERBOSE("%s: Not enough space for bootstage hdr\n", __func__);
		return -EPERM;
	}

	/* Write an arbitrary version number */
	hdr->version = BOOTSTAGE_VERSION;

	hdr->count = data->rec_count;
	hdr->size = 0;
	hdr->magic = BOOTSTAGE_MAGIC;
	ptr += sizeof(*hdr);

	/* Write the records, silently stopping when we run out of space */
	for (rec = data->record, i = 0; i < data->rec_count; i++, rec++)
		append_data(&ptr, end, rec, sizeof(*rec));

	/* Check for buffer overflow */
	if (ptr > end) {
		VERBOSE("%s: Not enough space for bootstage export\n", __func__);
		return -EPERM;
	}

	/* Update total data size */
	hdr->size = ptr - (char *)base;
	VERBOSE("Exported %lu records\n", hdr->count);

	return 0;
}

int bootstage_import(const void *base, int size)
{
	const struct bootstage_hdr *hdr = (struct bootstage_hdr *)base;
	struct bootstage_data *data = &bootstage;
	const char *ptr = base, *end = ptr + size;
	struct bootstage_record *rec;
	uint32_t rec_size;

	if (size == -1)
		end = (char *)(~(uintptr_t)0);

	if (hdr + 1 > (struct bootstage_hdr *)end) {
		VERBOSE("%s: Not enough space for bootstage hdr\n", __func__);
		return -EPERM;
	}

	if (hdr->magic != BOOTSTAGE_MAGIC) {
		VERBOSE("%s: Invalid bootstage magic\n", __func__);
		return -ENOENT;
	}

	if (ptr + hdr->size > end) {
		VERBOSE("%s: Bootstage data runs past buffer end\n", __func__);
		return -ENOSPC;
	}

	if (hdr->count * sizeof(*rec) > hdr->size) {
		VERBOSE("%s: Bootstage has %lu records needing %llu bytes, but "
		        "only %lu bytes is available\n",
		        __func__, hdr->count, (uint64_t)hdr->count * sizeof(*rec), hdr->size);
		return -ENOSPC;
	}

	if (hdr->version != BOOTSTAGE_VERSION) {
		VERBOSE("%s: Bootstage data version %lx unrecognised\n", __func__, hdr->version);
		return -EINVAL;
	}

	if (data->rec_count + hdr->count > PLAT_BOOTSTAGE_RECORD_COUNT) {
		VERBOSE("%s: Bootstage has %lu records, we have space for %lu\n"
		        "Please increase PLAT_BOOTSTAGE_RECORD_COUNT\n",
		        __func__, hdr->count, PLAT_BOOTSTAGE_RECORD_COUNT - data->rec_count);
		return -ENOSPC;
	}

	ptr += sizeof(*hdr);

	/* Read the records */
	rec_size = hdr->count * sizeof(*data->record);
	memcpy(data->record + data->rec_count, ptr, rec_size);

	/* Mark the records as read */
	data->rec_count += hdr->count;
	VERBOSE("Imported %lu records\n", hdr->count);

	return 0;
}

int bootstage_init(enum bootstage_id id)
{
	return bootstage_add_record(id, 0);
}
