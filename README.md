<!--
Copyright © 2025 Luca Valsassina
SPDX-License-Identifier: MIT
-->

# bpl

A collection of simple and fast containers, allocators and, algorithms written in C++ 20.

> [!NOTE]
>
> This library does not support exceptions.

## Headers

### Containers

- `bpl/array.hpp`: a dynamic array with custom allocator support.
- `bpl/span.hpp`: like `std::span` but can be used with an `std::initializer_list` in a function parameter.
- `bpl/linked_list.hpp`
- `bpl/doubly_linked_list.hpp`
- `bpl/binary_tree.hpp`
- `bpl/ring_buffer.hpp`: a ring buffer with fixed capacity and custom allocator support.

### Algorithms

- `bpl/algorithm.hpp`: various general purpose algorithms like `partition`, `lower_bound`, `upper_bound`, `binary_search`, etc.
- `bpl/sort.hpp`: sorting algorithms like `selection_sort`, `insertion_sort`, `quicksort`, etc.

### Memory

- `bpl/allocator.hpp`: C++ 20 concepts to use allocators with containers.
- `bpl/arena.hpp`: an arena allocator.
- `bpl/memory.hpp`: data structures and functions to work with raw memory.
- `bpl/non_null.hpp`: a pointer that is never null.
- `bpl/ptr.hpp`: functions to work with pointers.

### Utility

- `bpl/assert.hpp`: the classic assert macros.
- `bpl/bit.hpp`: functions that manipulate bits.
- `bpl/function_objects.hpp`: used by STL-style algorithms.
- `bpl/literals.hpp`: useful user-defined literals.
- `bpl/macros.hpp`: macros to help with portability between different compilers.
- `bpl/math.hpp`: math functions.
- `bpl/os.hpp`: platform-specific functions to interface with an OS.
- `bpl/ranges.hpp`: like C++ 20 ranges but simpler and much faster to compile.
- `bpl/tags.hpp`: tags are used with forwarding references in constructors.
- `bpl/traits.hpp`: useful concepts.
- `bpl/utility.hpp`: anything that didn't belong in the other headers.

## Requirements

- Linux or macOS
- Clang 14+
- CMake (see `cmake_minimum_required` in [CMakeLists.txt][bpl-cmakelists.txt] for version)
- Doxygen 1+ (optional)
- GTest 1+ (optional)

> [!NOTE]
>
> Other versions of these dependencies may work.

## Building

### Using CMake Workflow Preset

```sh
cmake --workflow clang
```

### Manually

```sh
cmake -B build -S . -DCMAKE_CXX_STANDARD=20
cmake --build build
ctest --test-dir build
```

## Usage

To include this library in your project, you can use [FetchContent ↗][cmake-fetchcontent] in your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
	bpl
	GIT_REPOSITORY https://github.com/valsassina/bpl
	GIT_TAG        main
)
FetchContent_MakeAvailable(bpl)

target_link_libraries(<your-target> bpl::bpl)
```

### Example

```cpp
#include <bpl/array.hpp>
#include <bpl/sort.hpp>

bpl::Array<int> numbers = { 1, 6, 4, 8, 7 };
bpl::quicksort(numbers);
// numbers = 1, 4, 6, 7, 8
```

## License

This project is distributed under the [MIT License][bpl-license].

<!-- Internal links -->
[bpl-cmakelists.txt]: CMakeLists.txt
[bpl-license]: LICENSE.txt

<!-- External links -->
[cmake-fetchcontent]: https://cmake.org/cmake/help/latest/module/FetchContent.html
[cppreference-explicit-this-parameter]: https://en.cppreference.com/w/cpp/experimental/feature_test.html#cpp_explicit_this_parameter
