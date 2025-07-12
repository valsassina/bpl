// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/doubly_linked_list.hpp>

#include <gtest/gtest.h>

#include <iterator>
#include <ranges>

TEST(DoublyLinkedList, concepts) {
	static_assert(std::forward_iterator<bpl::DoublyLinkedListIterator<int>>);
	static_assert(std::ranges::forward_range<bpl::DoublyLinkedList<int>>);
}

TEST(DoublyLinkedList, rangeBasedForLoop) {
	bpl::DoublyLinkedList<int> list(10u);
	{
		int k = 0;
		for (int& node_data : list) {
			node_data = k;
			k += 1;
		}
	}
	{
		int k = 0;
		for (const int& node_data : list) {
			EXPECT_EQ(node_data, k);
			k += 1;
		}
		EXPECT_EQ(k, 10u);
	}
}
