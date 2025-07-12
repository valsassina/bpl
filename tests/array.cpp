// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/array.hpp>
#include <bpl/tags.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <forward_list>
#include <memory>
#include <numeric>
#include <span>
#include <vector>

#include <cstddef>

TEST(Array, constructDefault) {
	bpl::Array<int> array;
	static_assert(sizeof(array) == 3 * sizeof(void*));

	EXPECT_TRUE(array.empty());
	EXPECT_EQ(array.data(), nullptr);
	EXPECT_EQ(array.capacity(), 0);
	EXPECT_EQ(array.size(), 0);
}

TEST(Array, constructCountElements) {
	bpl::Array<int> array(42, 42);

	EXPECT_EQ(array.size(), 42);
	size_t count = 0;
	for (const int& element : array) {
		ASSERT_EQ(element, 42);
		count += 1;
	}
	EXPECT_EQ(count, 42);
}

TEST(Array, constructFromRange) {
	std::forward_list<int> data(42);
	std::iota(data.begin(), data.end(), 42);
	bpl::Array<int> array(bpl::from_range, data);
	EXPECT_TRUE(std::ranges::equal(data, array));
}

TEST(Array, constructFromContiguousRange) {
	std::vector<int> data(42);
	std::iota(data.begin(), data.end(), 42);
	bpl::Array<int> array(bpl::from_range, data);
	EXPECT_TRUE(std::ranges::equal(data, array));
}

TEST(Array, reserve) {
	bpl::Array<int> array;
	array.reserve(42);
	EXPECT_GE(array.capacity(), 42);
}

TEST(Array, clear) {
	bpl::Array<int> array(42);
	array.clear();
	EXPECT_TRUE(array.empty());
}

TEST(Array, resizeUninit) {
	bpl::Array<int> array;
	array.resize_uninit(42);
	EXPECT_EQ(array.size(), 42);

	array.resize_uninit(84);
	EXPECT_EQ(array.size(), 84);

	for (auto& element : array) {
		std::construct_at(std::addressof(element), 42);
	}

	array.resize_uninit(42);
	EXPECT_EQ(array.size(), 42);
}

TEST(Array, resize) {
	bpl::Array<int> array;
	array.resize(42, 42);
	EXPECT_EQ(array.size(), 42);
	EXPECT_EQ(std::ranges::count(array, 42), 42);

	array.resize(84, 42);
	EXPECT_EQ(array.size(), 84);
	EXPECT_EQ(std::ranges::count(array, 42), 84);

	array.resize(42);
	EXPECT_EQ(array.size(), 42);
	EXPECT_EQ(std::ranges::count(array, 42), 42);
}

TEST(Array, assignCount) {
	{
		bpl::Array<int> array;
		array.assign(42, 42);
		EXPECT_EQ(array.size(), 42);
		EXPECT_EQ(std::ranges::count(array, 42), 42);
	}
	{
		bpl::Array<int> array(84);
		array.assign(42, 42);
		EXPECT_EQ(array.size(), 42);
		EXPECT_EQ(std::ranges::count(array, 42), 42);
	}
	{
		bpl::Array<int> array(21);
		array.assign(42, 42);
		EXPECT_EQ(array.size(), 42);
		EXPECT_EQ(std::ranges::count(array, 42), 42);
	}
}

TEST(Array, assignRange) {
	std::array data = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
	{
		bpl::Array<int> array;
		array.assign(data);
		EXPECT_TRUE(std::ranges::equal(array, data));
	}
	{
		bpl::Array<int> array(data.size() * 2);
		array.assign(data);
		EXPECT_TRUE(std::ranges::equal(array, data));
	}
	{
		bpl::Array<int> array(data.size() / 2);
		array.assign(data);
		EXPECT_TRUE(std::ranges::equal(array, data));
	}
}

TEST(Array, append) {
	{
		bpl::Array<int> array;
		array.append(42);
		EXPECT_EQ(array.size(), 1);
		EXPECT_EQ(array[0], 42);
	}
	{
		bpl::Array<int> array(42, 42);
		array.reserve(43);
		array.append(42);
		EXPECT_EQ(array.size(), 43);
		EXPECT_EQ(std::ranges::count(array, 42), 43);
	}
	{
		bpl::Array<int> array(42, 42);
		size_t count = array.capacity();
		array.assign(count, 42);
		array.append(42);
		EXPECT_EQ(array.size(), count + 1);
		EXPECT_EQ(std::ranges::count(array, 42), count + 1);
	}
}

TEST(Array, appendN) {
	{
		bpl::Array<int> array;
		array.append_n(42, 42);
		EXPECT_EQ(array.size(), 42);
		EXPECT_EQ(std::ranges::count(array, 42), 42);
	}
	{
		bpl::Array<int> array(42, 42);
		array.reserve(84);
		array.append_n(42, 42);
		EXPECT_EQ(array.size(), 84);
		EXPECT_EQ(std::ranges::count(array, 42), 84);
	}
	{
		bpl::Array<int> array(42, 42);
		size_t count = array.capacity();
		array.assign(count, 42);
		array.append_n(42, 42);
		EXPECT_EQ(array.size(), count + 42);
		EXPECT_EQ(std::ranges::count(array, 42), count + 42);
	}
}

TEST(Array, appendRange) {
	std::array data_tmp = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
	std::forward_list data(data_tmp.begin(), data_tmp.end());
	{
		bpl::Array<int> array;
		array.append_range(data);
		EXPECT_EQ(array.size(), data_tmp.size());
		EXPECT_TRUE(std::ranges::equal(array, data));
	}
	{
		bpl::Array<int> array(42, 42);
		array.append_range(data);
		EXPECT_EQ(array.size(), 42 + data_tmp.size());
		EXPECT_TRUE(std::ranges::equal(std::span(array).subspan(42), data));
	}
	{
		bpl::Array<int> array(42, 42);
		size_t count = array.capacity();
		array.assign(count, 42);
		array.append_range(data);
		EXPECT_EQ(array.size(), count + data_tmp.size());
		EXPECT_TRUE(std::ranges::equal(std::span(array).subspan(count), data));
	}
}

TEST(Array, appendContiguousRange) {
	std::array data = { 0, 1, 2, 4, 5, 6, 7, 8, 9 };
	{
		bpl::Array<int> array;
		array.append_range(data);
		EXPECT_EQ(array.size(), data.size());
		EXPECT_TRUE(std::ranges::equal(array, data));
	}
	{
		bpl::Array<int> array(42, 42);
		array.append_range(data);
		EXPECT_EQ(array.size(), 42 + data.size());
		EXPECT_TRUE(std::ranges::equal(std::span(array).subspan(42), data));
	}
	{
		bpl::Array<int> array(42, 42);
		size_t count = array.capacity();
		array.assign(count, 42);
		array.append_range(data);
		EXPECT_EQ(array.size(), count + data.size());
		EXPECT_TRUE(std::ranges::equal(std::span(array).subspan(count), data));
	}
}

// TEST(Array, insert) {
// }

// TEST(Array, insertRange) {
// }

// TEST(Array, pop) {
// }

// TEST(Array, remove) {
// }
