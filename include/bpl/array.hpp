// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/allocator.hpp>
#include <bpl/assert.hpp>
#include <bpl/math.hpp>
#include <bpl/memory.hpp>
#include <bpl/ranges.hpp>
#include <bpl/span.hpp>
#include <bpl/tags.hpp>
#include <bpl/traits.hpp>

#include <limits>
#include <memory>
#include <utility>

#include <cstddef>

namespace bpl {

/// A dynamic array.
template<relocatable T, Allocator A = GlobalAllocator>
class Array {
public:
	/// @name Special member functions
	/// @{

	/// Constructs an empty dynamic array.
	Array() = default;

	Array(Array& other);
	auto operator=(const Array& other) -> Array&;

	Array(Array&& other) noexcept;
	auto operator=(Array&& other) noexcept -> Array&;

	~Array();

	/// @}

	/// @name Constructors
	/// @{

	/// Constructs an empty dynamic array with a custom allocator.
	explicit Array(A&& allocator);

	/// Constructs a dynamic array with `count` uninitialized elements.
	///
	/// @warning Reading from uninitialized memory is undefined behaviour.
	explicit Array(uninit_t, size_t count) : Array(uninit, A{}, count) {}
	explicit Array(uninit_t, A&& allocator, size_t count) : Array(std::move(allocator)) {
		if (count == 0) {
			return;
		}
		m_block = m_allocator.allocate(count * sizeof(T), this->alignment());
		m_count = count;
	}

	/// Constructs a dynamic array with `count` elements, constructed in-place.
	template<typename... Args>
	explicit Array(size_t count, Args&&... args) : Array(A{}, count, std::forward<Args>(args)...) {}
	template<typename... Args>
	explicit Array(A&& allocator, size_t count, Args&&... args) : Array(uninit, std::move(allocator), count) {
		construct(*this, std::forward<Args>(args)...);
	}

	/// Constructs a dynamic array by copying the elements from the range `r`.
	template<range R>
	explicit Array(from_range_t, R&& r) : Array(from_range, A{}, std::forward<R>(r)) {}
	template<range R>
	explicit Array(from_range_t, A&& allocator, R&& r) : Array(std::move(allocator)) {
		for (const auto& element : r) {
			this->append(element);
		}
	}

	/// Constructs a dynamic array by copying the elements from the sized range `r`.
	template<sized_range R>
	explicit Array(from_range_t, R&& r) : Array(from_range, A{}, std::forward<R>(r)) {}
	template<sized_range R>
	explicit Array(from_range_t, A&& allocator, R&& r) : Array(uninit, std::move(allocator), bpl::size(r)) {
		uninitialized_copy(r, *this);
	}

	/// @}

	/// @name Iterators
	/// @{

	/// Returns a pointer to the first element.
	auto data() -> T* { return static_cast<T*>(m_block.ptr); }
	auto data() const -> const T* { return static_cast<const T*>(m_block.ptr); }

	/// Returns a pointer to the first element.
	auto begin() -> T* { return this->data(); }
	auto begin() const -> const T* { return this->data(); }

	/// Returns a pointer to one past the last element.
	auto end() -> T* { return this->data() + this->size(); }
	auto end() const -> const T* { return this->data() + this->size(); }

	/// @}

	/// @name Element access
	/// @{

	/// Returns a reference to the element at `idx`.
	///
	/// @pre
	///   - `idx` is not out-of-bounds.
	auto operator[](size_t idx) -> T& { return Span(*this)[idx]; }
	auto operator[](size_t idx) const -> const T& { return Span(*this)[idx]; }

	/// Returns a reference to the element at `idx`.
	///
	/// Aborts if `idx` is out-of-bounds.
	auto at(size_t idx) -> T& { return Span(*this).at(idx); }
	auto at(size_t idx) const -> T& { return Span(*this).at(idx); }

	/// Returns a reference to the first element.
	///
	/// Aborts if `idx` is out-of-bounds.
	auto front() -> T& { return Span(*this).front(); }
	auto front() const -> const T& { return Span(*this).front(); }

	/// Returns a reference to the last element.
	///
	/// @pre
	///   - `empty() == false`.
	auto back() -> T& { return Span(*this).back(); }
	auto back() const -> const T& { return Span(*this).back(); }

	/// @}

	/// @name Inspection
	/// @{

	/// Returns the number of elements in the container.
	auto size() const -> size_t { return m_count; }

	/// Returns the size of the array in bytes.
	auto size_bytes() const -> size_t { return this->size() * sizeof(T); }

	/// Returns `true` if the container has no elements.
	auto empty() const -> bool { return this->size() == 0; }

	/// Returns the number of elements that can be stored in the currently allocated block of memory.
	auto capacity() const -> size_t { return m_block.size / sizeof(T); }

	/// Returns the alignment of the block of memory.
	auto alignment() const -> size_t { return alignof(T); }

	/// Returns a const reference to the underlying allocator.
	auto allocator() const -> const A& { return m_allocator; }

	/// @}

	/// @name Modifiers
	/// @{

	/// Increases capacity of the container to fit at least `count` elements.
	void reserve(size_t count);

	/// Destroys all elements in the array.
	void clear();

	/// Resizes array to contain `count` elements, leaving new elements uninitialized.
	void resize_uninit(size_t count);

	/// Resizes array to contain `count` elements, constructing new elements in-place.
	template<typename... Args>
	void resize(size_t count, Args&&... args);

	/// Replaces all elements of the array with `count` copies of `x`.
	void assign(size_t count, const T& x);

	/// Replaces all elements of the array with a copy of each element in `range`.
	template<sized_range R>
	void assign(R&& range);

	/// Appends a new value in-place.
	template<typename... Args>
	void append(Args&&... args);

	/// Appends `n` new values in-place.
	template<typename... Args>
	void append_n(size_t n, Args&&... args);

	/// Appends elements from `range`, one at a time.
	template<range R>
	void append_range(R&& range);

	/// Appends elements from `range`.
	template<contiguous_range R>
	void append_range(R&& range);

	/// Inserts an element before `idx`, in-place.
	template<typename... Args>
	void insert(size_t idx, Args&&... args);

	template<sized_range R>
	void insert_range(size_t idx, R&& range);

	/// Erases and returns the element at `idx`.
	///
	/// @pre
	///   - `idx < size()`
	auto remove(size_t idx) -> T;

	/// Erases the elements in `[ start, end )`.
	///
	/// @pre
	///   - `start <= size()`
	///   - `end <= size()`
	///   - `end - start <= size()`
	void remove(size_t start, size_t end);

	void swap(Array& other) noexcept;

	/// @}

private:
	MemoryBlock m_block = {};
	size_t m_count = 0;
	[[no_unique_address]] A m_allocator{};

	auto as_raw_span() -> Span<T> { return Span(this->data(), this->capacity()); }

	auto allocate_amortized(size_t count) const -> MemoryBlock {
		BPL_DEBUG_ASSERT(count <= std::numeric_limits<size_t>::max() / sizeof(T));
		size_t bytes = count * sizeof(T);
		if (bytes >= std::numeric_limits<size_t>::max() / 2) {
			bytes = std::numeric_limits<size_t>::max();
		} else {
			bytes = bpl::max(this->capacity() * sizeof(T) * 2, bytes);
		}
		return m_allocator.allocate(bytes, this->alignment());
	}

	void deallocate() {
		if (this->data() != nullptr) {
			m_allocator.deallocate(m_block, this->alignment());
			m_block = {};
		}
	}
};

template<relocatable T, Allocator A>
Array<T, A>::Array(Array& other) : Array(other.data(), other.size()) {
}

template<relocatable T, Allocator A>
auto Array<T, A>::operator=(const Array& other) -> Array<T, A>& {
	if (this == &other) {
		return *this;
	}
	this->assign(other.data(), other.size());
	return *this;
}

template<relocatable T, Allocator A>
Array<T, A>::Array(Array&& other) noexcept {
	this->clear();
	this->deallocate();
	m_block = std::exchange(other.m_block, {});
	m_count = std::exchange(other.m_count, 0);
	m_allocator = std::move(other.m_allocator); // NOLINT(cppcoreguidelines-prefer-member-initializer)
}

template<relocatable T, Allocator A>
auto Array<T, A>::operator=(Array&& other) noexcept -> Array<T, A>& {
	this->clear();
	this->deallocate();
	m_block = std::exchange(other.m_block, {});
	m_count = std::exchange(other.m_count, 0u);
	m_allocator = std::move(other.m_allocator);
	return *this;
}

template<relocatable T, Allocator A>
Array<T, A>::~Array() {
	this->clear();
	this->deallocate();
}

template<relocatable T, Allocator A>
Array<T, A>::Array(A&& allocator) : m_allocator(std::move(allocator)) {
}

template<relocatable T, Allocator A>
void Array<T, A>::reserve(size_t count) {
	if (count <= this->capacity()) {
		return;
	}
	MemoryBlock new_block = m_allocator.allocate(count * sizeof(T), this->alignment());
	relocate(*this, Span<T>(static_cast<T*>(new_block.ptr), new_block.size / sizeof(T)));
	m_allocator.deallocate(m_block, this->alignment());
	m_block = new_block;
}

template<relocatable T, Allocator A>
void Array<T, A>::clear() {
	destroy_backward(*this);
	m_count = 0;
}

template<relocatable T, Allocator A>
void Array<T, A>::resize_uninit(size_t count) {
	if (count > this->capacity()) {
		this->reserve(count);
	} else if (count < this->size()) {
		destroy_backward(Span(*this)[{ .start = count }]);
	}
	m_count = count;
}

template<relocatable T, Allocator A>
template<typename... Args>
void Array<T, A>::resize(size_t count, Args&&... args) {
	if (count > this->size()) {
		if (count > this->capacity()) {
			this->reserve(count);
		}
		construct(this->as_raw_span()[{ .start = this->size(), .end = count }], std::forward<Args>(args)...);
	} else if (count < this->size()) {
		destroy_backward(Span(*this)[{ .start = count }]);
	}
	m_count = count;
}

template<relocatable T, Allocator A>
void Array<T, A>::assign(size_t count, const T& x) {
	if (count <= this->capacity()) {
		if (count <= this->size()) {
			fill(Span(*this)[{ .count = count }], x);
		} else {
			fill(*this, x);
			construct(this->as_raw_span()[{ .start = this->size(), .end = count }]);
		}
	} else {
		*this = Array<T, A>(uninit, std::move(m_allocator), count);
		construct(Span(*this)[{ .count = count }], x);
	}
	m_count = count;
}

template<relocatable T, Allocator A>
template<sized_range R>
void Array<T, A>::assign(R&& range) {
	if (bpl::size(range) <= this->capacity()) {
		if (bpl::size(range) <= this->size()) {
			copy(range, *this);
		} else {
			copy_n(range, this->size(), *this);
			std::uninitialized_copy_n(bpl::begin(range) + this->size(), bpl::size(range) - this->size(), this->end());
		}
	} else {
		*this = Array<T, A>(uninit, std::move(m_allocator), bpl::size(range));
		uninitialized_copy(range, *this);
	}
	m_count = bpl::size(range);
}

template<relocatable T, Allocator A>
template<typename... Args>
void Array<T, A>::append(Args&&... args) {
	this->reserve(this->size() + 1);
	std::construct_at(this->end(), std::forward<Args>(args)...);
	m_count += 1;
}

template<relocatable T, Allocator A>
template<typename... Args>
void Array<T, A>::append_n(size_t n, Args&&... args) {
	this->reserve(this->size() + n);
	m_count += construct(this->as_raw_span()[{ .start = this->size(), .count = n }], std::forward<Args>(args)...);
}

template<relocatable T, Allocator A>
template<range R>
void Array<T, A>::append_range(R&& range) {
	for (const auto& element : range) {
		this->append(element);
	}
}

template<relocatable T, Allocator A>
template<contiguous_range R>
void Array<T, A>::append_range(R&& range) {
	this->reserve(this->size() + bpl::size(range));
	m_count += uninitialized_copy(range, this->as_raw_span()[{ .start = this->size() }]);
}

template<relocatable T, Allocator A>
template<typename... Args>
void Array<T, A>::insert(size_t i, Args&&... args) {
	this->reserve(this->size() + 1);
	relocate_backward(Span(*this)[{ .start = i }], this->as_raw_span()[{ .start = this->size() + 1 }]);
	std::construct_at(this->data() + i, std::forward<Args>(args)...);
	m_count += 1;
}

template<relocatable T, Allocator A>
template<sized_range R>
void Array<T, A>::insert_range(size_t idx, R&& range) {
	if (bpl::size(range) == 0) {
		return;
	}
	this->reserve(this->size() + bpl::size(range));
	relocate_backward(Span(*this)[{ .start = idx }], this->as_raw_span()[{ .end = this->size() + bpl::size(range) }]);
	m_count += uninitialized_copy(range, this->as_raw_span()[{ .start = idx }]);
}

template<relocatable T, Allocator A>
auto Array<T, A>::remove(size_t idx) -> T {
	BPL_ASSERT(idx < this->size());
	T x = std::move(this->operator[](idx));
	std::destroy_at(std::addressof(this->operator[](idx)));
	copy(Span(*this)[{ .start = idx + 1 }], Span(*this)[{ .start = idx }]);
	m_count -= 1;
	return x;
}

template<relocatable T, Allocator A>
void Array<T, A>::remove(size_t start, size_t end) {
	BPL_DEBUG_ASSERT(start <= this->size());
	BPL_DEBUG_ASSERT(end <= this->size());
	BPL_DEBUG_ASSERT(end - start <= this->size());
	destroy_backward(Span(*this)[{ .start = start, .end = end }]);
	copy(Span(*this)[{ .start = end }], Span(*this)[{ .start = start }]);
	m_count -= end - start;
}

template<relocatable T, Allocator A>
void Array<T, A>::swap(Array& other) noexcept {
	std::swap(m_block, other.m_block);
	std::swap(m_count, other.m_count);
	std::swap(m_allocator, other.m_allocator);
}

} // namespace bpl
