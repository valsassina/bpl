// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/assert.hpp>

#include <utility>

#include <cstddef>

namespace bpl {

/// A pointer that is never null.
template<typename T>
class NonNull {
public:
	/// @name Types
	/// @{

	using value_type = T;

	/// @}

	/// @name Special member functions
	/// @{

	constexpr NonNull() = delete;

	constexpr NonNull(const NonNull& other) = default;
	constexpr auto operator=(const NonNull& other) -> NonNull& = default;
	/// Copy assignment from a `nullptr` is not allowed.
	constexpr auto operator=(std::nullptr_t) -> NonNull& = delete;

	constexpr NonNull(NonNull&& other) = default;
	constexpr auto operator=(NonNull&& other) -> NonNull& = default;

	constexpr ~NonNull() = default;

	/// @}

	/// @name Constructors
	/// @{

	/// Constructs from a pointer `p`, aborting if `p == nullptr`.
	constexpr NonNull(T* p) : m_ptr(p) { BPL_ASSERT(m_ptr != nullptr); }

	/// Construction from a `nullptr` is not allowed.
	constexpr NonNull(std::nullptr_t) = delete;

	/// @}

	/// @name Operators
	/// @{

	constexpr auto operator*() const -> T& { return *(this->get()); }

	constexpr auto operator->() const -> T* { return this->get(); }

	constexpr auto operator==(const NonNull&) const -> bool = default;

	/// @}

	/// @name Inspection
	/// @{

	/// Returns the raw pointer.
	constexpr auto get() const -> T* { return m_ptr; }

	/// @}

	/// @name Modifiers
	/// @{

	constexpr void swap(NonNull<T*>& other) { std::swap(m_ptr, other.m_ptr); }

	constexpr void swap(NonNull<T*>& lhs, NonNull<T*>& rhs) { lhs.swap(rhs); }

	/// @}

private:
	T* m_ptr;
};

} // namespace bpl
