// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Essential utility functions and classes.

#include <utility>

#include <cstddef>

namespace bpl {

template<typename I1, typename I2>
constexpr void iter_swap(I1 a, I2 b) {
	std::swap(*a, *b);
}

namespace detail {

template<typename I, typename S = I>
class iterator_sentinel {
public:
	iterator_sentinel(I begin, S end) : m_begin(begin), m_end(end) {}

	[[nodiscard]] auto begin() const -> I { return m_begin; }
	[[nodiscard]] auto end() const -> S { return m_end; }

private:
	I m_begin;
	S m_end;
};

} // namespace detail

/// Allows an iterator and a sentinel to be used in a range-based for loop.
template<typename I, typename S = I>
auto make_range(I begin, S end) -> detail::iterator_sentinel<I, S> {
	return detail::iterator_sentinel<I, S>(begin, end);
}

/// Allows a pointer and size to be used in a range-based for loop.
template<typename T>
auto make_range(T* data, size_t count) -> detail::iterator_sentinel<T*, T*> {
	return detail::iterator_sentinel<T*, T*>(data, data + count);
}

/// Indicates that a pointer owns the memory it points to.
///
/// @note Use only in low-level abstractions.
///
/// @tparam T A pointer.
template<typename T>
using owner = T;

} // namespace bpl
