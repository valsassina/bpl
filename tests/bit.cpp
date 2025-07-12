// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/bit.hpp>

#include <gtest/gtest.h>

#include <climits>
#include <cstdint>
#include <cstring>

TEST(bit, bitsOf) {
	static_assert(bpl::bits_of<int8_t> == 8);
	static_assert(bpl::bits_of<int16_t> == 16);
	static_assert(bpl::bits_of<int32_t> == 32);
	static_assert(bpl::bits_of<int64_t> == 64);

	static_assert(bpl::bits_of<uint8_t> == 8);
	static_assert(bpl::bits_of<uint16_t> == 16);
	static_assert(bpl::bits_of<uint32_t> == 32);
	static_assert(bpl::bits_of<uint64_t> == 64);
}

TEST(bit, toUnsigned) {
	for (const int& x : { INT_MIN, 0, INT_MAX }) {
		const unsigned int y = bpl::to_unsigned(x);
		EXPECT_EQ(std::memcmp(&x, &y, sizeof(int)), 0);
	}
}

TEST(bit, toSigned) {
	for (const unsigned int& x : { 0u, UINT_MAX }) {
		const int y = bpl::to_signed(x);
		EXPECT_EQ(std::memcmp(&x, &y, sizeof(unsigned int)), 0);
	}
}
