// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/assert.hpp>
#include <bpl/bit.hpp>
#include <bpl/memory.hpp>
#include <bpl/os.hpp>

#include <sys/mman.h>

#include <cstddef>

namespace bpl {

auto reserve_memory(size_t size) -> MemoryBlock {
	BPL_DEBUG_ASSERT(size > 0);
	const size_t allocation_bytes = align_forward(size, get_page_size());
	void* ptr = mmap(nullptr, allocation_bytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	BPL_ASSERT(ptr != MAP_FAILED);
	return { .ptr = ptr, .size = allocation_bytes };
}

auto try_commit_memory(MemoryBlock block) -> bool {
	BPL_DEBUG_ASSERT(block.size % get_page_size() == 0);
	return mprotect(block.ptr, block.size, PROT_READ | PROT_WRITE) == 0;
}

auto try_decommit_memory(MemoryBlock block) -> bool {
	BPL_DEBUG_ASSERT(block.size % get_page_size() == 0);
	if (madvise(block.ptr, block.size, MADV_DONTNEED) != 0) {
		return false;
	}
	return mprotect(block.ptr, block.size, PROT_NONE) == 0;
}

auto try_release_memory(MemoryBlock block) -> bool {
	BPL_DEBUG_ASSERT(block.size % get_page_size() == 0);
	return munmap(block.ptr, block.size) == 0;
}

} // namespace bpl
