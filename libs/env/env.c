// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <log.h>
#include <lib/crc32/crc32.h>
#include "env.h"

/**************************************************************************
 *
 * The "environment" is stored as a list of '\0' terminated
 * "name=value" strings. The end of the list is marked by a double
 * '\0'. New entries are always added at the end. Deleting an entry
 * shifts the remaining entries to the front. Replacing an entry is a
 * combination of deleting the old value and adding the new one.
 *
 * The environment is preceded by a 32 bit CRC over the data part.
 *
 *************************************************************************/

#define EXIT_PREP(ret, code, label) \
	do {                        \
		(ret) = (code);     \
		goto label;         \
	} while (0)

#define ENV_SIZE      (ctx->cfg.size)
#define ENV_DATA_SIZE (ctx->cfg.size - sizeof(env_t))

typedef struct {
	uint32_t crc; /* CRC32 over data bytes */
	char data[0]; /* Environment data */
} env_t;

static int env_cmp(const void *item1, const void *item2)
{
	env_kv_t *p1 = (env_kv_t *)item1;
	env_kv_t *p2 = (env_kv_t *)item2;
	return strcmp(p1->key, p2->key);
}

static env_kv_t *env_get_item(env_ctx_t *ctx, const char *name)
{
	if (ctx && ctx->ekv && name) {
		for (int i = 0; i < ctx->count; ++i) {
			if (ctx->ekv[i].key && strcmp(ctx->ekv[i].key, name) == 0)
				return &ctx->ekv[i];
		}
	}

	return NULL;
}

static int env_get_item_index(env_ctx_t *ctx, env_kv_t *item)
{
	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	if (item == NULL) {
		ERROR("Pointer to element is NULL\r\n");
		return -1;
	}

	int idx = (int)((ptrdiff_t)item - (ptrdiff_t)ctx->ekv) / sizeof(env_kv_t);

	if (idx < 0 || idx >= ctx->count) {
		ERROR("Wrong element index\r\n");
		return -1;
	}

	return idx;
}

int env_init(env_ctx_t *ctx, signed long offset, size_t size)
{
	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	memset(ctx, 0, sizeof(env_ctx_t));
	env_io_init(&ctx->cfg, offset, size);

	return 0;
}

char *env_get(env_ctx_t *ctx, const char *name)
{
	env_kv_t *ekv = env_get_item(ctx, name);
	if (ekv)
		return ekv->value;

	return NULL;
}

int env_set(env_ctx_t *ctx, const char *name, const char *value)
{
	uint32_t len;
	env_kv_t *ekv;

	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	if (name == NULL) {
		ERROR("Empty variable name\r\n");
		return -1;
	}

	if (strchr(name, '=')) {
		ERROR("Illegal character '=' in name \"%s\"\r\n", name);
		return -1;
	}

	// Delete value
	if (value == NULL) {
		ekv = env_get_item(ctx, name);
		if (ekv) {
			// ekv is a part of ctx->ekv array.
			int idx = env_get_item_index(ctx, ekv);

			len = (ctx->count - 1) * sizeof(env_kv_t);
			ekv = (env_kv_t *)malloc(len);
			if (ekv == NULL) {
				ERROR("Can't alloc %ld bytes\r\n", len);
				return -1;
			}

			if (idx)
				memcpy(&ekv[0], &ctx->ekv[0], idx * sizeof(env_kv_t));
			if (ctx->count - idx - 1)
				memcpy(&ekv[idx], &ctx->ekv[idx + 1],
				       (ctx->count - idx - 1) * sizeof(env_kv_t));

			free(ctx->ekv[idx].key);
			free(ctx->ekv[idx].value);
			free(ctx->ekv);
			ctx->ekv = ekv;
			ctx->count--;
		}
		return 0;
	}

	// Insert / replace new value.
	ekv = env_get_item(ctx, name);
	if (ekv) {
		len = strlen(value) + 1;
		char *new_value = (char *)malloc(len);
		if (new_value == NULL) {
			ERROR("Can't alloc %ld bytes\r\n", len);
			return -1;
		}
		strncpy(new_value, value, len);
		free(ekv->value);
		ekv->value = new_value;
	} else {
		len = (ctx->count + 1) * sizeof(env_kv_t);
		ekv = (env_kv_t *)malloc(len);
		if (ekv == NULL) {
			ERROR("Can't alloc %ld bytes\r\n", len);
			return -1;
		}
		memcpy(ekv, ctx->ekv, ctx->count * sizeof(env_kv_t));

		len = strlen(name) + 1;
		ekv[ctx->count].key = (char *)malloc(len);
		if (ekv[ctx->count].key == NULL) {
			free(ekv);
			ERROR("Can't alloc %ld bytes\r\n", len);
			return -1;
		}
		strncpy(ekv[ctx->count].key, name, len);

		len = strlen(value) + 1;
		ekv[ctx->count].value = (char *)malloc(len);
		if (ekv[ctx->count].value == NULL) {
			free(ekv[ctx->count].key);
			free(ekv);
			ERROR("Can't alloc %ld bytes\r\n", len);
			return -1;
		}
		strncpy(ekv[ctx->count].value, value, len);

		if (ctx->ekv)
			free(ctx->ekv);

		ctx->ekv = ekv;
		ctx->count++;
	}

	return 0;
}

int env_import(env_ctx_t *ctx)
{
	uint32_t len;
	env_t *env = NULL;
	uint32_t crc;
	int ret = 0;

	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		EXIT_PREP(ret, -1, exit);
	}

	len = ENV_SIZE;
	env = (env_t *)malloc(len);
	if (env == NULL) {
		ERROR("Can't alloc %ld bytes\r\n", len);
		EXIT_PREP(ret, -1, exit);
	}

	if (env_io_read(&ctx->cfg, env, len, 0) != 0) {
		ERROR("Can't read env from flash\r\n");
		EXIT_PREP(ret, -1, exit);
	}

	crc = crc_32((const unsigned char *)env->data, ENV_DATA_SIZE);
	if (crc != env->crc) {
		ERROR("Bad CRC env copied from flash\r\n");
		EXIT_PREP(ret, -1, exit);
	}

	char *name = (char *)env->data;
	while (*name) {
		char *eq = strchr((char *)name, '=');
		if (eq) {
			*eq = '\0';
			char *value = eq + 1;
			len = strlen(value) + 1;
			if (env_set(ctx, name, value) < 0) {
				ERROR("Can't set variable with name %s\r\n", name);
				EXIT_PREP(ret, -1, exit);
			}
			name = value + len;
		} else {
			ERROR("There should be '=' after name %s\r\n", name);
			EXIT_PREP(ret, -1, exit);
		}
	}

exit:
	if (env)
		free(env);

	return ret;
}

int env_export(env_ctx_t *ctx)
{
	uint32_t off = 0U;
	uint32_t len;
	int ret = 0;
	env_t *env = NULL;

	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		EXIT_PREP(ret, -1, exit);
	}

	len = ENV_SIZE;
	env = (env_t *)malloc(len);
	if (env == NULL) {
		ERROR("Can't alloc %ld bytes\r\n", len);
		EXIT_PREP(ret, -1, exit);
	}

	if (ctx->count)
		qsort(ctx->ekv, ctx->count, sizeof(*ctx->ekv), env_cmp);

	if (ctx->ekv) {
		for (int i = 0; i < ctx->count; ++i) {
			char *key = ctx->ekv[i].key;
			char *value = ctx->ekv[i].value;
			if (key && value) {
				int n = snprintf(NULL, 0, "%s=%s", key, value);
				if ((off + n + 1) == len) {
					ERROR("Can't add %s=%s to env, no space\r\n", key, value);
					break;
				}
				off += snprintf(&env->data[off], n + 1, "%s=%s", key, value) + 1;
			}
		}
	}
	env->data[off] = '\0';

	env->crc = crc_32((const unsigned char *)env->data, ENV_DATA_SIZE);

	if (env_io_erase(&ctx->cfg)) {
		ERROR("Failed to erase env\r\n");
		EXIT_PREP(ret, -1, exit);
	}

	if (env_io_write(&ctx->cfg, env, len, 0)) {
		ERROR("Failed to save env\r\n");
		EXIT_PREP(ret, -1, exit);
	}

exit:
	if (env)
		free(env);

	return ret;
}

int env_destroy(env_ctx_t *ctx)
{
	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	if (ctx->ekv) {
		for (int i = 0; i < ctx->count; ++i) {
			if (ctx->ekv[i].key && ctx->ekv[i].value) {
				free(ctx->ekv[i].key);
				free(ctx->ekv[i].value);
			}
		}

		free(ctx->ekv);
		ctx->ekv = NULL;
	}
	ctx->count = 0;

	return 0;
}

int env_print(env_ctx_t *ctx, char *name)
{
	int len = 0;

	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	if (name) {
		char *value = env_get(ctx, name);
		if (value)
			len += printf("%s=%s\r\n", name, value);
	} else {
		if (ctx->ekv) {
			for (int i = 0; i < ctx->count; ++i) {
				if (ctx->ekv[i].key && ctx->ekv[i].value)
					len += printf("%s=%s\r\n", ctx->ekv[i].key,
						      ctx->ekv[i].value);
			}
		}
	}

	return len;
}

int env_move(env_ctx_t *dest, env_ctx_t *src)
{
	if (src == NULL) {
		ERROR("Src env context is NULL\r\n");
		return -1;
	}

	if (env_destroy(dest) < 0) {
		ERROR("Can't destroy dest ctx\r\n");
		return -1;
	}

	dest->count = src->count;
	dest->ekv = src->ekv;

	src->count = 0;
	src->ekv = NULL;

	return 0;
}

int env_invalidate(env_ctx_t *ctx)
{
	if (ctx == NULL) {
		ERROR("Env context is NULL\r\n");
		return -1;
	}

	if (env_io_erase(&ctx->cfg)) {
		ERROR("Failed to erase env\r\n");
		return -1;
	}

	return 0;
}
