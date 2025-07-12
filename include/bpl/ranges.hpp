// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// All you need to work with C++ ranges.

#include <bpl/traits.hpp>
#include <bpl/utility.hpp>

#include <concepts>
#include <initializer_list>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace bpl {

/// @name Access
/// @{

template<typename R>
constexpr auto begin(R&& range) -> decltype(range.begin()) {
	return range.begin();
}

template<typename T, size_t N>
constexpr auto begin(T (&array)[N]) -> T* {
	return array;
}

template<typename R>
constexpr auto end(R&& range) -> decltype(range.end()) {
	return range.end();
}

template<typename T, size_t N>
constexpr auto end(T (&array)[N]) -> T* {
	return array + N;
}

template<typename R>
constexpr auto size(R&& range) -> decltype(range.size()) {
	return range.size();
}

template<typename T, size_t N>
constexpr auto size(T (&)[N]) -> size_t {
	return N;
}

template<typename R>
requires requires(R&& range) { bool(range.empty()); }
constexpr auto empty(R&& range) -> bool {
	return bool(range.empty());
}

template<typename R>
constexpr auto empty(R&& range) -> bool {
	return bpl::size(range) == 0;
}

template<typename T, size_t N>
constexpr auto empty(T (&)[N]) -> bool {
	return N == 0;
}

template<typename R>
constexpr auto data(R&& range) -> decltype(range.data()) {
	return range.data();
}

template<typename T, size_t N>
constexpr auto data(T (&array)[N]) -> T* {
	return array;
}

template<typename T>
constexpr auto data(const std::initializer_list<T>& il) -> const T* {
	return il.begin();
}

/// @}

/// @name Primitives
/// @{

template<typename R>
using range_reference_t = decltype(*bpl::begin(std::declval<R>()));

template<typename R>
using range_value_t = std::remove_reference_t<bpl::range_reference_t<R>>;

template<typename R>
using range_ptr_t = decltype(bpl::data(std::declval<R>()));

template<typename I>
using iter_reference_t = decltype(*std::declval<I>());

template<typename I>
using iter_value_t = std::remove_reference_t<decltype(*std::declval<I>())>;

template<typename R>
concept range = requires(R& r) {
	bpl::begin(r);
	bpl::end(r);
};

/// @}

/// @name Concepts
/// @{

template<typename R>
concept sized_range = bpl::range<R> && requires(R& r) { bpl::size(r); };

// clang-format off

template<typename R>
concept random_access_range = bpl::range<R> && requires(R&& r, size_t index) {
	{ r[index] } -> std::same_as<bpl::range_reference_t<R>>;
};

template<typename R>
concept contiguous_range = bpl::sized_range<R> && requires(R& r) {
	{ *bpl::data(r) } -> bpl::reference;
};

// clang-format on

/// @}

template<bpl::range R1, bpl::range R2>
constexpr auto copy(R1&& src, R2&& dst) -> size_t {
	size_t count = 0;
	auto src_it = bpl::begin(src);
	auto dst_it = bpl::begin(dst);
	for (; src_it != bpl::end(src) && dst_it != bpl::end(dst); ++src_it, ++dst_it, ++count) {
		*dst_it = *src_it;
	}
	return count;
}

template<bpl::range R1, bpl::range R2>
constexpr auto copy_n(R1&& src, size_t n, R2&& dst) -> size_t {
	size_t i = 0;
	auto src_it = bpl::begin(src);
	auto dst_it = bpl::begin(dst);
	for (; i != n && src_it != bpl::end(src) && dst_it != bpl::end(dst); ++src_it, ++dst_it, ++i) {
		*dst_it = *src_it;
	}
	return i;
}

template<bpl::range R1, bpl::range R2>
constexpr auto copy_backward(R1&& src, R2&& dst) -> size_t {
	size_t count = 0;
	auto src_it = bpl::end(src);
	auto dst_it = bpl::end(dst);
	for (; src_it != bpl::begin(src) && dst_it != bpl::begin(dst); ++count) {
		--src_it;
		--dst_it;
		*dst_it = *src_it;
	}
	return count;
}

template<bpl::range R1, bpl::range R2>
constexpr auto move(R1&& src, R2&& dst) -> size_t {
	size_t count = 0;
	auto src_it = bpl::begin(src);
	auto dst_it = bpl::begin(dst);
	for (; src_it != bpl::end(src) && dst_it != bpl::end(dst); ++src_it, ++dst_it, ++count) {
		*dst_it = std::move(*src_it);
	}
	return count;
}

template<bpl::range R1, bpl::range R2>
constexpr auto move_backward(R1&& src, R2&& dst) -> size_t {
	size_t count = 0;
	auto src_it = bpl::end(src);
	auto dst_it = bpl::end(dst);
	for (; src_it != bpl::begin(src) && dst_it != bpl::begin(dst); ++count) {
		--src_it;
		--dst_it;
		*dst_it = std::move(*src_it);
	}
	return count;
}

template<bpl::range R1, bpl::range R2>
constexpr auto swap_ranges(R1&& r1, R2&& r2) -> size_t {
	size_t count = 0;
	auto it1 = bpl::begin(r1);
	auto it2 = bpl::begin(r2);
	for (; it1 != bpl::end(r1) && it2 != bpl::end(r2); ++it1, ++it2, ++count) {
		bpl::iter_swap(it1, it2);
	}
	return count;
}

template<bpl::range R, typename T = bpl::range_value_t<R>>
constexpr auto fill(R&& r, const T& x) -> size_t {
	size_t count = 0;
	for (auto& element : r) {
		element = x;
		count += 1;
	}
	return count;
}

/// Checks if two contiguous ranges overlap.
template<bpl::contiguous_range R1, bpl::contiguous_range R2>
constexpr auto overlay(R2&& r1, R2&& r2) -> bool {
	return (ptr_to_addr(bpl::begin(r1)) < ptr_to_addr(bpl::end(r2)))
		&& (ptr_to_addr(bpl::end(r1)) > ptr_to_addr(bpl::begin(r2)));
}

} // namespace bpl
