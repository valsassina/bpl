// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Allocators.

#include <bpl/assert.hpp>
#include <bpl/bit.hpp>
#include <bpl/math.hpp>
#include <bpl/memory.hpp>
#include <bpl/os.hpp>

#include <concepts>

#include <cstddef>
#include <cstdlib>

namespace bpl {

/// @name Concepts
/// @{

// clang-format off

template<typename A>
concept Allocator =
	std::movable<A>
	&& requires(A& allocator, size_t size, size_t alignment, MemoryBlock block) {
	{ allocator.allocate(size, alignment) } -> std::same_as<MemoryBlock>;
	{ allocator.deallocate(block, alignment) };
};

template<typename A>
concept GrowableAllocator =
	Allocator<A>
	&& requires(A& allocator, MemoryBlock block, size_t alignment, size_t additional) {
		{ allocator.try_grow(block, alignment, additional) } -> std::same_as<MemoryBlock>;
	};

template<typename A>
concept ShrinkableAllocator =
	Allocator<A>
	&& requires(A& allocator, MemoryBlock block, size_t alignment, size_t new_size) {
		{ allocator.try_shrink(block, alignment, new_size) } -> std::same_as<bool>;
	};

template<typename A>
concept ResizableAllocator = GrowableAllocator<A> && ShrinkableAllocator<A>;

// clang-format on

/// @}

/// @name Stateless allocators
/// @{

/// A stateless allocator that uses `aligned_alloc`.
struct GlobalAllocator {
	/// @name Allocator API
	/// @{

	static auto allocate(size_t size, size_t alignment) -> MemoryBlock {
		BPL_DEBUG_ASSERT(alignment % 2 == 0);
		// From malloc(3): "aligned_alloc() returns a NULL pointer [...] if alignment is not a power of 2 at least as large as sizeof(void*)"
		alignment = bpl::max(sizeof(void*), alignment);
		size_t aligned_size = align_forward(size, alignment);
		void* ptr = std::aligned_alloc(alignment, aligned_size);
		BPL_ASSERT(ptr != nullptr);
		return { .ptr = ptr, .size = aligned_size };
	}

	static void deallocate(MemoryBlock block, size_t /*alignment*/) { std::free(block.ptr); }

	/// @}
};

/// A stateless allocator that uses `mmap`.
struct PagesAllocator {
	/// @name Allocator API
	/// @{

	/// @pre
	///   - `alignment` is less or equal to the page size
	static auto allocate(size_t size, size_t alignment) -> MemoryBlock {
		BPL_DEBUG_ASSERT(alignment <= get_page_size());
		MemoryBlock block = reserve_memory(size);
		BPL_ASSERT(try_commit_memory(block));
		return block;
	}

	static void deallocate(MemoryBlock block, size_t /*alignment*/) { BPL_ASSERT(try_release_memory(block)); }

	/// @}
};

/// @}

} // namespace bpl
