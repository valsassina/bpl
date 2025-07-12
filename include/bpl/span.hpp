// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

#include <bpl/assert.hpp>
#include <bpl/math.hpp>
#include <bpl/ptr.hpp>
#include <bpl/ranges.hpp>

#include <bit>
#include <optional>
#include <type_traits>

#include <cstddef>
#include <cstdint>

namespace bpl {

/// Used by `Span` to refer to a range of elements.
///
/// One of the following combinations can be specified, in order of precedence:
///
/// - `[ start, end )`
/// - `[ start, last ]`
/// - `[ start, start + count )`
///
/// If `start` is not specified, the range starts at 0.
/// If `end`, `last`, and `count` are not specified, the range ends at the end of the span.
///
/// @note
///   - Use with designated initializers
///   - This is a zero-cost abstraction only if the compiler can see and inline everything
struct Bounds {
	size_t start = 0;
	std::optional<size_t> end, last, count;
};

template<typename T>
class Span;

template<typename R>
concept is_span = std::is_same_v<std::remove_cvref_t<R>, Span<range_value_t<R>>>;

/// A non-owning view over a contiguous sequence of objects.
template<typename T>
class Span {
public:
	//////////////////////////////////////////////////
	/// @name Special member functions
	/// @{

	constexpr Span() = default;

	/// @}

	//////////////////////////////////////////////////
	/// @name Constructors
	/// @{

	constexpr Span(T* data, size_t count) : m_data(data), m_count(count) {}

	/// Constructs a span of the elements in the range `r`.
	template<contiguous_range R>
	requires (!is_span<R>)
	constexpr Span(R&& r) : Span(bpl::data(r), bpl::size(r)) {}

	/// @}

	//////////////////////////////////////////////////
	/// @name Inspection
	/// @{

	/// Returns a pointer to the first element.
	constexpr auto data() const -> T* { return m_data; }

	/// Returns the size of this span, in number of elements.
	constexpr auto size() const -> size_t { return m_count; }

	/// Returns the size of this span, in bytes.
	constexpr auto size_bytes() const -> size_t { return this->size() * sizeof(T); }

	/// Returns `true` if the span is empty.
	[[nodiscard]]
	constexpr auto empty() -> bool {
		return this->size() == 0;
	}

	/// @}

	//////////////////////////////////////////////////
	/// @name Iterators
	/// @{

	/// Returns a pointer to the first element.
	constexpr auto begin() const -> T* { return this->data(); }

	/// Returns a pointer one past the last element.
	constexpr auto end() const -> T* { return this->data() + this->size(); }

	/// @}

	//////////////////////////////////////////////////
	/// @name Element access
	/// @{

	/// Returns a reference to the element at position `idx`.
	///
	/// @pre
	///   - `idx` is not out of bounds
	constexpr auto operator[](size_t idx) const -> T& {
		BPL_DEBUG_ASSERT(idx < this->size());
		return *(this->data() + idx);
	}

	/// Returns a subspan as defined by `bounds`.
	///
	/// @pre
	///   - `bounds` is a valid range
	constexpr auto operator[](Bounds bounds) const -> Span<T> {
		if (bounds.end) {
			return Span<T>(this->data() + bounds.start, *bounds.end - bounds.start);
		}
		if (bounds.last) {
			return Span<T>(this->data() + bounds.start, *bounds.end - bounds.start + 1);
		}
		if (bounds.count) {
			return Span<T>(this->data() + bounds.start, *bounds.count);
		}
		return Span<T>(this->data() + bounds.start, this->size() - bounds.start);
	}

	/// Returns a reference to the element at position `idx`, aborting if out-of-bounds.
	///
	/// Aborts if `idx` is out of bounds.
	constexpr auto at(size_t idx) const -> T& {
		BPL_ASSERT(idx < this->size());
		return *(this->data() + idx);
	}

	/// Returns a subspan as defined by `bounds`, aborting if the requested range is out-of-bounds.
	///
	/// Aborts if `bounds` is not a valid range.
	constexpr auto at(Bounds bounds) const -> Span<T> {
		BPL_ASSERT(bounds.start <= this->size());
		if (bounds.end) {
			size_t end = *bounds.end;
			BPL_ASSERT(bounds.start <= end && end <= this->size());
			return Span<T>(this->data() + bounds.start, end - bounds.start);
		}
		if (bounds.last) {
			size_t last = *bounds.last;
			BPL_ASSERT(bounds.start <= last && last < this->size());
			return Span<T>(this->data() + bounds.start, last - bounds.start + 1);
		}
		if (bounds.count) {
			size_t count = *bounds.count;
			// SAFETY: bounds.start <= this->size()
			BPL_ASSERT(this->size() - bounds.start >= count);
			return Span<T>(this->data() + bounds.start, count);
		}
		// SAFETY: bounds.start <= this->size()
		return Span<T>(this->data() + bounds.start, this->size() - bounds.start);
	}

	/// Returns a reference to the first element.
	///
	/// Aborts if the span is empty.
	constexpr auto front() -> T& { return this->at(0); }
	constexpr auto front() const -> const T& { return this->at(0); }

	/// Returns a reference to the last element.
	///
	/// Aborts if the span is empty.
	constexpr auto back() -> T& { return this->at(this->size() - 1); }
	constexpr auto back() const -> const T& { return this->at(this->size() - 1); }

	/// Transmutes this span to a span of `uint8_t`.
	///
	/// @returns A span of `uint8_t` of the elements in this span.
	constexpr auto as_bytes() const -> Span<const uint8_t> {
		return Span<const uint8_t>(std::bit_cast<const uint8_t*>(this->data()), this->size_bytes());
	}

	/// Transmutes this span to a read-only span of `uint8_t`.
	///
	/// @returns A read-only span of `uint8_t` of the elements in this span.
	constexpr auto as_writable_bytes() const -> Span<uint8_t> {
		return Span<uint8_t>(std::bit_cast<uint8_t*>(this->data()), this->size_bytes());
	}

	// template<typename U>
	// struct align_to_result {
	// 	Span<T> prefix;
	// 	Span<U> middle;
	// 	Span<T> suffix;
	// };

	/// Transmutes this span to a span of another type with the correct alignment.
	///
	/// The span is split into three distinct spans:
	///   - prefix
	///   - middle, correctly aligned to the alignment of `U`
	///   - suffix
	///
	/// @pre
	///   - `alignof(U)` is a multiple of `alignof(T)`.
	///
	/// @returns A struct containing three distinct spans: prefix, middle, and suffix.
	// template<typename U>
	// constexpr align_to_result<U> align_to() const {
	// 	static_assert(alignof(U) % alignof(T) == 0);
	// 	size_t prefix_count = ptr_align_offset(this->data(), alignof(U));
	// 	if (prefix_count > this->size()) {
	// 		return { .prefix = *this };
	// 	}
	// 	Span<T> rest = this->operator[]({ .start = prefix_count });
	// 	size_t middle_count = rest.size() / (sizeof(U) / sizeof(T));
	// 	if (middle_count == 0) {
	// 		return { .prefix = *this };
	// 	}
	// 	size_t suffix_count = rest.size() % (sizeof(U) / sizeof(T));
	// 	return {
	// 		.prefix = this->operator[]({ .count = prefix_count }),
	// 		.middle = Span<U>(reinterpret_cast<U*>(std::assume_aligned<alignof(U)>(rest.data())), middle_count),
	// 		.suffix = rest[{ .start = this->size() - suffix_count }],
	// 	};
	// }

	/// @}

	//////////////////////////////////////////////////
	/// @name Operators
	/// @{

	template<typename U>
	constexpr auto operator==(Span<U> other) const -> bool {
		if (this->size() != other.size()) {
			return false;
		}
		for (size_t i = 0; i < this->size(); ++i) {
			if (this->operator[](i) != other[i]) {
				return false;
			}
		}
		return true;
	}

	template<contiguous_range R>
	requires (!is_span<R>)
	constexpr auto operator==(R&& other) const -> bool {
		return *this == Span(std::forward<R>(other));
	}

	template<typename U>
	constexpr auto operator<=>(Span<U> other) const {
		size_t prefix = bpl::min(this->size(), other.size());
		for (size_t i = 0; i < prefix; ++i) {
			if (auto cmp = this->operator[](i) <=> other[i]; cmp != 0) {
				return cmp;
			}
		}
		return this->size() <=> other.size();
	}

	template<contiguous_range R>
	requires (!is_span<R>)
	constexpr auto operator<=>(R&& other) const {
		return *this <=> Span(std::forward<R>(other));
	}

	/// @}

private:
	// Pointer to the first element
	T* m_data = nullptr;
	// Number of elements
	size_t m_count = 0;
};

//////////////////////////////////////////////////
/// @name Deduction guides
/// @{

template<typename I>
Span(I, size_t) -> Span<iter_value_t<I>>;

template<typename I, typename S = I>
Span(I, S) -> Span<iter_value_t<I>>;

template<contiguous_range R>
Span(R&&) -> Span<range_value_t<R>>;

/// @}

} // namespace bpl
