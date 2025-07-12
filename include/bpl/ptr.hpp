// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Utilities for working with raw pointers.

#include <bpl/bit.hpp>

#include <bit>

#include <cstdint>

namespace bpl {

/// Reinterprets the bits of a pointer as a `uintptr_t`.
template<typename T>
constexpr auto ptr_to_addr(T* ptr) -> uintptr_t {
	return std::bit_cast<uintptr_t>(ptr);
}

/// Reinterprets the bits of an address as a pointer.
template<typename T>
constexpr auto addr_to_ptr(uintptr_t addr) -> T* {
	return std::bit_cast<T*>(addr);
}

/// Computes the offset that needs to be applied to `ptr` in order to make it aligned to `alignment`.
///
/// @pre
///   - `alignment` is a multiple of `sizeof(T)`.
template<typename T>
constexpr auto ptr_align_offset(T* ptr, size_t alignment) -> size_t {
	uintptr_t addr = ptr_to_addr(ptr);
	uintptr_t aligned_addr = align_forward(addr, alignment);
	size_t offset = aligned_addr - addr;
	BPL_DEBUG_ASSERT(offset % sizeof(T) == 0);
	return offset / sizeof(T);
}

/// Aligns a pointer to `alignment`.
///
/// @pre
///   - `alignment` is a multiple of `sizeof(T)`.
template<typename T>
constexpr auto ptr_align(T* ptr, size_t alignment) -> T* {
	T* result = ptr + ptr_align_offset(ptr, alignment);
	return BPL_ASSUME_ALIGNED(alignment, result);
}

/// Aligns a pointer to the alignment of `U`.
///
/// @pre
///   - `alignment` is a multiple of `sizeof(T)`.
template<typename U, typename T>
constexpr auto ptr_align_to(T* ptr) -> U* {
	static_assert(alignof(U) % sizeof(T) == 0);
	return ptr_align(ptr, alignof(U));
}

} // namespace bpl
