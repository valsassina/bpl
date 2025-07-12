// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/ring_buffer.hpp>

#include <gtest/gtest.h>

#include <array>

#include <cstddef>

TEST(RingBuffer, pushAndPop) {
	const std::array array = { 1, 2, 3, 4, 5 };
	bpl::RingBuffer<int> rb(array.size());
	for (const int& element : array) {
		ASSERT_TRUE(rb.push(element));
	}
	for (const int& element : array) {
		EXPECT_EQ(element, rb.pop());
	}
}
