// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Type traits and concepts.

#include <concepts>
#include <type_traits>

namespace bpl {

template<typename T>
concept pointer = std::is_pointer_v<T>;

template<typename T>
concept reference = std::is_reference_v<T>;

template<typename T>
concept trivially_copyable = std::is_trivially_copyable_v<T>;

template<typename T>
concept trivially_movable = std::is_trivially_move_constructible_v<T> && std::is_trivially_move_assignable_v<T>;

template<typename T>
concept trivially_destructible = std::is_trivially_destructible_v<T>;

template<typename T>
concept relocatable = std::movable<T> && std::destructible<T>;

} // namespace bpl
