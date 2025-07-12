// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/assert.hpp>
#include <bpl/memory.hpp>

#include <unistd.h>

#include <version>

#if defined(__cpp_lib_hardware_interference_size)
#include <new>
#endif

#include <cstddef>

namespace bpl {

/// The size of the L1 data cache line, in bytes.
#if defined(__cpp_lib_hardware_interference_size)
static constexpr size_t CACHE_LINE_SIZE = std::hardware_constructive_interference_size;
#elif defined(__APPLE__) && defined(__aarch64__) // Apple silicon
static constexpr size_t CACHE_LINE_SIZE = 128;
#else
constexpr size_t CACHE_LINE_SIZE = 64;
#endif

namespace detail {

inline auto get_page_size_impl() -> size_t {
	const long page_size = sysconf(_SC_PAGE_SIZE);
	BPL_ASSERT(page_size >= 0);
	return static_cast<size_t>(page_size);
}

} // namespace detail

/// Returns the size of a page in bytes.
///
/// @note The value is retrieved once and then cached.
inline auto get_page_size() -> size_t {
	static const size_t page_size = bpl::detail::get_page_size_impl();
	return page_size;
}

/// Tries to allocate enough pages to fit `size` bytes, aborting if the operation fails.
///
/// @pre
///   - `bytes > 0`.
///
/// @returns A `MemoryBlock` containing the allocated pages.
[[nodiscard]]
auto reserve_memory(size_t size) -> MemoryBlock;

/// Tries to commit a previously reserved block of memory.
///
/// @returns `true` if successful.
[[nodiscard]]
auto try_commit_memory(MemoryBlock block) -> bool;

/// Tries to decommit a block of memory.
///
/// @returns `true` if successful.
[[nodiscard]]
auto try_decommit_memory(MemoryBlock block) -> bool;

/// Tries to release a block of memory.
///
/// @returns `true` if successful.
[[nodiscard]]
auto try_release_memory(MemoryBlock block) -> bool;

} // namespace bpl
