// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/memory.hpp>
#include <bpl/tags.hpp>
#include <bpl/utility.hpp>

#include <utility>

#include <cstddef>

namespace bpl {

template<typename T>
struct DoublyLinkedListNode {
	T data{};
	DoublyLinkedListNode* next = nullptr;
	DoublyLinkedListNode* prev = nullptr;

	DoublyLinkedListNode() = default;

	template<typename... Args>
	DoublyLinkedListNode(in_place_t, Args&&... args) : data(std::forward<Args>(args)...) {}

	template<typename... Args>
	DoublyLinkedListNode(DoublyLinkedListNode* next, DoublyLinkedListNode* prev, Args&&... args)
		: data(std::forward<Args>(args)...),
		  next(next),
		  prev(prev) {}
};

template<typename T>
class DoublyLinkedListIterator {
public:
	// Types

	using value_type = T;
	using difference_type = std::ptrdiff_t;

	// Special member functions

	DoublyLinkedListIterator() = default;

	// Constructors

	explicit DoublyLinkedListIterator(DoublyLinkedListNode<T>* node) : m_node(node) {}

	// Operators

	auto operator*() const -> T& { return m_node->data; }

	auto operator->() const -> T* { return &(m_node->data); }

	auto operator++() -> DoublyLinkedListIterator& {
		m_node = m_node->next;
		return *this;
	}

	auto operator++(int) -> DoublyLinkedListIterator {
		DoublyLinkedListIterator tmp(*this);
		++(*this);
		return tmp;
	}

	auto operator--() -> DoublyLinkedListIterator& {
		m_node = m_node->prev;
		return *this;
	}

	auto operator--(int) -> DoublyLinkedListIterator {
		DoublyLinkedListIterator tmp(*this);
		--(*this);
		return tmp;
	}

	auto operator==(const DoublyLinkedListIterator&) const -> bool = default;

private:
	DoublyLinkedListNode<T>* m_node = nullptr;
};

/// A doubly linked list.
template<typename T>
class DoublyLinkedList {
public:
	// Special member functions

	DoublyLinkedList() = default;

	DoublyLinkedList(DoublyLinkedList&) = delete;
	auto operator=(DoublyLinkedList&) -> DoublyLinkedList& = delete;

	DoublyLinkedList(DoublyLinkedList&& other) noexcept : m_head(std::exchange(other.m_head, nullptr)) {}
	auto operator=(DoublyLinkedList&& rhs) noexcept -> DoublyLinkedList& {
		m_head = std::exchange(rhs.m_head, nullptr);
	}

	~DoublyLinkedList() {
		DoublyLinkedListNode<T>* node = m_head;
		while (node != nullptr) {
			delete std::exchange(node, node->next);
		}
	}

	// Constructors

	explicit DoublyLinkedList(size_t count) {
		if (count == 0) {
			return;
		}
		m_head = make_node();
		DoublyLinkedListNode<T>* node = m_head;
		for (size_t i = 1; i < count; ++i) {
			node->next = make_node(nullptr, node);
			node = node->next;
		}
	}

	DoublyLinkedList(size_t count, const T& x) {
		if (count == 0) {
			return;
		}
		m_head = make_node();
		auto node = m_head;
		for (size_t i = 1; i < count; ++i) {
			node->next = make_node(nullptr, node, x);
			node = node->next;
		}
	}

	/// @{
	/// @tparam I Forward iterator.
	template<typename I>
	DoublyLinkedList(I begin, I end) {
		if (begin == end) {
			return;
		}
		m_head = make_node(*begin);
		auto node = m_head;
		for (const auto& element : make_range(begin, end)) {
			node->next = make_node(nullptr, node, element);
			node = node->next;
		}
	}
	/// @}

	/// @{
	/// @tparam R Forward range.
	template<typename R>
	DoublyLinkedList(from_range_t, R&& range) : DoublyLinkedList(bpl::begin(range), bpl::end(range)) {}
	/// @}

	// Inspection

	auto alignment() const -> size_t { return alignof(DoublyLinkedListNode<T>); }

	// Iterators

	auto begin() -> DoublyLinkedListIterator<T> { return DoublyLinkedListIterator(m_head); }
	auto begin() const -> DoublyLinkedListIterator<const T> { return DoublyLinkedListIterator(m_head); }

	auto end() -> DoublyLinkedListIterator<T> { return DoublyLinkedListIterator<T>(nullptr); }
	auto end() const -> DoublyLinkedListIterator<const T> { return DoublyLinkedListIterator<T>(nullptr); }

private:
	DoublyLinkedListNode<T>* m_head = nullptr;

	/// @param Args Arguments to pass to the constructor of `T`.
	template<typename... Args>
	auto make_node(DoublyLinkedListNode<T>* next = nullptr, DoublyLinkedListNode<T>* prev = nullptr, Args&&... args)
		-> DoublyLinkedListNode<T>* {
		return ::new DoublyLinkedListNode<T>(next, prev, std::forward<Args>(args)...);
	}
};

} // namespace bpl
