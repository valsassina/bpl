# Copyright © 2025 Luca Valsassina
# SPDX-License-Identifier: MIT

find_package(GTest 1 REQUIRED)

include(GoogleTest)

set(tests
	algorithm
	allocator
	arena
	array
	binary_tree
	bit
	doubly_linked_list
	function_objects
	linked_list
	literals
	math
	memory
	non_null
	ring_buffer
	sort
	span
	utility
)

set(
	SANITIZER_FLAGS
	-fsanitize=address,pointer-compare,pointer-subtract,undefined$<$<CXX_COMPILER_ID:Clang>:,leak> -fsanitize-undefined-trap-on-error
)

foreach(test ${tests})
	set(target bpl.tests.${test})

	add_executable(${target})

	target_sources(${target} PRIVATE ${test}.cpp)

	target_link_libraries(
		${target}
		PRIVATE
			bpl::bpl GTest::gtest_main
	)

	target_compile_options(
		${target}
		PRIVATE
			-Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wnarrowing
			$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wmissing-method-return-type>
			# Clang <= 18
			$<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_LESS:$<CXX_COMPILER_VERSION>,18>>:-Wno-missing-field-initializers>
			# Clang >= 19
			$<$<AND:$<CXX_COMPILER_ID:Clang>,$<VERSION_GREATER_EQUAL:$<CXX_COMPILER_VERSION>,19>>:-Wno-missing-designated-field-initializers>
			-fno-exceptions -fno-rtti -fno-omit-frame-pointer
			${SANITIZER_FLAGS}
	)

	target_link_options(${target} PRIVATE ${SANITIZER_FLAGS})

	gtest_discover_tests(${target})
endforeach()
