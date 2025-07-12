// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/linked_list.hpp>

#include <gtest/gtest.h>

#include <iterator>
#include <ranges>

#include <cstddef>

TEST(LinkedList, concepts) {
	static_assert(std::forward_iterator<bpl::LinkedListIterator<int>>);
	static_assert(std::ranges::forward_range<bpl::LinkedList<int>>);
}

TEST(LinkedList, rangeBasedForLoop) {
	bpl::LinkedList<int> list(10u, 77);
	size_t i = 0;
	for (const auto& x : list) {
		EXPECT_EQ(x, 77);
		i += 1;
	}
	EXPECT_EQ(i, 10u);
}
