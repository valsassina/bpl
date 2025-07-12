// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#pragma once

/// @file
/// Tags.

namespace bpl {

struct from_range_t {
	explicit from_range_t() = default;
};
inline constexpr from_range_t from_range{};

struct in_place_t {
	explicit in_place_t() = default;
};
inline constexpr in_place_t in_place{};

struct uninit_t {
	explicit uninit_t() = default;
};
inline constexpr uninit_t uninit{};

struct unsafe_t {
	explicit unsafe_t() = default;
};
inline constexpr unsafe_t unsafe{};

} // namespace bpl
