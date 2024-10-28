// SPDX-License-Identifier: MIT
// Copyright 2023 RnD Center "ELVEES", JSC

#ifndef __ENV_H__
#define __ENV_H__

#include "env_io.h"

typedef struct {
	char *key;
	char *value;
} env_kv_t;

typedef struct {
	env_io_config_t cfg;
	int count;
	env_kv_t *ekv;
} env_ctx_t;

/**
 * env_init() - Initialize env library
 *
 * @ctx:    Instance of env context
 * @offset: Offset of env
 * @size:   Size of env
 * Return: Return: 0 if OK, -1 on error
 */
int env_init(env_ctx_t *ctx, signed long offset, size_t size);

/**
 * env_get() - Look up the value of an environment variable
 *
 * @ctx:   Instance of env context
 * @name:  Variable to look up
 * Return: value of variable, or NULL if not found
 */
char *env_get(env_ctx_t *ctx, const char *name);

/**
 * env_set() - Set an environment variable
 *
 * This sets or updates the value of an environment variable. For setting the
 * value the variable is created if it does not already exist.
 *
 * @ctx:   Instance of env context
 * @name:  Variable to adjust
 * @value: Value to set for the variable
 * Return: 0 if OK, -1 on error
 */
int env_set(env_ctx_t *ctx, const char *name, const char *value);

/**
 * env_import() - Import the environment from storage
 *
 * @ctx:   Instance of env context
 * Return: 0 if OK, -1 on error
 */
int env_import(env_ctx_t *ctx);

/**
 * env_export() - Export the environment to storage
 *
 * @ctx:   Instance of env context
 * Return: 0 if OK, -1 on error
 */
int env_export(env_ctx_t *ctx);

/**
 * env_destroy() - Destroy loaded environment
 *
 * @ctx:   Instance of env context
 * Return: 0 if OK, -1 on error
 */
int env_destroy(env_ctx_t *ctx);

/**
 * env_print() - Print environment variables
 *
 * @ctx:    Instance of env context
 * @name:   Variable to print, if NULL all variables are printed
 * Returns: -1 in case of error, or length of printed strings
 */
int env_print(env_ctx_t *ctx, char *name);

/**
 * env_move() - Move env to another context
 *
 * The command moves env from src context to dest context.
 * At the same time src context is droped to init state.
 * After that src ctx should not be destroyed.
 * IO settings of dest aren't changed.
 *
 * @dest:  Destination context
 * @src:   Context to be moved
 * Return: 0 if OK, -1 on error
 */
int env_move(env_ctx_t *dest, env_ctx_t *src);

/**
 * env_invalidate() - Invalidate environment storage
 *
 * @ctx:   Instance of env context
 * Return: 0 if OK, -1 on error
 */
int env_invalidate(env_ctx_t *ctx);

#endif /* __ENV_H__ */
