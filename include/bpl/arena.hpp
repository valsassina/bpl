// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/assert.hpp>
#include <bpl/bit.hpp>
#include <bpl/memory.hpp>
#include <bpl/os.hpp>
#include <bpl/ptr.hpp>

#include <utility>

#include <cstddef>
#include <cstdint>

namespace bpl {

/// An arena allocator.
class Arena {
public:
	//////////////////////////////////////////////////
	/// @name Special member functions
	/// @{

	Arena() = default;

	/// Copying an arena doesn't make sense.
	Arena(Arena&) = delete;
	/// Copying an arena doesn't make sense.
	auto operator=(Arena&) -> Arena& = delete;

	Arena(Arena&& other) noexcept
		: m_block(std::exchange(other.m_block, {})),
		  m_end(std::exchange(other.m_end, nullptr)) {}
	auto operator=(Arena&& other) noexcept -> Arena& {
		m_block = std::exchange(other.m_block, {});
		m_end = std::exchange(other.m_end, {});
		return *this;
	}

	~Arena() {
		(void) try_decommit_memory(m_block);
		(void) try_release_memory(m_block);
	}

	/// @}

	//////////////////////////////////////////////////
	/// @name Constructors
	/// @{

	/// Creates an arena of `capacity` bytes.
	///
	/// Aborts if memory allocation fails.
	///
	/// @pre
	///   - `capacity > 0`
	explicit Arena(size_t capacity) {
		BPL_ASSERT(capacity > 0);
		m_block = reserve_memory(capacity);
		BPL_ASSERT(try_commit_memory(m_block));
		m_end = m_block.ptr;
	}

	/// @}

	//////////////////////////////////////////////////
	/// @name Inspection
	/// @{

	[[nodiscard]]
	auto capacity() const -> size_t {
		return m_block.size;
	}

	[[nodiscard]]
	auto size() const -> size_t {
		return ptr_to_addr(m_end) - ptr_to_addr(m_block.ptr);
	}

	[[nodiscard]]
	auto empty() const -> bool {
		return this->size() == 0;
	}

	/// @}

	//////////////////////////////////////////////////
	/// @name Methods
	/// @{

	[[nodiscard]]
	auto push(size_t size, size_t alignment) -> MemoryBlock {
		uintptr_t addr_begin = align_forward(ptr_to_addr(m_end), alignment);
		uintptr_t addr_end = align_forward(addr_begin + size, alignment);
		if (addr_end > ptr_to_addr(m_block.ptr) + m_block.size) {
			return {};
		}
		m_end = addr_to_ptr<void>(addr_end);
		return {
			.ptr = addr_to_ptr<void>(addr_begin),
			.size = static_cast<size_t>(addr_end - addr_begin),
		};
	}

	[[nodiscard]]
	auto pop(MemoryBlock block, size_t alignment) -> bool {
		uintptr_t block_end = ptr_to_addr(block.ptr) + block.size;
		uintptr_t arena_end_aligned = align_forward(ptr_to_addr(m_end), alignment);
		if (block_end != arena_end_aligned) {
			return false;
		}
		m_end = block.ptr;
		return true;
	}

	/// Clears the arena.
	void clear() { m_end = m_block.ptr; }

	/// @}

	/// @name Allocator API
	/// @{

	[[nodiscard]]
	auto allocate(size_t size, size_t alignment) -> MemoryBlock {
		return this->push(size, alignment);
	}

	void deallocate(MemoryBlock block, size_t alignment) { (void) this->pop(block, alignment); }

	[[nodiscard]]
	auto try_grow(MemoryBlock block, size_t alignment, size_t additional) -> MemoryBlock {
		uintptr_t block_end = ptr_to_addr(block.ptr) + block.size;
		uintptr_t arena_end = ptr_to_addr(m_end);
		if (block_end != arena_end) {
			return {};
		}
		MemoryBlock new_block = this->push(additional, alignment);
		return { .ptr = block.ptr, .size = block.size + new_block.size };
	}

	[[nodiscard]]
	auto try_shrink(MemoryBlock block, size_t alignment, size_t new_size) -> bool {
		if (new_size > block.size) {
			return false;
		}
		return this->pop(
			MemoryBlock{ .ptr = static_cast<char*>(block.ptr) + new_size, .size = block.size - new_size }, alignment
		);
	}

	/// @}

private:
	MemoryBlock m_block = {};
	void* m_end = m_block.ptr;
};

} // namespace bpl
