// Copyright © 2025 Luca Valsassina
// SPDX-License-Identifier: MIT

#include <bpl/literals.hpp>

#include <gtest/gtest.h>

using namespace bpl::literals;

// Binary prefixes (IEC 60027-2)

TEST(literals, kibibytes) {
	// 0 KiB
	static_assert(0_KiB == 0u);

	// 1 KiB
	static_assert(1_KiB == 1'024u);

	// (2^54 - 1) KiB
	static_assert(18'014'398'509'481'983_KiB == 18'446'744'073'709'550'592u);

	// 2^54 KiB
	EXPECT_DEATH(18'014'398'509'481'984_KiB, "");
}

TEST(literals, mibibytes) {
	// 0 MiB
	static_assert(0_MiB == 0u);

	// 1 MiB
	static_assert(1_MiB == 1'048'576u);

	// (2^44 - 1) MiB
	static_assert(17'592'186'044'415_MiB == 18'446'744'073'708'503'040u);

	// 2^44 MiB
	EXPECT_DEATH(17'592'186'044'416_MiB, "");
}

TEST(literals, gibibytes) {
	// 0 GiB
	static_assert(0_GiB == 0u);

	// 1 GiB
	static_assert(1_GiB == 1'073'741'824u);

	// (2^34 - 1) GiB
	static_assert(17'179'869'183_GiB == 18'446'744'072'635'809'792u);

	// 2^34 GiB
	EXPECT_DEATH(17'179'869'184_GiB, "");
}

TEST(literals, tebibytes) {
	// 0 TiB
	static_assert(0_TiB == 0u);

	// 1 TiB
	static_assert(1_TiB == 1'099'511'627'776u);

	// (2^24 - 1) TiB
	static_assert(16'777'215_TiB == 18'446'742'974'197'923'840u);

	// 2^24 TiB
	EXPECT_DEATH(16'777'216_TiB, "");
}

TEST(literals, pebibytes) {
	// 0 PiB
	static_assert(0_PiB == 0u);

	// 1 TiB
	static_assert(1_PiB == 1'125'899'906'842'624u);

	// (2^14 - 1) PiB
	static_assert(16'383_PiB == 18'445'618'173'802'708'992u);

	// 2^14 PiB
	EXPECT_DEATH(16'777'216_PiB, "");
}

TEST(literals, exbibytes) {
	// 0 EiB
	static_assert(0_EiB == 0u);

	// 1 EiB
	static_assert(1_EiB == 1'152'921'504'606'846'976u);

	// (2^4 - 1) EiB
	static_assert(15_EiB == 17'293'822'569'102'704'640u);

	// 2^4 EiB
	EXPECT_DEATH(16_EiB, "");
}

// SI prefixes

TEST(literals, kilobytes) {
	// 0 kB
	static_assert(0_kB == 0u);

	// 1 kB
	static_assert(1_kB == 1'000u);

	// ((2^64 - 1) / 10^3) kB
	static_assert(18'446'744'073'709'551_kB == 18'446'744'073'709'551'000u);

	EXPECT_DEATH(18'446'744'073'709'552_kB, "");
}

TEST(literals, megabytes) {
	// 0 MB
	static_assert(0_MB == 0u);

	// 1 MB
	static_assert(1_MB == 1'000'000u);

	// ((2^64 - 1) / 10^6) MB
	static_assert(18'446'744'073'709_MB == 18'446'744'073'709'000'000u);

	EXPECT_DEATH(18'446'744'073'710_MB, "");
}

TEST(literals, gigabytes) {
	// 0 GB
	static_assert(0_GB == 0u);

	// 1 GB
	static_assert(1_GB == 1'000'000'000u);

	// ((2^64 - 1) / 10^9) GB
	static_assert(18'446'744'073_GB == 18'446'744'073'000'000'000u);

	EXPECT_DEATH(18'446'744'074_GB, "");
}

TEST(literals, terabytes) {
	// 0 TB
	static_assert(0_TB == 0u);

	// 1 TB
	static_assert(1_TB == 1'000'000'000'000u);

	// ((2^64 - 1) / 10^12) TB
	static_assert(18'446'744_TB == 18'446'744'000'000'000'000u);

	EXPECT_DEATH(18'446'745_TB, "");
}

TEST(literals, petabytes) {
	// 0 PB
	static_assert(0_PB == 0u);

	// 1 PB
	static_assert(1_PB == 1'000'000'000'000'000u);

	// ((2^64 - 1) / 10^15) PB
	static_assert(18'446_PB == 18'446'000'000'000'000'000u);

	EXPECT_DEATH(18'447_PB, "");
}

TEST(literals, exabytes) {
	// 0 EB
	static_assert(0_EB == 0u);

	// 1 EB
	static_assert(1_EB == 1'000'000'000'000'000'000u);

	// ((2^64 - 1) / 10^18) EB
	static_assert(18_EB == 18'000'000'000'000'000'000u);

	EXPECT_DEATH(19_EB, "");
}
