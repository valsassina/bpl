// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/allocator.hpp>
#include <bpl/arena.hpp>
#include <bpl/memory.hpp>
#include <bpl/utility.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <span>

#include <cstddef>
#include <cstdint>

TEST(Arena, allocatorConcepts) {
	EXPECT_TRUE(bpl::Allocator<bpl::Arena>);
	EXPECT_TRUE(bpl::GrowableAllocator<bpl::Arena>);
	EXPECT_TRUE(bpl::ShrinkableAllocator<bpl::Arena>);
	EXPECT_TRUE(bpl::ResizableAllocator<bpl::Arena>);
}

TEST(Arena, constructWithCapacity) {
	bpl::Arena arena(64u);
	EXPECT_GE(arena.capacity(), 64u);
	EXPECT_TRUE(arena.empty());
}

TEST(Arena, push) {
	const std::array _storage = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::span data(_storage);
	using T = decltype(data)::value_type;

	bpl::Arena arena(data.size());
	bpl::MemoryBlock block = arena.push(data.size_bytes(), alignof(T));
	EXPECT_NE(block.ptr, nullptr);
	EXPECT_GE(block.size, data.size_bytes());
	EXPECT_GE(arena.size(), data.size_bytes());

	auto* block_begin = static_cast<int32_t*>(block.ptr);
	int32_t* block_end = std::ranges::copy(data, block_begin).out;
	EXPECT_TRUE(std::equal(data.begin(), data.end(), block_begin, block_end));
}

TEST(Arena, pop) {
	constexpr size_t alignment = 4u;

	bpl::Arena arena(64u);
	bpl::MemoryBlock block1 = arena.push(16u, alignment);
	EXPECT_TRUE(arena.pop(block1, alignment));
	EXPECT_TRUE(arena.empty());

	bpl::MemoryBlock block2 = arena.push(16u, alignment);
	bpl::MemoryBlock block3 = arena.push(16u, alignment);
	EXPECT_FALSE(arena.pop(block2, alignment));
	EXPECT_TRUE(arena.pop(block3, alignment));
	EXPECT_TRUE(arena.pop(block2, alignment));
	EXPECT_TRUE(arena.empty());
}

TEST(Arena, clear) {
	constexpr size_t alignment = 4u;

	bpl::Arena arena(64u);
	bpl::MemoryBlock block1 = arena.push(16u, alignment);
	arena.clear();
	EXPECT_TRUE(arena.empty());

	bpl::MemoryBlock block2 = arena.push(16u, alignment);
	EXPECT_FALSE(arena.empty());
	EXPECT_EQ(block1, block2);
	arena.clear();
	EXPECT_TRUE(arena.empty());
}
