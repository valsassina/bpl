// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/math.hpp>

#include <gtest/gtest.h>

#include <climits>

TEST(math, checkedAdd) {
	EXPECT_EQ(bpl::checked_add(2, 2), 4);
	EXPECT_FALSE(bpl::checked_add(INT_MAX, 1).has_value());
	EXPECT_FALSE(bpl::checked_add(INT_MIN, -1).has_value());
}

TEST(math, checkedSub) {
	EXPECT_EQ(bpl::checked_sub(2, 2), 0);
	EXPECT_FALSE(bpl::checked_sub(INT_MIN, 1).has_value());
	EXPECT_FALSE(bpl::checked_sub(INT_MAX, -1).has_value());
}

TEST(math, checkedMul) {
	EXPECT_EQ(bpl::checked_mul(2, 3), 6);
	EXPECT_FALSE(bpl::checked_mul((INT_MAX / 2) + 1, 2).has_value());
	EXPECT_FALSE(bpl::checked_mul((INT_MIN / 2) - 1, 2).has_value());
}

TEST(math, checkedDiv) {
	EXPECT_EQ(bpl::checked_div(10, 2), 5);
	EXPECT_FALSE(bpl::checked_div(10, 0).has_value());
}

TEST(math, saturatingAdd) {
	EXPECT_EQ(bpl::saturating_add(3, 5), 8);
	EXPECT_EQ(bpl::saturating_add(3, INT_MAX - 1), INT_MAX);
	EXPECT_EQ(bpl::saturating_add(INT_MAX, INT_MAX), INT_MAX);
	EXPECT_EQ(bpl::saturating_add(INT_MAX - 2, 1), INT_MAX - 1);
}
