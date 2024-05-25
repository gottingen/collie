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
#if TOML_ENABLE_SIMD

#if defined(__SSE2__)                                                                                                  \
	|| (defined(_MSC_VER) && (defined(_M_AMD64) || defined(_M_X64) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)))
#define TOML_HAS_SSE2 1
#endif

#if defined(__SSE4_1__) || (defined(_MSC_VER) && (defined(__AVX__) || defined(__AVX2__)))
#define TOML_HAS_SSE4_1 1
#endif

#endif // TOML_ENABLE_SIMD

#ifndef TOML_HAS_SSE2
#define TOML_HAS_SSE2 0
#endif
#ifndef TOML_HAS_SSE4_1
#define TOML_HAS_SSE4_1 0
#endif

TOML_DISABLE_WARNINGS;
#if TOML_HAS_SSE4_1
#include <smmintrin.h>
#endif
#if TOML_HAS_SSE2
#include <emmintrin.h>
#endif
TOML_ENABLE_WARNINGS;
