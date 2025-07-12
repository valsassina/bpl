// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Literals.

#include <bpl/assert.hpp>
#include <bpl/bit.hpp>
#include <bpl/math.hpp>

#include <cstddef>
#include <cstdint>

namespace bpl {

namespace detail {

constexpr auto shl_abort_on_overflow(size_t x, uint32_t amount) -> size_t {
	size_t result = bpl::strict_shl(x, amount);
	BPL_ASSERT((x != 0 && result != 0) or (x == 0 && result == 0));
	return result;
}

} // namespace detail

inline namespace literals {

/// @name Binary prefixes (IEC 60027-2)
// @{

constexpr auto operator""_KiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 10);
}

constexpr auto operator""_MiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 20);
}

constexpr auto operator""_GiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 30);
}

constexpr auto operator""_TiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 40);
}

constexpr auto operator""_PiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 50);
}

constexpr auto operator""_EiB(unsigned long long x) -> size_t {
	return bpl::detail::shl_abort_on_overflow(x, 60);
}

/// @}

/// @name SI prefixes
/// @{

constexpr auto operator""_kB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000ull);
}

constexpr auto operator""_MB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000'000ull);
}

constexpr auto operator""_GB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000'000'000ull);
}

constexpr auto operator""_TB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000'000'000'000ull);
}

constexpr auto operator""_PB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000'000'000'000'000ull);
}

constexpr auto operator""_EB(unsigned long long x) -> size_t {
	return bpl::strict_mul(x, 1'000'000'000'000'000'000ull);
}

/// @}

} // namespace literals

} // namespace bpl
