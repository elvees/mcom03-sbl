// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <libs/errors.h>
#include <libs/log.h>
#include <third-party/crc/checksum.h>

#include "env-io.h"
#include "env.h"

/**
 * The "environment" is stored as a list of '\0' terminated
 * "name=value" strings. The end of the list is marked by a double
 * '\0'. New entries are always added at the end. Deleting an entry
 * shifts the remaining entries to the front. Replacing an entry is a
 * combination of deleting the old value and adding the new one.
 *
 * The environment is preceded by a 32 bit CRC over the data part.
 */

#define EXIT_PREP(ret, code, label) \
	do {                        \
		(ret) = (code);     \
		goto label;         \
	} while (0)

#define ENV_SIZE(ctx)       ((ctx)->cfg.size)
#define ENV_DATA_SIZE(ctx)  ((ctx)->cfg.size - sizeof(env_io_t))
#define ENV_FLAGS_VAR       ".flags"
#define ENV_ATTR_LIST_DELIM ','
#define ENV_ATTR_SEP        ':'
#define ENV_KEY_VALUE_SEP   '='

static int env_cmp_items(const void *item1, const void *item2)
{
	kv_t *kv1 = (kv_t *)item1;
	kv_t *kv2 = (kv_t *)item2;
	return strcmp(kv1->key, kv2->key);
}

static kv_t *env_get_item(kv_t *kvs, int kv_count, const char *name)
{
	if (!kvs || !name)
		return NULL;

	for (int i = 0; i < kv_count; ++i)
		if (kvs[i].key && !strcmp(kvs[i].key, name))
			return &kvs[i];

	return NULL;
}

static kv_t *env_get_item_by_index(kv_t *kvs, int kv_count, int idx)
{
	if (kvs && idx < kv_count)
		return &kvs[idx];

	return NULL;
}

static int env_get_item_index(kv_t *kvs, kv_t *kv, int kv_count)
{
	if (!kvs) {
		ERROR("Pointer to kv context is NULL\n");
		return -ENULL;
	}

	if (!kv) {
		ERROR("Pointer to kv item is NULL\n");
		return -ENULL;
	}

	int idx = (int)((ptrdiff_t)kv - (ptrdiff_t)kvs) / sizeof(kv_t);

	if (idx < 0 || idx >= kv_count) {
		ERROR("Wrong kv element index\n");
		return -EINVALIDPARAM;
	}

	return idx;
}

static int env_insert_item(kv_t **kvs, int *kv_count, const char *key, const char *value)
{
	uint32_t len;
	kv_t *kv;

	if (!kvs) {
		ERROR("Pointer to kv context is NULL\n");
		return -ENULL;
	}

	if (!kv_count) {
		ERROR("Pointer to kv count is NULL\n");
		return -ENULL;
	}

	if (!key) {
		ERROR("Empty key is provided\n");
		return -ENULL;
	}

	if (!value) {
		ERROR("Empty value is provided\n");
		return -ENULL;
	}

	kv = env_get_item(*kvs, *kv_count, key);
	if (kv) {
		len = strlen(value) + 1;
		char *new_value = (char *)malloc(len);
		if (!new_value) {
			ERROR("Can't alloc %ld bytes\n", len);
			return -ENULL;
		}
		strncpy(new_value, value, len);
		free(kv->value);
		kv->value = new_value;
	} else {
		len = (*kv_count + 1) * sizeof(kv_t);
		kv = (kv_t *)malloc(len);
		if (!kv) {
			ERROR("Can't alloc %ld bytes\n", len);
			return -ENULL;
		}
		if (*kvs)
			memcpy((void *)kv, (void *)(*kvs), (*kv_count) * sizeof(kv_t));

		len = strlen(key) + 1;
		kv[*kv_count].key = (char *)malloc(len);
		if (!kv[*kv_count].key) {
			free(kv);
			ERROR("Can't alloc %ld bytes\n", len);
			return -ENULL;
		}
		strncpy(kv[*kv_count].key, key, len);

		len = strlen(value) + 1;
		kv[*kv_count].value = (char *)malloc(len);
		if (!kv[*kv_count].value) {
			free(kv[*kv_count].key);
			free(kv);
			ERROR("Can't alloc %ld bytes\n", len);
			return -ENULL;
		}
		strncpy(kv[*kv_count].value, value, len);

		if (*kvs)
			free(*kvs);

		(*kvs) = kv;
		(*kv_count)++;
	}

	return 0;
}

static int env_delete_item(kv_t **kvs, int *kv_count, int *idx, const char *key)
{
	uint32_t len;
	kv_t *kv;

	kv = env_get_item(*kvs, *kv_count, key);
	if (!kv)
		return 0;

	// kv is a part of kvs array.
	*idx = env_get_item_index(*kvs, kv, *kv_count);

	len = (*kv_count - 1) * sizeof(kv_t);
	if (len) {
		kv = (kv_t *)malloc(len);
		if (!kv) {
			ERROR("Can't alloc %ld bytes\n", len);
			return -ENULL;
		}

		if (*idx)
			memcpy(&kv[0], &(*kvs)[0], *idx * sizeof(kv_t));
		if (*kv_count - *idx - 1)
			memcpy(&kv[*idx], &(*kvs)[*idx + 1], (*kv_count - *idx - 1) * sizeof(kv_t));
	} else {
		kv = NULL;
	}

	free((*kvs)[*idx].key);
	free((*kvs)[*idx].value);
	free((*kvs));
	(*kvs) = kv;
	(*kv_count)--;

	return 0;
}

static int env_prepare_io_ctx(env_ctx_t *ctx, env_io_t *env_io, uint32_t len)
{
	uint32_t off = 0U;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		return -ENULL;
	}

	if (!env_io) {
		ERROR("The IO environment context is NULL\n");
		return -ENULL;
	}

	if (ctx->ekv) {
		if (!ctx->ekv_count) {
			ERROR("Variable of flags has not to be zero\n");
			return -EINVALIDPARAM;
		}

		qsort(ctx->ekv, ctx->ekv_count, sizeof(*ctx->ekv), env_cmp_items);

		for (int i = 0; i < ctx->ekv_count; ++i) {
			char *key = ctx->ekv[i].key;
			char *value = ctx->ekv[i].value;
			if (key && value) {
				int n = snprintf(NULL, 0, "%s=%s", key, value);
				if ((off + n + 1) >= len) {
					ERROR("Can't add %s=%s to environment, no space\n", key,
					      value);
					break;
				}
				off += snprintf(&env_io->data[off], n + 1, "%s=%s", key, value) + 1;
			}
		}
	}

	env_io->data[off] = '\0';

	env_io->crc = crc_32((const unsigned char *)env_io->data, ENV_DATA_SIZE(ctx));

	return 0;
}

static int env_destroy(env_ctx_t *ctx)
{
	if (!ctx) {
		ERROR("Environment context is NULL\n");
		return -ENULL;
	}

	if (ctx->ekv) {
		for (int i = 0; i < ctx->ekv_count; ++i)
			if (ctx->ekv[i].key && ctx->ekv[i].value) {
				free(ctx->ekv[i].key);
				free(ctx->ekv[i].value);
			}

		free(ctx->ekv);
		ctx->ekv = NULL;
	}
	ctx->ekv_count = 0;

	if (ctx->fkv) {
		for (int i = 0; i < ctx->fkv_count; ++i)
			if (ctx->fkv[i].key && ctx->fkv[i].value) {
				free(ctx->fkv[i].key);
				free(ctx->fkv[i].value);
			}

		free(ctx->fkv);
		ctx->fkv = NULL;
	}
	ctx->fkv_count = 0;

	return 0;
}

int env_init(env_ctx_t *ctx, signed long offset, size_t size, env_io_location_t location)
{
	if (!ctx) {
		ERROR("Environment context is NULL\n");
		return -ENULL;
	}

	memset(ctx, 0, sizeof(env_ctx_t));

	return env_io_init(&ctx->cfg, offset, size, location);
}

int env_deinit(env_ctx_t *ctx)
{
	if (env_destroy(ctx)) {
		ERROR("Can't destroy ctx\n");
		return -EINVALIDSTATE;
	}

	return env_io_deinit(&ctx->cfg);
}

char *env_get(env_ctx_t *ctx, const char *name)
{
	if (!ctx)
		return NULL;

	kv_t *kv = env_get_item(ctx->ekv, ctx->ekv_count, name);
	if (!kv)
		return NULL;

	return kv->value;
}

char *env_get_key_next(env_ctx_t *ctx)
{
	if (!ctx)
		return NULL;

	kv_t *kv = env_get_item_by_index(ctx->ekv, ctx->ekv_count, ctx->eit.next);
	while (kv) {
		if (kv->key && kv->key[0] == '.') {
			ctx->eit.next++;
			kv = env_get_item_by_index(ctx->ekv, ctx->ekv_count, ctx->eit.next);
			continue;
		}

		ctx->eit.next++;
		return kv->key;
	}

	return NULL;
}

char *env_get_key_first(env_ctx_t *ctx)
{
	if (!ctx)
		return NULL;

	ctx->eit.next = 0;
	return env_get_key_next(ctx);
}

int env_set(env_ctx_t *ctx, const char *name, const char *value)
{
	int ret = 0;
	int idx;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		return -ENULL;
	}

	if (!name) {
		ERROR("Empty variable name\n");
		return -ENULL;
	}

	if (strchr(name, ENV_KEY_VALUE_SEP) || strchr(name, ENV_ATTR_LIST_DELIM) ||
	    strchr(name, ENV_ATTR_SEP)) {
		ERROR("Illegal character '%c', '%c' or '%c' in name '%s'\n", ENV_KEY_VALUE_SEP,
		      ENV_ATTR_LIST_DELIM, ENV_ATTR_SEP, name);
		return -EINVALIDPARAM;
	}

	if (!value) {
		if (env_delete_item(&ctx->ekv, &ctx->ekv_count, &idx, name))
			EXIT_PREP(ret, -EINVALIDSTATE, exit);
		if (ctx->eit.next > idx)
			ctx->eit.next--;
	} else if (env_insert_item(&ctx->ekv, &ctx->ekv_count, name, value)) {
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

exit:
	return ret;
}

int env_set_hex(env_ctx_t *ctx, const char *name, unsigned long long value)
{
	char str[19];

	snprintf(str, sizeof(str), "0x%llx", value);
	return env_set(ctx, name, str);
}

char *env_get_flag(env_ctx_t *ctx, const char *name)
{
	if (!ctx)
		return NULL;

	kv_t *kv = env_get_item(ctx->fkv, ctx->fkv_count, name);
	if (!kv)
		return NULL;

	return kv->value;
}

char *env_get_flag_key_next(env_ctx_t *ctx)
{
	if (!ctx)
		return NULL;

	kv_t *kv = env_get_item_by_index(ctx->fkv, ctx->fkv_count, ctx->fit.next);
	if (!kv)
		return NULL;

	ctx->fit.next++;
	return kv->key;
}

char *env_get_flag_key_first(env_ctx_t *ctx)
{
	if (!ctx)
		return NULL;

	ctx->fit.next = 0;
	return env_get_flag_key_next(ctx);
}

int env_set_flag(env_ctx_t *ctx, const char *name, const char *attr)
{
	uint32_t off = 0U;
	char *flags_value = NULL;
	int ret = 0;
	int idx;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		EXIT_PREP(ret, -ENULL, exit);
	}

	if (!name) {
		ERROR("Empty variable name\n");
		EXIT_PREP(ret, -ENULL, exit);
	}

	if (strchr(name, ENV_KEY_VALUE_SEP) || strchr(name, ENV_ATTR_LIST_DELIM) ||
	    strchr(name, ENV_ATTR_SEP)) {
		ERROR("Illegal character '%c', '%c' or '%c' in name '%s'\n", ENV_KEY_VALUE_SEP,
		      ENV_ATTR_LIST_DELIM, ENV_ATTR_SEP, name);
		EXIT_PREP(ret, -EINVALIDPARAM, exit);
	}

	if (!attr) {
		if (env_delete_item(&ctx->fkv, &ctx->fkv_count, &idx, name))
			EXIT_PREP(ret, -EINVALIDSTATE, exit);
		if (ctx->fit.next > idx)
			ctx->fit.next--;
	} else if (env_insert_item(&ctx->fkv, &ctx->fkv_count, name, attr)) {
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	if (ctx->fkv) {
		if (!ctx->fkv_count) {
			ERROR("Count of flags has not to be zero\n");
			EXIT_PREP(ret, -EINVALIDPARAM, exit);
		}

		qsort(ctx->fkv, ctx->fkv_count, sizeof(*ctx->fkv), env_cmp_items);

		for (int i = 0; i < ctx->fkv_count; ++i) {
			char *key = ctx->fkv[i].key;
			char *value = ctx->fkv[i].value;

			int n = snprintf(NULL, 0, "%s%c%s%c", key, ENV_ATTR_SEP, value,
			                 ENV_ATTR_LIST_DELIM);

			char *temp = (char *)realloc(flags_value, off + n + 1);
			if (!temp) {
				ERROR("Can't alloc %d bytes\n", n);
				EXIT_PREP(ret, -EINTERNAL, exit);
			}
			flags_value = temp;

			off += snprintf(&flags_value[off], n + 1, "%s%c%s%c", key, ENV_ATTR_SEP,
			                value, ENV_ATTR_LIST_DELIM);
		}

		flags_value[off - 1] = '\0';
	}

	if (env_set(ctx, ENV_FLAGS_VAR, flags_value)) {
		ERROR("Can't set variable with name %s\n", ENV_FLAGS_VAR);
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

exit:
	if (flags_value)
		free(flags_value);

	return ret;
}

int env_import(env_ctx_t *ctx)
{
	uint32_t len;
	env_io_t *env_io = NULL;
	char *flags_copy = NULL;
	uint32_t crc;
	int ret = 0;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		EXIT_PREP(ret, -ENULL, exit);
	}

	len = ENV_SIZE(ctx);
	env_io = (env_io_t *)malloc(len);
	if (!env_io) {
		ERROR("Can't alloc %ld bytes\n", len);
		EXIT_PREP(ret, -EINTERNAL, exit);
	}

	if (env_io_read(&ctx->cfg, env_io, len, 0)) {
		ERROR("Can't read environment from flash\n");
		EXIT_PREP(ret, -EUNKNOWN, exit);
	}

	crc = crc_32((const unsigned char *)env_io->data, ENV_DATA_SIZE(ctx));
	if (crc != env_io->crc) {
		ERROR("Bad environment CRC\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	char *name = (char *)env_io->data;
	while (*name) {
		char *eq = strchr((char *)name, ENV_KEY_VALUE_SEP);
		if (eq) {
			*eq = '\0';
			char *value = eq + 1;
			len = strlen(value) + 1;
			if (env_set(ctx, name, value)) {
				ERROR("Can't set variable with name %s\n", name);
				EXIT_PREP(ret, -EINVALIDSTATE, exit);
			}
			name = value + len;
		} else {
			ERROR("There should be '%c' after name %s\n", ENV_KEY_VALUE_SEP, name);
			EXIT_PREP(ret, -EINVALIDPARAM, exit);
		}
	}

	char *flags = env_get(ctx, ENV_FLAGS_VAR);
	if (!flags)
		goto exit;

	flags_copy = strdup(flags);
	if (!flags_copy) {
		ERROR("Can't duplicate string '%s'\n", flags);
		EXIT_PREP(ret, -EUNKNOWN, exit);
	}

	name = (char *)flags_copy;
	while (*name) {
		char *eq = strchr((char *)name, ENV_ATTR_SEP);
		if (eq) {
			*eq = '\0';
			char *attr = eq + 1;
			eq = strchr(attr, ENV_ATTR_LIST_DELIM);
			if (eq) {
				*eq = '\0';
				len = strlen(attr) + 1;
			} else {
				len = strlen(attr);
			}

			if (env_set_flag(ctx, name, attr)) {
				ERROR("Can't set flag with name %s\n", name);
				EXIT_PREP(ret, -EINVALIDSTATE, exit);
			}
			name = attr + len;
		} else {
			ERROR("There should be '%c' after name %s\n", ENV_ATTR_SEP, name);
			EXIT_PREP(ret, -EINVALIDPARAM, exit);
		}
	}

exit:
	if (env_io)
		free(env_io);

	if (flags_copy)
		free(flags_copy);

	return ret;
}

int env_export(env_ctx_t *ctx)
{
	uint32_t len;
	int ret = 0;
	env_io_t *env_io = NULL;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		EXIT_PREP(ret, -ENULL, exit);
	}

	len = ENV_SIZE(ctx);
	env_io = (env_io_t *)malloc(len);
	if (!env_io) {
		ERROR("Can't alloc %ld bytes\n", len);
		EXIT_PREP(ret, -EINTERNAL, exit);
	}

	if (env_prepare_io_ctx(ctx, env_io, len)) {
		ERROR("Failed to add key=value to IO environment context\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	if (env_io_clear(&ctx->cfg)) {
		ERROR("Failed to erase IO environment storage\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	if (env_io_write(&ctx->cfg, env_io, len, 0)) {
		ERROR("Failed to save IO environment context\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

exit:
	if (env_io)
		free(env_io);

	return ret;
}

int env_move(env_ctx_t *dest, env_ctx_t *src, bool invalidate)
{
	if (!src) {
		ERROR("The `src` environment context is NULL\n");
		return -ENULL;
	}

	if (env_destroy(dest)) {
		ERROR("Can't destroy dest ctx\n");
		return -EINVALIDSTATE;
	}

	dest->ekv_count = src->ekv_count;
	dest->ekv = src->ekv;

	dest->fkv_count = src->fkv_count;
	dest->fkv = src->fkv;
	if (env_export(dest)) {
		ERROR("Can't export dest ctx\n");
		return -EINVALIDSTATE;
	}

	if (invalidate && env_io_invalidate(&src->cfg))
		ERROR("Failed to invalidate IO environment storage\n");

	env_io_deinit(&src->cfg);

	src->ekv_count = 0;
	src->ekv = NULL;

	src->fkv_count = 0;
	src->fkv = NULL;

	return 0;
}

int env_copy(env_ctx_t *dest, env_ctx_t *src)
{
	uint32_t len;
	int ret = 0;
	env_io_t *env_io = NULL;
	uint32_t crc;

	if (!dest || !src) {
		ERROR("The `dest` or/and `src` environment contexts are NULL\n");
		return -ENULL;
	}

	len = ENV_SIZE(dest);
	env_io = (env_io_t *)malloc(len);
	if (!env_io) {
		ERROR("Can't alloc %ld bytes\n", len);
		EXIT_PREP(ret, -EINTERNAL, exit);
	}

	if (env_prepare_io_ctx(dest, env_io, len)) {
		ERROR("Failed to add key=value to IO environment context\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	crc = env_io->crc;

	if (src->ekv)
		for (int i = 0; i < src->ekv_count; ++i) {
			char *key = src->ekv[i].key;
			char *value = src->ekv[i].value;
			if (key && value && env_set(dest, key, value)) {
				ERROR("Can't set variable %s\n", key);
				EXIT_PREP(ret, -EINVALIDSTATE, exit);
			}
		}

	if (src->fkv)
		for (int i = 0; i < src->fkv_count; ++i) {
			char *key = src->fkv[i].key;
			char *value = src->fkv[i].value;
			if (key && value && env_set_flag(dest, key, value)) {
				ERROR("Can't add flags for %s\n", key);
				EXIT_PREP(ret, -EINVALIDSTATE, exit);
			}
		}

	if (env_prepare_io_ctx(dest, env_io, len)) {
		ERROR("Failed to add key=value to IO environment context\n");
		EXIT_PREP(ret, -EINVALIDSTATE, exit);
	}

	if (crc != env_io->crc) {
		if (env_io_clear(&dest->cfg)) {
			ERROR("Failed to erase IO environment storage\n");
			EXIT_PREP(ret, -EINVALIDSTATE, exit);
		}

		if (env_io_write(&dest->cfg, env_io, len, 0)) {
			ERROR("Failed to save IO environment context\n");
			EXIT_PREP(ret, -EINVALIDSTATE, exit);
		}
	}

exit:
	if (env_io)
		free(env_io);

	return ret;
}

int env_invalidate(env_ctx_t *ctx)
{
	if (!ctx) {
		ERROR("The environment context is NULL\n");
		return -ENULL;
	}

	if (env_io_invalidate(&ctx->cfg)) {
		ERROR("Failed to invalidate IO environment storage\n");
		return -EINVALIDSTATE;
	}

	return 0;
}

int env_print(env_ctx_t *ctx, const char *name)
{
	int len = 0;

	if (!ctx) {
		ERROR("Environment context is NULL\n");
		return -ENULL;
	}

	if (name) {
		char *value = env_get(ctx, name);
		if (value)
			len += printf("%s=%s\n", name, value);
	} else if (ctx->ekv) {
		for (int i = 0; i < ctx->ekv_count; ++i)
			if (ctx->ekv[i].key && ctx->ekv[i].value && ctx->ekv[i].key[0] != '.')
				len += printf("%s=%s\n", ctx->ekv[i].key, ctx->ekv[i].value);
	}

	return len;
}
