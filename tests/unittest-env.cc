// SPDX-License-Identifier: MIT
// Copyright 2025 RnD Center "ELVEES", JSC

#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <gtest/gtest.h>
#include <libs/env/env-io-ram.h>
#include <libs/env/env.h>
#include <libs/errors.h>

static void modify_foo_bar(env_ctx_t *ctx)
{
	char *foo;
	char *bar;

	foo = env_get(ctx, "foo");
	bar = env_get(ctx, "bar");
	if (foo && bar && strcmp(foo, bar) != 0) {
		char *baz = env_get(ctx, "baz");
		if (baz) {
			// baz exist - set foo to bar, clean baz
			env_set(ctx, "foo", bar);
			env_set(ctx, "baz", nullptr);
		} else {
			// NO baz - set baz to true
			env_set(ctx, "baz", "true");
		}
	}
}

TEST(EnvTests, check_params)
{
	env_ctx_t ctx;
	char *str;
	int ret;

	ret = env_init(nullptr, 0, 0, ENV_IO_UNKNOWN);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_deinit(nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

	ret = env_init(&ctx, 0, 0, ENV_IO_UNKNOWN);
	GTEST_ASSERT_EQ(ret, -ENOTSUPPORTED);

	ret = env_init(&ctx, 0, 0, ENV_IO_RAM);
	GTEST_ASSERT_EQ(ret, 0);

	ret = env_deinit(&ctx);
	GTEST_ASSERT_EQ(ret, 0);

	ret = env_import(nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_export(nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_move(nullptr, nullptr, true);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_move(nullptr, nullptr, false);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_move(nullptr, &ctx, true);
	GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

	ret = env_move(nullptr, &ctx, false);
	GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

	ret = env_move(&ctx, nullptr, true);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_move(&ctx, nullptr, false);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_invalidate(nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_print(nullptr, nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_set(nullptr, nullptr, nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_set(&ctx, nullptr, nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_set(&ctx, ",", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	ret = env_set(&ctx, ":", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	ret = env_set(&ctx, "=", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	ret = env_set_flag(nullptr, nullptr, nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_set_flag(&ctx, nullptr, nullptr);
	GTEST_ASSERT_EQ(ret, -ENULL);

	ret = env_set_flag(&ctx, ",", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	ret = env_set_flag(&ctx, ":", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	ret = env_set_flag(&ctx, "=", nullptr);
	GTEST_ASSERT_EQ(ret, -EINVALIDPARAM);

	str = env_get_key_first(nullptr);
	GTEST_ASSERT_EQ(str, (char *)nullptr);

	str = env_get_flag_key_first(nullptr);
	GTEST_ASSERT_EQ(str, (char *)nullptr);
}

TEST(EnvTests, check_import_export)
{
	size_t sz = 16;
	int ret;

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

		ret = env_set(&ctx, "key",
		              "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVYZ");
		GTEST_ASSERT_EQ(ret, 0);

		// Should be greater then 0 as we add one variable
		ret = env_print(&ctx, nullptr);
		GTEST_ASSERT_GT(ret, 0);

		// Variable should not be written to storage as resulting len is greater then buffer
		// size
		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		// Should be 0 as we don't have any value in storage
		ret = env_print(&ctx, nullptr);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_set(&ctx, "key", "0123");
		GTEST_ASSERT_EQ(ret, 0);

		// Should be greater then 0 as we add one variable
		ret = env_print(&ctx, nullptr);
		GTEST_ASSERT_GT(ret, 0);

		// Variable should be written to storage as resulting len is less then buffer size
		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *key = env_get(&ctx, "key");
		GTEST_ASSERT_NE(key, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(key, "0123"), 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		// Should be greater then 0 as we add one variable
		ret = env_print(&ctx, nullptr);
		GTEST_ASSERT_GT(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_set_remove)
{
	env_ctx_t ctx;
	size_t sz = 64 * 1024;
	int ret;

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Create env with 64K size
	{
		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Check that foo, bar, baz don't exist
	{
		char *foo = env_get(&ctx, "foo");
		GTEST_ASSERT_EQ(foo, (char *)nullptr);

		char *bar = env_get(&ctx, "bar");
		GTEST_ASSERT_EQ(bar, (char *)nullptr);

		char *baz = env_get(&ctx, "baz");
		GTEST_ASSERT_EQ(baz, (char *)nullptr);
	}

	// SET foo=true, bar=true
	{
		ret = env_set(&ctx, "foo", "true");
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_set(&ctx, "bar", "true");
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		char *bar = env_get(&ctx, "bar");
		GTEST_ASSERT_NE(bar, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(bar, "true"), 0);
	}

	// SET baz=true, bar=false
	{
		ret = env_set(&ctx, "baz", "true");
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_set(&ctx, "bar", "false");
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		char *baz = env_get(&ctx, "baz");
		GTEST_ASSERT_NE(baz, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(baz, "true"), 0);

		char *bar = env_get(&ctx, "bar");
		GTEST_ASSERT_NE(bar, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(bar, "false"), 0);
	}

	// DELETE baz
	{
		ret = env_set(&ctx, "baz", nullptr);
		GTEST_ASSERT_EQ(ret, 0);

		char *baz = env_get(&ctx, "baz");
		GTEST_ASSERT_EQ(baz, (char *)nullptr);
	}

	// Check foo != bar without baz, baz will be added
	{
		modify_foo_bar(&ctx);
		char *baz = env_get(&ctx, "baz");
		GTEST_ASSERT_NE(baz, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(baz, "true"), 0);
	}

	// Check foo != bar with baz,  baz will be removed
	{
		modify_foo_bar(&ctx);
		char *baz = env_get(&ctx, "baz");
		GTEST_ASSERT_EQ(baz, (char *)nullptr);
	}

	// Deinit
	{
		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_move)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io_to_move = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io_to_move.get(), (char *)nullptr);

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Prepare env
	{
		env_ctx_t ctx_to_move;

		ret = env_init(&ctx_to_move, (signed long)env_io_to_move.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_set(&ctx_to_move, "foo", "true");
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_move, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_export(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Move without invalidation
	{
		env_ctx_t ctx_to_move;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_move, (signed long)env_io_to_move.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_move(&ctx, &ctx_to_move, false);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_move, "foo");
		GTEST_ASSERT_EQ(foo, (char *)nullptr);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Import after move without invalidation
	{
		env_ctx_t ctx_to_move;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_move, (signed long)env_io_to_move.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_move, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_deinit(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Move with invalidation
	{
		env_ctx_t ctx_to_move;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_move, (signed long)env_io_to_move.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_move(&ctx, &ctx_to_move, true);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_move, "foo");
		GTEST_ASSERT_EQ(foo, (char *)nullptr);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Import after move with invalidation
	{
		env_ctx_t ctx_to_move;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_move, (signed long)env_io_to_move.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_move, "foo");
		GTEST_ASSERT_EQ(foo, (char *)nullptr);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_deinit(&ctx_to_move);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_copy)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io_to_copy = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io_to_copy.get(), (char *)nullptr);

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Prepare env
	{
		env_ctx_t ctx_to_copy;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_set(&ctx_to_copy, "foo", "true");
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_copy, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_export(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Copy env
	{
		env_ctx_t ctx_to_copy;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_copy(&ctx, &ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_copy, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Import after copy
	{
		env_ctx_t ctx_to_copy;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *foo = env_get(&ctx_to_copy, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		foo = env_get(&ctx, "foo");
		GTEST_ASSERT_NE(foo, (char *)nullptr);
		GTEST_ASSERT_EQ(strcmp(foo, "true"), 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_invalidate)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Prepare env
	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		for (int i = 0; i < 100; ++i) {
			std::string value = std::to_string(i);
			std::string key = "foo" + value;
			ret = env_set(&ctx, key.c_str(), value.c_str());
			GTEST_ASSERT_EQ(ret, 0);
		}

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		env_invalidate(&ctx);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, -EINVALIDSTATE);

		char *key = env_get_key_first(&ctx);
		GTEST_ASSERT_EQ(key, (char *)nullptr);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_flags)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Prepare env
	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		for (int i = 0; i < 100; ++i) {
			std::string value = std::to_string(i);
			std::string key = "foo" + value;
			ret = env_set(&ctx, key.c_str(), value.c_str());
			GTEST_ASSERT_EQ(ret, 0);

			ret = env_set_flag(&ctx, key.c_str(), "so");
			GTEST_ASSERT_EQ(ret, 0);
		}

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	{
		env_ctx_t ctx;

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *flags = env_get(&ctx, ".flags");
		GTEST_ASSERT_NE(flags, (char *)nullptr);

		{
			char *key = env_get_key_first(&ctx);
			while (key) {
				char *value = env_get(&ctx, key);
				GTEST_ASSERT_NE(value, (char *)nullptr);

				ret = env_set(&ctx, key, nullptr);
				GTEST_ASSERT_EQ(ret, 0);

				key = env_get_key_next(&ctx);
			}

			key = env_get_key_first(&ctx);
			GTEST_ASSERT_EQ(key, (char *)nullptr);

			key = env_get_key_next(&ctx);
			GTEST_ASSERT_EQ(key, (char *)nullptr);
		}

		{
			char *key = env_get_flag_key_first(&ctx);
			while (key) {
				char *value = env_get_flag(&ctx, key);
				GTEST_ASSERT_EQ(strcmp(value, "so"), 0);

				ret = env_set_flag(&ctx, key, nullptr);
				GTEST_ASSERT_EQ(ret, 0);

				key = env_get_flag_key_next(&ctx);
			}

			key = env_get_flag_key_first(&ctx);
			GTEST_ASSERT_EQ(key, (char *)nullptr);

			key = env_get_flag_key_next(&ctx);
			GTEST_ASSERT_EQ(key, (char *)nullptr);
		}

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_copy_with_flags)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io_to_copy = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io_to_copy.get(), (char *)nullptr);

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	// Prepare env
	{
		env_ctx_t ctx_to_copy;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		for (int i = 0; i < 100; ++i) {
			std::string value = std::to_string(i);
			std::string key = "foo" + value;
			ret = env_set(&ctx_to_copy, key.c_str(), value.c_str());
			GTEST_ASSERT_EQ(ret, 0);

			ret = env_set_flag(&ctx_to_copy, key.c_str(), "so");
			GTEST_ASSERT_EQ(ret, 0);
		}

		ret = env_export(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Copy env
	{
		env_ctx_t ctx_to_copy;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_copy(&ctx, &ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		char *flags_to_copy = env_get(&ctx_to_copy, ".flags");
		GTEST_ASSERT_NE(flags_to_copy, (char *)nullptr);

		char *flags = env_get(&ctx, ".flags");
		GTEST_ASSERT_NE(flags, (char *)nullptr);

		GTEST_ASSERT_EQ(strcmp(flags_to_copy, flags), 0);

		ret = env_export(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}

	// Import after copy
	{
		env_ctx_t ctx_to_copy;
		env_ctx_t ctx;

		ret = env_init(&ctx_to_copy, (signed long)env_io_to_copy.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_import(&ctx);
		GTEST_ASSERT_EQ(ret, 0);

		char *flags_to_copy = env_get(&ctx_to_copy, ".flags");
		GTEST_ASSERT_NE(flags_to_copy, (char *)nullptr);

		char *flags = env_get(&ctx, ".flags");
		GTEST_ASSERT_NE(flags, (char *)nullptr);

		GTEST_ASSERT_EQ(strcmp(flags_to_copy, flags), 0);

		ret = env_deinit(&ctx_to_copy);
		GTEST_ASSERT_EQ(ret, 0);

		ret = env_deinit(&ctx);
		GTEST_ASSERT_EQ(ret, 0);
	}
}

TEST(EnvTests, check_set_hex)
{
	size_t sz = 64 * 1024;
	int ret;

	auto env_io = std::make_unique<char[]>(sz);
	GTEST_ASSERT_NE(env_io.get(), (char *)nullptr);

	env_ctx_t ctx;

	ret = env_init(&ctx, (signed long)env_io.get(), sz, ENV_IO_RAM);
	GTEST_ASSERT_EQ(ret, 0);

	ret = env_set_hex(&ctx, "hex", 16);
	GTEST_ASSERT_EQ(ret, 0);

	char *hex = env_get(&ctx, "hex");
	GTEST_ASSERT_NE(hex, (char *)nullptr);

	env_print(&ctx, "hex");

	GTEST_ASSERT_EQ(strcmp(hex, "0x10"), 0);

	ret = env_deinit(&ctx);
	GTEST_ASSERT_EQ(ret, 0);
}
