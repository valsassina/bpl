// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Assertions.

#include <bpl/macros.hpp>

#if defined(NDEBUG)
#define BPL_DEBUG_ASSERT_I_(...)  \
	do {                          \
		if (false) {              \
			(void) (__VA_ARGS__); \
		}                         \
	} while (false)
#else
#define BPL_DEBUG_ASSERT_I_(...)            \
	do {                                    \
		if (BPL_UNLIKELY(!(__VA_ARGS__))) { \
			BPL_BREAKPOINT();               \
			BPL_FAIL_FAST();                \
		}                                   \
	} while (false)
#endif

// clang-format off
#if defined(NDEBUG)
#include <cstdio>
#define BPL_ASSERT_I_(...)                                                                                                  \
	do {                                                                                                                    \
		if (BPL_UNLIKELY(!(__VA_ARGS__))) {                                                                                 \
			(void) std::fputs(                                                                                                     \
				__FILE__ ":" BPL_TOKEN_TO_STRING(__LINE__) ": Runtime assertion failed: `" #__VA_ARGS__ "` in function: `", \
				stderr                                                                                                      \
			);                                                                                                              \
			(void) std::fputs(__PRETTY_FUNCTION__, stderr);                                                                        \
			(void) std::fputs("`\n", stderr);                                                                                      \
			BPL_FAIL_FAST();                                                                                                \
		}                                                                                                                   \
	} while (false)
#else
#define BPL_ASSERT_I_(...)                  \
	do {                                    \
		if (BPL_UNLIKELY(!(__VA_ARGS__))) { \
			BPL_BREAKPOINT();               \
			BPL_FAIL_FAST();                \
		}                                   \
	} while (false)
#endif
// clang-format on

/// Asserts that a boolean expression is `true` at runtime.
///
/// @note This statement is enabled only if `NDEBUG` is not defined.
#define BPL_DEBUG_ASSERT(...) BPL_DEBUG_ASSERT_I_(__VA_ARGS__)

/// Asserts that a boolean expression is `true` at runtime.
#define BPL_ASSERT(...) BPL_ASSERT_I_(__VA_ARGS__)
