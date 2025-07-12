// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Utilities for memory management.

#include <bpl/assert.hpp>
#include <bpl/bit.hpp>
#include <bpl/ptr.hpp>
#include <bpl/ranges.hpp>

#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

#include <cstddef>
#include <cstdint>

namespace bpl {

// clang-format off
template<size_t Size>
using smallest_size_type =
    std::conditional_t<Size <= std::numeric_limits<uint8_t>::max(), uint8_t,
    std::conditional_t<Size <= std::numeric_limits<uint16_t>::max(), uint16_t,
    std::conditional_t<Size <= std::numeric_limits<uint32_t>::max(), uint32_t,
    std::conditional_t<Size <= std::numeric_limits<uint64_t>::max(), uint64_t,
    size_t>>>>;
// clang-format on

/// A block of memory.
struct MemoryBlock {
	/// Pointer to the beginning of the block of memory.
	void* ptr;
	/// Size of the block of memory in bytes.
	size_t size;

	constexpr auto operator==(const MemoryBlock&) const -> bool = default;
};

/// Returns `true` if `ptr` is in the range `[ begin, end )`.
template<typename T>
constexpr auto pointer_in_range(const T* begin, const T* ptr, const T* end) -> bool {
	return begin <= ptr && ptr < end;
}

template<typename T>
constexpr auto align_ptr_backward(T* ptr, size_t alignment) -> T* {
	BPL_DEBUG_ASSERT(alignment % sizeof(T) == 0);
	auto addr = ptr_to_addr(ptr);
	const uintptr_t aligned_addr = align_backward(addr, alignment);
	// SAFETY: addr >= aligned_addr
	uintptr_t distance = (addr - aligned_addr) / sizeof(T);
	return ptr - distance;
}

template<typename T>
constexpr auto align_ptr_forward(T* ptr, size_t alignment) -> T* {
	BPL_DEBUG_ASSERT(alignment % sizeof(T) == 0);
	uintptr_t addr = ptr_to_addr(ptr);
	uintptr_t aligned_addr = align_forward(addr, alignment);
	// SAFETY: aligned_addr >= addr
	uintptr_t distance = (aligned_addr - addr) / sizeof(T);
	return ptr + distance;
}

template<typename T>
constexpr void default_construct_at(T* ptr) {
	::new (ptr) T;
}

template<bpl::range R, typename... Args>
constexpr auto construct(R&& range, Args&&... args) -> size_t {
	size_t count = 0;
	for (auto& element : range) {
		std::construct_at(std::addressof(element), std::forward<Args>(args)...);
		count += 1;
	}
	return count;
}

template<bpl::range R1, bpl::range R2>
constexpr auto relocate(R1&& src, R2&& dst) -> size_t {
	size_t count = 0;
	auto src_it = bpl::begin(src);
	auto dst_it = bpl::begin(dst);
	for (; src_it != bpl::end(src) && dst_it != bpl::end(dst); ++src_it, ++dst_it, ++count) {
		std::construct_at(std::addressof(*dst_it), std::move(*src_it));
		std::destroy_at(std::addressof(*src_it));
	}
	return count;
}

template<bpl::random_access_range R1, bpl::random_access_range R2>
constexpr auto relocate_backward(R1&& src, R2&& dst) -> size_t {
	BPL_DEBUG_ASSERT(bpl::size(src) <= bpl::size(dst));
	size_t count = bpl::size(src);
	size_t src_index = bpl::size(src) - 1;
	size_t dst_index = bpl::size(dst) - 1;
	for (size_t i = 0; i < count; ++i) {
		std::construct_at(std::addressof(dst[dst_index - i]), std::move(src[src_index - i]));
		std::destroy_at(std::addressof(src[src_index - i]));
	}
	return count;
}

template<bpl::range R>
constexpr auto destroy(R&& r) -> size_t {
	size_t count = 0;
	for (auto& element : r) {
		std::destroy_at(std::addressof(element));
		count += 1;
	}
	return count;
}

template<bpl::random_access_range R>
constexpr auto destroy_backward(R&& r) -> size_t {
	for (size_t i = bpl::size(r); i-- > 0;) {
		std::destroy_at(std::addressof(r[i]));
	}
	return bpl::size(r);
}

/// Copies the elements from `src` to an uninitialized memory area beginning at `dst`.
///
/// @returns The number of elements copied.
template<bpl::range R1, bpl::range R2>
constexpr auto uninitialized_copy(R1&& src, R2&& dst) -> size_t {
	if constexpr (bpl::sized_range<R1> && bpl::sized_range<R2>) {
		BPL_DEBUG_ASSERT(bpl::size(src) <= bpl::size(dst));
	}
	size_t count = 0;
	{
		auto src_it = bpl::begin(src);
		auto dst_it = bpl::begin(dst);
		for (; src_it != bpl::end(src); ++src_it, ++dst_it, ++count) {
			std::construct_at(std::addressof(*dst_it), *src_it);
		}
	}
	return count;
}

/// Moves the elements from `src` to an uninitialized memory area beginning at `dst`.
///
/// @pre
///   - `size(src) <= size(dst)`.
///
/// @returns The number of elements moved.
template<bpl::range R1, bpl::range R2>
constexpr auto uninitialized_move(R1&& src, R2&& dst) -> size_t {
	if constexpr (bpl::sized_range<R1> && bpl::sized_range<R2>) {
		BPL_DEBUG_ASSERT(bpl::size(src) <= bpl::size(dst));
	}
	size_t count = 0;
	{
		auto src_it = bpl::begin(src);
		auto dst_it = bpl::begin(dst);
		for (; src_it != bpl::end(src); ++src_it, ++dst_it, ++count) {
			std::construct_at(std::addressof(*dst_it), std::move(*src_it));
		}
	}
	return count;
}

} // namespace bpl
