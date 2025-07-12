// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Implementation of various algorithms.

#include <bpl/assert.hpp>
#include <bpl/function_objects.hpp>
#include <bpl/math.hpp>
#include <bpl/ranges.hpp>
#include <bpl/span.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace bpl {

/// Swaps the elements of `r1` and `r2`.
template<range R1, range R2>
constexpr void swap_ranges(R1&& r1, R2&& r2) {
	auto r1_it = bpl::begin(r1);
	auto r2_it = bpl::begin(r2);
	for (; r1_it != bpl::end(r1); ++r1_it, ++r2_it) {
		bpl::iter_swap(r1_it, r2_it);
	}
}

/// @name Partitioning operations
/// @{

/// Returns the index of the first element of the second subset if it exists; otherwise `span.size()`.
template<typename T, typename P>
constexpr auto partition(Span<T> span, P&& predicate) -> size_t {
	// Find the first element that doesn't match the predicate
	size_t p = 0;
	for (; p < span.size(); ++p) {
		if (!std::forward<P>(predicate)(span[p])) {
			break;
		}
	}
	if (p == span.size()) {
		return p;
	}

	for (size_t q = p + 1; q < span.size(); ++q) {
		if (std::forward<P>(predicate)(span[q])) {
			std::swap(span[q], span[p]);
			p += 1;
		}
	}
	return p;
}

/// @}

/// @name Binary search operations
/// @{

template<typename T, typename F = bpl::less>
constexpr auto lower_bound(Span<T> span, const T& x, F&& compare = {}) -> size_t {
	auto len = span.size();
	size_t base = 0;
	while (len != 0) {
		const auto half = len / 2;
		const auto mid = base + half;

		if (std::forward<F>(compare)(span[mid], x)) {
			base = mid + 1;
			len -= half + 1;
		} else {
			len = half;
		}
	}
	return base;
}

template<typename T, typename F = bpl::less>
constexpr auto upper_bound(Span<const T> span, const T& x, F&& compare = {}) -> size_t {
	size_t len = span.size();
	size_t base = 0;
	while (len != 0) {
		const size_t half = len / 2;
		const size_t mid = base + half;
		if (std::forward<F>(compare)(x, span[mid])) {
			base = mid + 1;
			len -= half + 1;
		} else {
			len = half;
		}
	}
	return base;
}

template<typename T, typename F = bpl::less>
constexpr auto binary_search(Span<T> span, const T& x, F&& compare = {}) -> size_t {
	size_t i = lower_bound(span, x, std::forward<F>(compare));
	if (i < span.size() && (!std::forward<F>(compare)(i, x) && !std::forward<F>(compare)(x, i))) {
		return i;
	}
	return span.size();
}

/// @}

/// @name Order changing operations
/// @{

template<typename T>
constexpr void reverse(Span<T> span) {
	if (span.empty()) {
		return;
	}

	size_t left = 0;
	size_t right = span.size() - 1;
	while (left < right) {
		std::swap(span[left], span[right]);
		left += 1;
		right -= 1;
	}
}

template<typename Iterator>
constexpr void rotate_left(Iterator first, Iterator middle, Iterator last) {
	using distance_type = std::iter_difference_t<Iterator>;
	using unsigned_distance_type = std::make_unsigned_t<distance_type>;

	if (first == middle or last == middle) {
		return;
	}

	distance_type n = std::distance(first, last);
	distance_type k = std::distance(first, middle);
	if (k == n - k) {
		swap_ranges(Span(first, middle), Span(middle, last));
		return;
	}

	Iterator p = first;
	for (;;) {
		if (k < n - k) { // left side < right side
			distance_type q = p + k;
			for (distance_type i = 0; i < n - k; ++i) {
				std::swap(*p, *q);
				++p;
				++q;
			}
			n = static_cast<unsigned_distance_type>(n) % static_cast<unsigned_distance_type>(k);
			if (n == 0) {
				break;
			}
			std::swap(n, k);
			k = n - k;
		} else { // left side >= right side
			k = n - k;
			auto q = p + n;
			p = q - k;
			for (distance_type i = 0; i < n - k; ++i) {
				--p;
				--q;
				std::swap(*p, *q);
			}
			n = static_cast<unsigned_distance_type>(n) % static_cast<unsigned_distance_type>(k);
			if (n == 0) {
				break;
			}
			std::swap(n, k);
		}
	}
}

/// @}

/// @name Pattern search
/// @{

/// @}

} // namespace bpl
