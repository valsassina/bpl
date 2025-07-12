// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/span.hpp>
#include <bpl/traits.hpp>

#include <gtest/gtest.h>

#include <cstdint>

TEST(Span, isTrivial) {
	EXPECT_TRUE(bpl::trivially_copyable<bpl::Span<int>>);
	EXPECT_TRUE(bpl::trivially_movable<bpl::Span<int>>);
	EXPECT_TRUE(bpl::trivially_destructible<bpl::Span<int>>);
}
