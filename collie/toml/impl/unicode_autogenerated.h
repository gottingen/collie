//
// Copyright (C) 2024 EA group inc.
// Author: Jeff.li lijippy@163.com
// All rights reserved.
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#pragma once

#include <collie/toml/impl/preprocessor.h>
#include <collie/toml/impl/header_start.h>
/// \cond

#if TOML_GCC && TOML_GCC < 9
#pragma GCC push_options
#pragma GCC optimize("O1") // codegen bugs
#endif

// the functions in this namespace block are automatically generated by a tool - they are not meant to be hand-edited

TOML_IMPL_NAMESPACE_START
{
	TOML_CONST_GETTER
	constexpr bool is_ascii_horizontal_whitespace(char32_t c) noexcept
	{
		return c == U'\t' || c == U' ';
	}

	TOML_CONST_GETTER
	constexpr bool is_non_ascii_horizontal_whitespace(char32_t c) noexcept
	{
		// 20 code units from 8 ranges (spanning a search area of 65120)

		if (c < U'\xA0' || c > U'\uFEFF')
			return false;

		const auto child_index_0 = (static_cast<uint_least64_t>(c) - 0xA0ull) / 0x3FAull;
		if ((1ull << child_index_0) & 0x7FFFFFFFFFFFF75Eull)
			return false;
		if (c == U'\xA0' || c == U'\u3000' || c == U'\uFEFF')
			return true;
		switch (child_index_0)
		{
			case 0x05: return c == U'\u1680' || c == U'\u180E';
			case 0x07:
				return (U'\u2000' <= c && c <= U'\u200B') || (U'\u205F' <= c && c <= U'\u2060') || c == U'\u202F';
			default: TOML_UNREACHABLE;
		}

		TOML_UNREACHABLE;
	}

	TOML_CONST_GETTER
	constexpr bool is_ascii_vertical_whitespace(char32_t c) noexcept
	{
		return c >= U'\n' && c <= U'\r';
	}

	TOML_CONST_GETTER
	constexpr bool is_non_ascii_vertical_whitespace(char32_t c) noexcept
	{
		return (U'\u2028' <= c && c <= U'\u2029') || c == U'\x85';
	}

	TOML_CONST_GETTER
	constexpr bool is_ascii_bare_key_character(char32_t c) noexcept
	{
#if TOML_LANG_UNRELEASED // toml/issues/644 ('+' in bare keys)
		if TOML_UNLIKELY(c == U'+')
			return true;
#endif
		// 64 code units from 5 ranges (spanning a search area of 78)

		if (c < U'-' || c > U'z')
			return false;

		return (((static_cast<uint_least64_t>(c) - 0x2Dull) / 0x40ull) != 0ull)
			|| ((1ull << (static_cast<uint_least64_t>(c) - 0x2Dull)) & 0xFFF43FFFFFF01FF9ull);
	}

#if TOML_LANG_UNRELEASED // toml/pull/891 (unicode bare keys)

	TOML_CONST_GETTER
	constexpr bool is_non_ascii_bare_key_character(char32_t c) noexcept
	{
		// 971732 code units from 16 ranges (spanning a search area of 982862)

		if (c < U'\xB2' || c > U'\U000EFFFF')
			return false;

		const auto child_index_0 = (static_cast<uint_least64_t>(c) - 0xB2ull) / 0x3BFEull;
		if ((1ull << child_index_0) & 0xFFFFFFFFFFFFFFE6ull)
			return true;
		switch (child_index_0)
		{
			case 0x00: // [0] 00B2 - 3CAF
			{
				// 12710 code units from 13 ranges (spanning a search area of 15358)

				TOML_ASSUME(c >= U'\xB2' && c <= U'\u3CAF');

				constexpr uint_least64_t bitmask_table_1[] = {
					0xFFFFFFDFFFFFDC83u, 0xFFFFFFFFFFFFFFDFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFEFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0x000000000C003FFFu, 0xC000000000006000u, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0x000000003FFFFFFFu,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0xFFFFC00000000000u, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0x0000000000003FFFu, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u, 0x0000000000000000u,
					0x0000000000000000u, 0xFFFFFFFFFFFFC000u, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0x3FFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFF8000u, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu,
					0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0xFFFFFFFFFFFFFFFFu, 0x3FFFFFFFFFFFFFFFu,
				};
				return bitmask_table_1[(static_cast<uint_least64_t>(c) - 0xB2ull) / 0x40ull]
					 & (0x1ull << ((static_cast<uint_least64_t>(c) - 0xB2ull) % 0x40ull));
			}
			case 0x03: return c <= U'\uD7FF';
			case 0x04:
				return (U'\uF900' <= c && c <= U'\uFDCF') || (U'\uFDF0' <= c && c <= U'\uFFFD') || U'\U00010000' <= c;
			default: TOML_UNREACHABLE;
		}

		TOML_UNREACHABLE;
	}

#endif // TOML_LANG_UNRELEASED
}
TOML_IMPL_NAMESPACE_END;

#if TOML_GCC && TOML_GCC < 9
#pragma GCC pop_options
#endif

/// \endcond
#include <collie/toml/impl/header_end.h>
