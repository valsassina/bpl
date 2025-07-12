// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#define BPL_TOKEN_TO_STRING_POST_EXPANSION_I_(_TOKEN) #_TOKEN
#define BPL_TOKEN_TO_STRING(_TOKEN)                   BPL_TOKEN_TO_STRING_POST_EXPANSION_I_(_TOKEN)

#if defined(__has_builtin)
#define BPL_HAS_BUILTIN(x) __has_builtin(x)
#else
#define BPL_HAS_BUILTIN(x) false
#endif

#if defined(__has_include)
#define BPL_HAS_INCLUDE(x) __has_include(x)
#else
#define BPL_HAS_INCLUDE(x) false
#endif

#if defined(__has_attribute)
#define BPL_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#define BPL_HAS_ATTRIBUTE false
#endif

#if defined(__has_cpp_attribute)
#define BPL_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define BPL_HAS_ATTRIBUTE false
#endif

#define BPL_INLINE_ALWAYS [[gnu::always_inline]] inline
#define BPL_INLINE_NEVER  [[gnu::noinline]]

#define BPL_LIKELY(x)   __builtin_expect(x, true)
#define BPL_UNLIKELY(x) __builtin_expect(x, false)

#define BPL_FAIL_FAST() __builtin_trap()

#if BPL_HAS_BUILTIN(__builtin_debugtrap)
#define BPL_BREAKPOINT_I_() __builtin_debugtrap()
#elif defined(__i386__) || defined(__x86_64__)
// nop is for GDB
#define BPL_BREAKPOINT_I_() __asm__ volatile("int $0x3\n\tnop")
#elif defined(__arch64__)
#define BPL_BREAKPOINT_I_() __asm__ volatile(".inst 0xd4200000")
#elif defined(__arm__)
#define BPL_BREAKPOINT_I_() __asm__ volatile(".inst 0xe7f001f0");
#else
#define BPL_BREAKPOINT_I_() __builtin_trap()
#endif
#define BPL_BREAKPOINT() BPL_BREAKPOINT_I_()

#if BPL_HAS_CPP_ATTRIBUTE(clang::lifetimebound)
#define BPL_LIFETIMEBOUND [[clang::lifetimebound]]
#elif BPL_HAS_ATTRIBUTE(lifetimebound)
#define BPL_LIFETIMEBOUND __attribute__((lifetimebound))
#else
#define BPL_LIFETIMEBOUND
#endif
