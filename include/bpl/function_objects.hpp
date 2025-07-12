// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Function objects.

#include <utility>

namespace bpl {

/// @name Operators
/// @{

struct plus {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x + y) {
		return std::forward<T>(x) + std::forward<U>(y);
	}
};

struct minus {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x - y) {
		return std::forward<T>(x) - std::forward<U>(y);
	}
};

struct multiplies {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x * y) {
		return std::forward<T>(x) * std::forward<U>(y);
	}
};

struct divides {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x / y) {
		return std::forward<T>(x) / std::forward<U>(y);
	}
};

struct modulus {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x % y) {
		return std::forward<T>(x) % std::forward<U>(y);
	}
};

struct negate {
	template<typename T>
	constexpr auto operator()(T&& x) -> decltype(!x) {
		return !std::forward<T>(x);
	}
};

// @}

/// @name Comparisons
/// @{

struct equal_to {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return std::forward<T>(x) == std::forward<U>(y);
	}
};

struct not_equal_to {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return !(std::forward<T>(x) == std::forward<U>(y));
	}
};

struct less {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return std::forward<T>(x) < std::forward<U>(y);
	}
};

struct less_equal {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return std::forward<T>(x) <= std::forward<U>(y);
	}
};

struct greater {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return std::forward<T>(x) > std::forward<U>(y);
	}
};

struct greater_equal {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> bool {
		return std::forward<T>(x) >= std::forward<U>(y);
	}
};

struct compare_three_way {
	template<typename T, typename U>
	constexpr auto operator()(T&& x, U&& y) -> decltype(x <=> y) {
		return std::forward<T>(x) <=> std::forward<U>(y);
	}
};

/// @}

} // namespace bpl
