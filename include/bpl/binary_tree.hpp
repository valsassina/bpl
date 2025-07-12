// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Binary tree data structures and algorithms.

#include <utility>

namespace bpl {

template<typename T>
struct BinaryTreeNode {
	T value;
	BinaryTreeNode<T>* left = nullptr;
	BinaryTreeNode<T>* right = nullptr;

	BinaryTreeNode() = default;

	explicit BinaryTreeNode(T value) : BinaryTreeNode(value, nullptr, nullptr) {}

	BinaryTreeNode(T value, BinaryTreeNode* left, BinaryTreeNode* right) : value(value), left(left), right(right) {}
};

template<typename T>
class BinaryTree {
public:
	BinaryTreeNode<T>* root = nullptr;

	/// @name Special member functions
	/// @{

	BinaryTree() = default;

	BinaryTree(const BinaryTree&) = delete;
	auto operator=(const BinaryTree&) -> BinaryTree& = delete;

	BinaryTree(BinaryTree&& other) noexcept : root(std::exchange(other.root, nullptr)) {}
	auto operator=(BinaryTree&& other) noexcept -> BinaryTree& {
		root = std::exchange(other.root, nullptr);
		return *this;
	}

	~BinaryTree() {
		traverse_post_order([](BinaryTreeNode<T>*& node) { delete node; });
	}

	/// @}

	/// @name Constructors
	/// @{

	explicit BinaryTree(BinaryTreeNode<T>* root) : root(root) {}

	explicit BinaryTree(const T& x) : root(BinaryTreeNode(x)) {}

	BinaryTree(const T& x, BinaryTreeNode<T>* left, BinaryTreeNode<T>* right) : root(BinaryTreeNode(x, left, right)) {}

	/// @}

	auto find(const T& x) const -> BinaryTreeNode<T>* {
		const auto* node = this->root;
		while (node != nullptr) {
			if (node->value == x) {
				break;
			}
			if (x < node->value) {
				node = node->left;
			} else {
				node = node->right;
			}
		}

		return node;
	}

	auto min_element() const -> BinaryTreeNode<T>* {
		if (this->root == nullptr) {
			return nullptr;
		}

		const auto* min = this->root;
		while (min->left != nullptr) {
			min = min->left;
		}
		return min;
	}

	/// Traverses a binary tree in post-order and calls `f` on every element.
	///
	/// @tparam F A function with the following signature: `void f(binary_tree_node<T>* node)`.
	template<typename F>
	void traverse_post_order(F f) const {
		traverse_post_order_fn(this->root, f);
	}

	auto insert(const T& x) -> BinaryTreeNode<T>* { return insert_fn(this->root, x); }

private:
	template<typename F>
	void traverse_post_order_fn(BinaryTreeNode<T>* node, F f) const {
		if (node == nullptr) {
			return;
		}
		traverse_post_order_fn(node->left, f);
		traverse_post_order_fn(node->right, f);
		f(node);
	}

	auto insert_fn(BinaryTreeNode<T>*& node, const T& x) -> BinaryTreeNode<T>* {
		if (node == nullptr) {
			node = ::new BinaryTreeNode<T>(x);
			return node;
		}

		if (x < node->value) {
			return insert_fn(node->left, x);
		}

		return insert_fn(node->right, x);
	}
};

} // namespace bpl
