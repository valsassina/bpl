// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/allocator.hpp>
#include <bpl/memory.hpp>

#include <optional>
#include <utility>

#include <cstddef>

namespace bpl {

/// A ring buffer with fixed capacity.
template<typename T, Allocator A = GlobalAllocator>
class RingBuffer {
public:
	//////////////////////////////////////////////////
	/// @name Special member functions
	/// @{

	RingBuffer() = default;

	/// You can't copy a ring buffer without consuming it.
	RingBuffer(const RingBuffer&) = delete;
	auto operator=(const RingBuffer&) -> RingBuffer& = delete;

	RingBuffer(RingBuffer&& other) noexcept
		: m_block(std::exchange(other.m_block, {})),
		  m_read_index(std::exchange(other.m_read_index, 0)),
		  m_write_index(std::exchange(other.m_write_index, 0)),
		  m_allocator(std::move(other.m_allocator)) {}

	auto operator=(RingBuffer&& rhs) noexcept -> RingBuffer& {
		this->release();

		m_block = std::exchange(rhs.m_block, {});
		m_read_index = std::exchange(rhs.m_read_index, 0);
		m_write_index = std::exchange(rhs.m_write_index, 0);
		m_allocator = std::move(rhs.m_allocator);
	}

	~RingBuffer() { this->release(); }

	/// @}

	//////////////////////////////////////////////////
	/// @name Constructors
	/// @{

	explicit RingBuffer(A&& allocator) : m_allocator(std::move(allocator)) {}

	explicit RingBuffer(size_t capacity) : RingBuffer(A{}, capacity) {}
	explicit RingBuffer(A&& allocator, size_t capacity) : RingBuffer(std::move(allocator)) {
		m_block = m_allocator.allocate(capacity * sizeof(T), this->alignment());
	}

	/// @}

	//////////////////////////////////////////////////
	/// @name Methods
	/// @{

	auto data() -> T* { return static_cast<T*>(m_block.ptr); }
	auto data() const -> const T* { return static_cast<T*>(m_block.ptr); }

	auto size() const -> size_t {
		if (m_read_index <= m_write_index) {
			return m_write_index - m_read_index;
		}
		return (this->capacity() - m_read_index) + m_write_index;
	}

	auto capacity() const -> size_t { return m_block.size / sizeof(T); }

	auto alignment() const -> size_t { return alignof(T); }

	[[nodiscard]]
	auto push(const T& x) -> bool {
		size_t next_write_index = (m_write_index + 1) % this->capacity();
		if (next_write_index == m_read_index) {
			return false;
		}
		this->data()[m_write_index] = x;
		m_write_index = next_write_index;
		return true;
	}

	[[nodiscard]]
	auto pop() -> std::optional<T> {
		std::optional<T> result;
		if (m_read_index != m_write_index) {
			result = std::move(this->data()[m_read_index]);
			std::destroy_at(this->data() + m_read_index);
			m_read_index = (m_read_index + 1) % this->capacity();
		}
		return result;
	}

	/// @}

private:
	MemoryBlock m_block = {};
	size_t m_read_index = 0;
	size_t m_write_index = 0;
	[[no_unique_address]] A m_allocator{};

	// Destroys all the elements and deallocates the memory.
	void release() {
		for (size_t i = m_read_index; i != m_write_index; i = (i + 1) % this->capacity()) {
			std::destroy_at(&this->data()[i]);
		}
		if (m_block.ptr != nullptr) {
			m_allocator.deallocate(m_block, this->alignment());
		}
	}
};

} // namespace bpl
