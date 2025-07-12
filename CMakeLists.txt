# Copyright © 2025 Luca Valsassina
# SPDX-License-Identifier: MIT

# CMake 3.23 added File Sets
cmake_minimum_required(VERSION 3.23...4.1)

project(bpl LANGUAGES CXX)

option(BPL_DOCS "Enable building documentation." ${PROJECT_IS_TOP_LEVEL})
option(BPL_TESTS "Enable building tests." ${PROJECT_IS_TOP_LEVEL})

add_library(bpl)
add_library(bpl::bpl ALIAS bpl)

target_sources(bpl
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS include
		FILES
			include/bpl/algorithm.hpp
			include/bpl/allocator.hpp
			include/bpl/arena.hpp
			include/bpl/array.hpp
			include/bpl/assert.hpp
			include/bpl/binary_tree.hpp
			include/bpl/bit.hpp
			include/bpl/doubly_linked_list.hpp
			include/bpl/function_objects.hpp
			include/bpl/linked_list.hpp
			include/bpl/literals.hpp
			include/bpl/macros.hpp
			include/bpl/math.hpp
			include/bpl/memory.hpp
			include/bpl/non_null.hpp
			include/bpl/os.hpp
			include/bpl/ranges.hpp
			include/bpl/ring_buffer.hpp
			include/bpl/sort.hpp
			include/bpl/span.hpp
			include/bpl/tags.hpp
			include/bpl/traits.hpp
			include/bpl/utility.hpp
	PRIVATE
		src/os.cpp
)

set_target_properties(bpl PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON)

if(BPL_DOCS)
	add_subdirectory(docs)
endif()

if(BPL_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
