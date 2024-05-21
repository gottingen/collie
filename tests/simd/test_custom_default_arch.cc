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

#ifdef __STSE2__

#define COLLIE_SIMD_DEFAULT_ARCH collie::simd::sse2
#include <collie/simd/simd.h>

#include "test_utils.hpp"

// Could be different than sse2 if we compile for other architecture avx
static_assert(std::is_same<collie::simd::default_arch, collie::simd::sse2>::value, "default arch correctly hooked");

#else

#define COLLIE_SIMD_DEFAULT_ARCH collie::simd::unsupported
#include <collie/simd/simd.h>

#endif
