// SPDX-License-Identifier: MIT
// Copyright 2023-2025 RnD Center "ELVEES", JSC

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>

#include "env-io.h"

typedef struct {
	char *key;
	char *value;
} kv_t;

typedef struct {
	int next;
} kv_iter_t;

typedef struct {
	env_io_config_t cfg;
	int ekv_count;
	kv_t *ekv;
	kv_iter_t eit;
	int fkv_count;
	kv_t *fkv;
	kv_iter_t fit;
} env_ctx_t;

/**
 * @brief Initialize environment library
 *
 * @param ctx      - Instance of environment context
 * @param offset   - Offset of environment
 * @param size     - Size of environment
 * @param location - Where environment is located
 *
 * @return  0             - Success,
 *         -ENULL         - ctx param is not provided (NULL pointers),
 *         -ENOTSUPPORTED - location is not supported
 */
int env_init(env_ctx_t *ctx, signed long offset, size_t size, env_io_location_t location);

/**
 * @brief Deinitialize environment library
 *
 * @param ctx - Instance of environment context
 *
 * @return  0             - Success,
 *         -EINVALIDSTATE - Could not destroy ctx
 */
int env_deinit(env_ctx_t *ctx);

/**
 * @brief Look up the value of an environment variable
 *
 * @param ctx  - Instance of environment context
 * @param name - Variable to look up
 *
 * @return The value of variable, or NULL if not found
 */
char *env_get(env_ctx_t *ctx, const char *name);

/**
 * @brief Look up the first key of an environment variable.
 *        The iterator context is stored in instance context ctx.
 *        If found key is started with '.' symbol, the lookup is continue.
 *
 * @param ctx - Instance of environment context
 *
 * @return The first key of variable, or NULL if not found
 */
char *env_get_key_first(env_ctx_t *ctx);

/**
 * @brief Look up the next key of an environment variable.
 *        The iterator context is stored in instance context ctx.
 *        If found key is started with '.' symbol, the lookup is continue.
 *
 * @param ctx - Instance of environment context
 *
 * @return The next key of variable, or NULL if not found
 */
char *env_get_key_next(env_ctx_t *ctx);

/**
 * @brief Set an environment variable
 *        This sets or updates the value of an environment variable.
 *        In case of setting a value the variable is created if it does not already exist.
 *
 * @param ctx   - Instance of environment context
 * @param name  - Variable to adjust
 * @param value - Value to set for the variable
 *
 * @return  0             - Success,
 *         -ENULL         - ctx or name params are not provided (NULL pointers),
 *         -EINVALIDSTATE - Function could not delete or insert item,
 *         -EINVALIDPARAM - Illegal character in name param
 */
int env_set(env_ctx_t *ctx, const char *name, const char *value);

/**
 * Set an environment variable to an value in hex
 *
 * @param ctx   - Instance of environment context
 * @param name  - Variable to adjust
 * @param value - Value to set for the variable
 *
 * @return See return values of env_set function
 */
int env_set_hex(env_ctx_t *ctx, const char *name, unsigned long long value);

/**
 * @brief Look up the flag attribute of an environment variable
 *
 * @param ctx  - Instance of environment context
 * @param name - Variable to look up
 *
 * @return The flag attribute of an environment variable, or NULL if not found
 */
char *env_get_flag(env_ctx_t *ctx, const char *name);

/**
 * @brief Look up the first flag key of an environment variable.
 *        The iterator context is stored in instance context ctx.
 *
 * @param ctx - Instance of environment context
 *
 * @return The first flag key of an environment variable, or NULL if not found
 */
char *env_get_flag_key_first(env_ctx_t *ctx);

/**
 * @brief Look up the next flag key of an environment variable.
 *        The iterator context is stored in instance context ctx.
 *
 * @param ctx - Instance of environment context
 *
 * @return The next flag key of an environment variable, or NULL if not found
 */
char *env_get_flag_key_next(env_ctx_t *ctx);

/**
 * @brief Set the flag of an environment variable.
 *        This sets or updates the flag of an environment variable.
 *        The variable '.flags' is created if it does not already exist.
 *
 * @param ctx  - Instance of environment context
 * @param name - Variable to adjust
 * @param attr - Variable access attr
 *
 * @return  0             - Success,
 *         -ENULL         - ctx or name params are not provided (NULL pointers),
 *         -EINVALIDSTATE - Function could not delete or insert item or set variable,
 *         -EINVALIDPARAM - Illegal character in name param or count of flags in ctx is zero,
 *         -EINTERNAL     - Function could not allocate mem in heap
 */
int env_set_flag(env_ctx_t *ctx, const char *name, const char *attr);

/**
 * @brief Import the environment from storage
 *
 * @param ctx - Instance of environment context
 *
 * @return  0             - Success,
 *         -ENULL         - ctx param is not provided (NULL pointers),
 *         -EINTERNAL     - Function could not allocate mem in heap,
 *         -EUNKNOWN      - Function could not read env from IO location or duplicate string,
 *         -EINVALIDSTATE - Bad environment CRC or function could not set variable or set flag,
 *         -EINVALIDPARAM - Illegal character in name,
 */
int env_import(env_ctx_t *ctx);

/**
 * @brief Export the environment to storage
 *
 * @param ctx - Instance of environment context
 *
 * @return  0             - Success,
 *         -ENULL         - ctx param is not provided (NULL pointers),
 *         -EINTERNAL     - Function could not allocate mem in heap,
 *         -EINVALIDSTATE - Function could not prepare IO or erase IO env storage or
 *                          save IO env context
 */
int env_export(env_ctx_t *ctx);

/**
 * @brief Move environment to another context.
 *        The command moves environment from a src context to a dest context.
 *        At the same time the src context is dropped to init state and can be
 *        invalidated. IO settings of dest aren't changed. The dest context
 *        is exported to IO environment storage.
 *
 * @param dest       - Destination context
 * @param src        - Context to be moved
 * @param invalidate - Invalidate src context
 *
 * @return  0             - Success,
 *         -ENULL         - src param is not provided (NULL pointers),
 *         -EINVALIDSTATE - Function could not destroy or export destination ctx
 */
int env_move(env_ctx_t *dest, env_ctx_t *src, bool invalidate);

/**
 * @brief Copy environment to another context.
 *        The command copies src context to dest context.
 *        Wherein if CRC32 of a dest context is not changed,
 *        latter is not exported to IO environment storage.
 *
 * @param dest - Destination context
 * @param src  - Context to be copied
 *
 * @return  0             - Success,
 *         -ENULL         - dest or src params are not provided (NULL pointers),
 *         -EINTERNAL     - Function could not allocate mem in heap,
 *         -EINVALIDSTATE - Function could not prepare IO env ctx or add flags or
 *                          add key=value to IO env ctx or erase IO env storage or
 *                          save IO env context
 */
int env_copy(env_ctx_t *dest, env_ctx_t *src);

/**
 * @brief Invalidate environment storage
 *
 * @param ctx - Instance of env context
 *
 * @return  0             - Success,
 *         -EINVALIDSTATE - Function could not invalidate IO env storage
 */
int env_invalidate(env_ctx_t *ctx);

/**
 * @brief Print environment variables
 *
 * @param ctx  - Instance of environment context
 * @param name - Variable to print, if NULL all variables are printed
 *
 * @return -ENULL when ctx param is not provided (NULL pointers), or length of printed strings
 */
int env_print(env_ctx_t *ctx, const char *name);

#ifdef __cplusplus
}
#endif
