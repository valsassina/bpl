// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Sorting algorithms.

#include <bpl/function_objects.hpp>
#include <bpl/ranges.hpp>

#include <utility>

#include <cstddef>

namespace bpl {

template<typename T, typename F = bpl::less>
constexpr auto is_sorted(T* data, size_t count, F&& compare = {}) -> bool {
	for (size_t i = 1; i < count; ++i) {
		if (std::forward<F>(compare)(data[i], data[i - 1])) {
			return false;
		}
	}
	return true;
}

template<typename T, typename F = bpl::less>
constexpr void selection_sort(T* data, size_t count, F&& compare = {}) {
	for (size_t i = 0; i < count; ++i) {
		size_t min_idx = i;
		for (size_t j = i + 1; j < count; ++j) {
			if (std::forward<F>(compare)(data[j], data[min_idx])) {
				min_idx = j;
			}
		}
		std::swap(data[i], data[min_idx]);
	}
}

template<typename T, typename F = bpl::less>
constexpr void insertion_sort(T* data, size_t count, F&& compare = {}) {
	for (size_t i = 1; i < count; ++i) {
		for (size_t j = i; j > 0 && std::forward<F>(compare)(data[j], data[j - 1]); j--) {
			std::swap(data[j], data[j - 1]);
		}
	}
}

template<typename T, typename F = bpl::less>
constexpr auto lomuto_partition(T* data, size_t count, F&& compare = {}) -> size_t {
	if (count <= 1) {
		return 0;
	}
	const size_t pivot = count - 1;
	size_t partition_point = 0;
	for (size_t i = 0; i < pivot; ++i) {
		if (std::forward<F>(compare)(data[i], data[pivot])) {
			std::swap(data[i], data[partition_point]);
			partition_point += 1;
		}
	}
	std::swap(data[pivot], data[partition_point]);
	return partition_point;
}

template<typename T, typename F = bpl::less>
constexpr void quicksort(T* data, size_t count, F&& compare = {}) {
	while (count > 1) {
		size_t p = lomuto_partition(data, count, std::forward<F>(compare));

		// Recurse on the smaller partition
		size_t right_size = count - (p + 1);
		if (p <= right_size) {
			quicksort(data, p, std::forward<F>(compare));
			data += p + 1;
			count -= p + 1;
		} else {
			quicksort(data + p + 1, right_size, std::forward<F>(compare));
			count = p;
		}
	}
}

template<typename T, typename F = bpl::less>
constexpr void quicksort_naive(T* data, size_t count, F&& compare = {}) {
	if (count > 1) {
		size_t p = lomuto_partition(data, count, std::forward<F>(compare));
		quicksort(data, p, std::forward<F>(compare));
		quicksort(data + p + 1, count - (p + 1), std::forward<F>(compare));
	}
}

/// @name Ranges API
/// @{

/// Checks if a range is sorted.
///
/// @param compare A function with the following signature: `bool compare(T const& x, T const& y)`.
template<contiguous_range R, typename F = bpl::less>
constexpr auto is_sorted(R&& range, F&& compare = {}) {
	return is_sorted(bpl::data(range), bpl::size(range), std::forward<F>(compare));
}

/// Sorts a range using the selection sort algorithm.
///
/// @param compare A function with the following signature: `bool compare(T const& x, T const& y)`.
template<contiguous_range R, typename F = bpl::less>
constexpr void selection_sort(R&& range, F&& compare = {}) {
	selection_sort(bpl::data(range), bpl::size(range), std::forward<F>(compare));
}

/// Sorts a range using the insertion sort algorithm.
///
/// @param compare A function with the following signature: `bool compare(T const& x, T const& y)`.
template<contiguous_range R, typename F = bpl::less>
constexpr void insertion_sort(R&& range, F&& compare = {}) {
	insertion_sort(bpl::data(range), bpl::size(range), std::forward<F>(compare));
}

/// Sorts a range using the quicksort algorithm.
///
/// @param compare A function with the following signature: `bool compare(T const& x, T const& y)`.
template<contiguous_range R, typename F = bpl::less>
constexpr void quicksort(R&& range, F&& compare = {}) {
	quicksort(bpl::data(range), bpl::size(range), std::forward<F>(compare));
}

/// Sorts a range using the bubble sort algorithm.
///
/// @param compare A function with the following signature: `bool compare(T const& x, T const& y)`.
template<contiguous_range R, typename F = bpl::less>
constexpr void bubble_sort(R&& range, F&& compare = {}) {
	bubble_sort(bpl::data(range), bpl::size(range), std::forward<F>(compare));
}

/// @}

} // namespace bpl
