// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Bit manipulation.

#include <bpl/assert.hpp>

#include <bit>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

#include <climits>
#include <cstddef>
#include <cstdint>

namespace bpl {

static_assert(CHAR_BIT == 8, "1 byte != 8 bits.");

/// Returns the size of `T` in bits.
template<std::integral T>
constexpr size_t bits_of = sizeof(T) * 8;

/// Transmutes an integer to an unsigned integer of the same size.
///
/// @returns An unsigned integer with the same bytes and of the same size as `x`.
template<std::integral T>
constexpr auto to_unsigned(T x) -> std::make_unsigned_t<T> {
	return std::bit_cast<std::make_unsigned_t<T>>(x);
}

/// Transmutes an integer to a signed integer of the same size.
///
/// @returns A signed integer with the same bytes and of the same size as `x`.
template<std::integral T>
constexpr auto to_signed(T x) -> std::make_signed_t<T> {
	return std::bit_cast<std::make_signed_t<T>>(x);
}

/// Shifts the bits in `x` to the left by `amount`, i.e., a logical left shift.
///
/// @returns `x << amount` if `amount` is less than the number of bits in `x`; otherwise `std::nullopt`.
template<std::unsigned_integral T>
constexpr auto checked_shl(T x, uint32_t amount) -> std::optional<T> {
	if (amount >= bits_of<T>) {
		return std::nullopt;
	}
	return x << amount;
}

/// Shifts the bits in `x` to the left by `amount`, i.e., a logical left shift.
///
/// Aborts if `amount` is greater or equal to the number of bits in `x`.
template<std::unsigned_integral T>
constexpr auto strict_shl(T x, uint32_t amount) -> T {
	std::optional<T> result = checked_shl(x, amount);
	BPL_ASSERT(result);
	return *result;
}

/// Shifts the bits in `x` to the right by `amount`, i.e., a logical right shift.
///
/// @returns `x << amount` if `amount` is less than the number of bits in `x`; otherwise `std::nullopt`.
template<std::unsigned_integral T>
constexpr auto checked_shr(T x, uint32_t amount) -> std::optional<T> {
	if (amount >= bits_of<T>) {
		return std::nullopt;
	}
	return x >> amount;
}

/// Shifts the bits in `x` to the right by `amount`, i.e., a logical right shift.
///
/// Aborts if `amount` is greater or equal to the number of bits in `x`.
template<std::unsigned_integral T>
constexpr auto strict_shr(T x, uint32_t amount) -> T {
	std::optional<T> result = checked_shr(x, amount);
	BPL_ASSERT(result);
	return *result;
}

/// Checks if `x` is a power of two.
///
/// @returns `true` if `x` is a power of 2.
template<std::unsigned_integral T>
constexpr auto is_pow2(T x) -> bool {
	return std::has_single_bit(x);
}

/// Computes a number that is less or equal to `x` and a multiple of `alignment`.
///
/// @pre
///   - `alignment > 0`.
///   - `alignment` is a power of 2.
template<std::unsigned_integral T>
constexpr auto align_backward(T x, size_t alignment) -> T {
	// SAFETY: If alignment is a power of 2, then it's also != 0.
	BPL_DEBUG_ASSERT(bpl::is_pow2(alignment));

	return x & ~(alignment - 1);
}

/// Computes a number that is greater or equal to `x` and a multiple of `alignment`.
///
/// @pre
///   - `alignment > 0`.
///   - `alignment` is a power of 2.
///   - `x + (alignment - 1)` doesn't overflow.
template<std::unsigned_integral T>
constexpr auto align_forward(T x, size_t alignment) -> T {
	// SAFETY: If alignment is a power of 2, then it's also > 0
	BPL_DEBUG_ASSERT(bpl::is_pow2(alignment));

	// SAFETY: `alignment - 1` doesn't underflow because `alignment > 0`
	BPL_DEBUG_ASSERT(alignment - 1 <= std::numeric_limits<T>::max() - x);

	return align_backward(x + (alignment - 1), alignment);
}

} // namespace bpl
