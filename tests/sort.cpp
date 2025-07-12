// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/function_objects.hpp>
#include <bpl/sort.hpp>

#include <gtest/gtest.h>

#include <array>

TEST(sort, emptyInput) {
	std::array<int, 0> x = {};
	EXPECT_TRUE(bpl::is_sorted(x));
	bpl::selection_sort(x);
	bpl::insertion_sort(x);
	bpl::quicksort(x);
}

TEST(sort, sortedInput) {
	const std::array x = { 1, 2, 3, 4, 5 };
	{
		std::array copy = x;
		bpl::selection_sort(copy);
		ASSERT_EQ(x, copy);
	}
	{
		std::array copy = x;
		bpl::insertion_sort(copy);
		ASSERT_EQ(x, copy);
	}
	{
		std::array copy = x;
		bpl::quicksort(copy);
		ASSERT_EQ(x, copy);
	}
}

TEST(sort, unsortedInput) {
	const std::array x = { 4, 3, 8, 1, 0 };
	EXPECT_FALSE(bpl::is_sorted(x));
	{
		std::array copy = x;
		bpl::selection_sort(copy);
		EXPECT_TRUE(bpl::is_sorted(copy));
	}
	{
		std::array copy = x;
		bpl::insertion_sort(copy);
		EXPECT_TRUE(bpl::is_sorted(copy));
	}
	{
		std::array copy = x;
		bpl::quicksort(copy);
		EXPECT_TRUE(bpl::is_sorted(copy));
	}
	{
		std::array copy = { 0, 0, 0, 0, 0 };
		bpl::quicksort(copy);
		EXPECT_TRUE(bpl::is_sorted(copy));
	}
}

TEST(sort, isSorted) {
	{
		const int x[] = { 0, 1, 2, 3, 4 };
		EXPECT_TRUE(bpl::is_sorted(x));
	}
	{
		const int x[] = { 4, 3, 2, 1, 0 };
		EXPECT_FALSE(bpl::is_sorted(x));
		EXPECT_TRUE(bpl::is_sorted(x, bpl::greater{}));
	}
	{
		const int x[] = { 1, 0, 2, 3, 4 };
		EXPECT_FALSE(bpl::is_sorted(x));
	}
}
