// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/memory.hpp>
#include <bpl/span.hpp>
#include <bpl/tags.hpp>

#include <utility>

#include <cstddef>

namespace bpl {

template<typename T>
struct LinkedListNode {
	T data;
	LinkedListNode* next = nullptr;

	/// @name Special member functions
	/// @{

	LinkedListNode() = default;

	/// @}

	/// @name Constructors
	/// @{

	template<typename... Args>
	explicit LinkedListNode(in_place_t, Args&&... args) : data(std::forward<Args>(args)...) {}

	template<typename... Args>
	explicit LinkedListNode(LinkedListNode* next, Args&&... args) : data(std::forward<Args>(args)...),
																	next(next) {}

	/// @}
};

template<typename T>
class LinkedListIterator {
public:
	/// @name Types
	/// @{

	using value_type = T;
	using difference_type = std::ptrdiff_t;

	/// @}

	/// @name Special member functions
	/// @{

	LinkedListIterator() = default;

	/// @}

	/// @name Constructors
	/// @{

	explicit LinkedListIterator(LinkedListNode<T>* node) : m_node(node) {}

	/// @}

	/// @name Operators
	/// @{

	auto operator*() const -> T& { return m_node->data; }

	auto operator->() const -> T* { return &(m_node->data); }

	auto operator++() -> LinkedListIterator& {
		m_node = m_node->next;
		return *this;
	}

	auto operator++(int) -> LinkedListIterator {
		LinkedListIterator tmp(*this);
		++(*this);
		return tmp;
	}

	auto operator==(const LinkedListIterator&) const -> bool = default;

	/// @}

private:
	LinkedListNode<T>* m_node = nullptr;
};

/// A singly linked list.
template<typename T>
class LinkedList {
public:
	/// @name Special member functions
	/// @{

	LinkedList() = default;

	LinkedList(LinkedList&) = delete;
	auto operator=(LinkedList&) -> LinkedList = delete;

	LinkedList(LinkedList&& other) noexcept : m_head(std::exchange(other.m_head, nullptr)) {}
	auto operator=(LinkedList&& rhs) noexcept -> LinkedList& {
		m_head = std::exchange(rhs.m_head, nullptr);
		return *this;
	}

	~LinkedList() {
		LinkedListNode<T>* node = m_head;
		while (node != nullptr) {
			delete std::exchange(node, node->next);
		}
	}

	/// @}

	/// @name Constructors
	/// @{

	/// @{
	/// Create a linked list with `count` elements constructed in-place.
	template<typename... Args>
	explicit LinkedList(size_t count, Args&&... args) {
		if (count == 0) {
			return;
		}
		m_head = make_node(in_place, std::forward<Args>(args)...);
		LinkedListNode<T>* node = m_head;
		for (size_t i = 1; i < count; ++i) {
			node->next = make_node(in_place, std::forward<Args>(args)...);
			node = node->next;
		}
	}
	/// @}

	explicit LinkedList(T* data, size_t count) {
		if (count == 0) {
			return;
		}
		m_head = make_node(in_place, *data);
		LinkedListNode<T>* node = m_head;
		for (const auto& element : make_sized_range(data + 1, count - 1)) {
			node->next = make_node(in_place, element);
			node = node->next;
		}
	}

	explicit LinkedList(Span<T> span) : LinkedList(span.data(), span.size()) {}

	/// @}

	/// @name Inspection
	/// @{

	static auto alignment() -> size_t { return alignof(LinkedListNode<T>); }

	/// @}

	/// @name Iterators
	/// @{

	auto begin() -> LinkedListIterator<T> { return LinkedListIterator(m_head); }
	auto begin() const -> LinkedListIterator<const T> { return LinkedListIterator(m_head); }

	auto end() -> LinkedListIterator<T> { return LinkedListIterator<T>(nullptr); }
	auto end() const -> LinkedListIterator<const T> { return LinkedListIterator<T>(nullptr); }

	/// @}

private:
	LinkedListNode<T>* m_head = nullptr;

	template<typename... Args>
	auto make_node(Args&&... args) -> LinkedListNode<T>* {
		return ::new LinkedListNode<T>(std::forward<Args>(args)...);
	}
};

} // namespace bpl
