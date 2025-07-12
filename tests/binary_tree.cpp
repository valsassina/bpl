// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/array.hpp>
#include <bpl/binary_tree.hpp>
#include <bpl/span.hpp>

#include <gtest/gtest.h>

#include <array>

TEST(BinaryTree, defaultConstruct) {
	bpl::BinaryTree<int> tree;
	for (int i = 1; i <= 3; ++i) {
		tree.insert(i);
	}

	auto values = bpl::Array<int>{};
	values.reserve(3);
	tree.traverse_post_order([&values](const bpl::BinaryTreeNode<int>* node) { values.append(node->value); });
	EXPECT_TRUE(bpl::Span<int>(values) == bpl::Span<int>(std::array{ 3, 2, 1 }));
}
