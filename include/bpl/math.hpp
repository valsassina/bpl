// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Common mathematical functions.

#include <bpl/assert.hpp>
#include <bpl/function_objects.hpp>
#include <bpl/macros.hpp>

#include <concepts>
#include <limits>
#include <optional>

namespace bpl {

template<std::integral T>
struct div_rem_result {
	T quotient;
	T remainder;
};

/// Computes the minimum of `x` and `y`.
template<typename T, typename F = bpl::less>
constexpr auto min(const T& x, const T& y, F&& compare = {}) -> T {
	return std::forward<F>(compare)(y, x) ? y : x;
}

/// Computes the maximum of `x` and `y`.
template<typename T, typename F = bpl::less>
constexpr auto max(const T& x, const T& y, F&& compare = {}) -> T {
	return std::forward<F>(compare)(y, x) ? x : y;
}

/// If `x` is within `[ lo, hi ]`, returns `x`, otherwise the nearest boundary.
template<typename T, typename F = bpl::less>
constexpr auto clamp(const T& lo, const T& x, const T& hi, F&& compare = {}) -> T {
	BPL_DEBUG_ASSERT(!(std::forward<F>(compare)(hi, lo)));
	return bpl::min(bpl::max(lo, x, std::forward<F>(compare)), hi, std::forward<F>(compare));
}

/// Returns a reference the minimum of `x` and `y`.
template<typename T, typename F = bpl::less>
constexpr auto min_ref(BPL_LIFETIMEBOUND const T& x, BPL_LIFETIMEBOUND const T& y, F&& compare = {}) -> const T& {
	return std::forward<F>(compare)(y, x) ? y : x;
}

/// Returns a reference the maximum of `x` and `y`.
template<typename T, typename F = bpl::less>
constexpr auto max_ref(BPL_LIFETIMEBOUND const T& x, BPL_LIFETIMEBOUND const T& y, F&& compare = {}) -> const T& {
	return std::forward<F>(compare)(y, x) ? x : y;
}

/// If `x` is within `[ lo, hi ]`, returns a reference to `x`, otherwise a reference to the nearest boundary.
template<typename T, typename F = bpl::less>
constexpr auto
clamp_ref(BPL_LIFETIMEBOUND const T& lo, BPL_LIFETIMEBOUND const T& x, BPL_LIFETIMEBOUND const T& hi, F&& compare = {})
	-> const T& {
	BPL_DEBUG_ASSERT(!(std::forward<F>(compare)(hi, lo)));
	return bpl::min_ref(bpl::max_ref(lo, x, std::forward<F>(compare)), hi, std::forward<F>(compare));
}

/// Computes the quotient and remainder of `x / y`.
template<std::integral T>
constexpr auto div_rem(T x, T y) -> div_rem_result<T> {
	return { .quotient = x / y, .remainder = x % y };
}

/// Computes `x + y`, returning `std::nullopt` if overflow occurred.
template<std::integral T>
constexpr auto checked_add(T x, T y) -> std::optional<T> {
	auto sum = std::make_optional<T>();
	bool overflow = __builtin_add_overflow(x, y, &*sum);
	if (overflow) {
		sum = std::nullopt;
	}
	return sum;
}

/// Computes `x + y`, aborting if overflow occurred.
template<std::integral T>
constexpr auto strict_add(T x, T y) -> T {
	auto sum = checked_add(x, y);
	BPL_ASSERT(sum.has_value());
	return *sum;
}

/// Computes `x - y`, returning `std::nullopt` if overflow occurred.
template<std::integral T>
constexpr auto checked_sub(T x, T y) -> std::optional<T> {
	auto difference = std::make_optional<T>();
	bool overflow = __builtin_sub_overflow(x, y, &*difference);
	if (overflow) {
		difference = std::nullopt;
	}
	return difference;
}

/// Computes `x - y`, aborting if overflow occurred.
template<std::integral T>
constexpr auto strict_sub(T x, T y) -> T {
	auto difference = checked_sub(x, y);
	BPL_ASSERT(difference.has_value());
	return *difference;
}

/// Computes `x * y`, returning `std::nullopt` if overflow occurred.
template<std::integral T>
constexpr auto checked_mul(T x, T y) -> std::optional<T> {
	auto product = std::make_optional<T>();
	bool overflow = __builtin_mul_overflow(x, y, &*product);
	if (overflow) {
		product = std::nullopt;
	}
	return product;
}

/// Computes `x * y`, aborting if overflow occurred.
template<std::integral T>
constexpr auto strict_mul(T x, T y) -> T {
	auto product = checked_mul(x, y);
	BPL_ASSERT(product.has_value());
	return *product;
}

/// Computes `x / y`, returning `nullopt` if `y == 0`.
template<std::integral T>
constexpr auto checked_div(T x, T y) -> std::optional<T> {
	if (y == 0) {
		return {};
	}
	return x / y;
}

/// Computes `x / y`, aborting if `y == 0`.
template<std::integral T>
constexpr auto strict_div(T x, T y) -> T {
	auto quotient = checked_div(x, y);
	BPL_ASSERT(quotient.has_value());
	return *quotient;
}

/// Computes the saturating addition `x + y`.
template<std::integral T>
constexpr auto saturating_add(T x, T y) -> T {
	if (auto sum = checked_add(x, y)) {
		return *sum;
	}
	if constexpr (std::unsigned_integral<T>) {
		return std::numeric_limits<T>::max();
	} else {
		if (x > 0) {
			return std::numeric_limits<T>::max();
		}
		return std::numeric_limits<T>::min();
	}
}

} // namespace bpl
