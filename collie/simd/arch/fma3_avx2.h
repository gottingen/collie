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

#ifndef COLLIE_SIMD_ARCH_FMA3_AVX2_H_
#define COLLIE_SIMD_ARCH_FMA3_AVX2_H_

#include <collie/simd/types/fma3_avx2_register.h>

// Allow inclusion of fma3_avx.hpp
#ifdef COLLIE_SIMD_ARCH_FMA3_AVX_H_
#undef COLLIE_SIMD_ARCH_FMA3_AVX_H_
#define COLLIE_SIMD_FORCE_FMA3_AVX_H_
#endif

// Disallow inclusion of ./fma3_avx_register.hpp
#ifndef COLLIE_SIMD_TYPES_FMA3_AVX_REGISTER_H_
#define COLLIE_SIMD_TYPES_FMA3_AVX_REGISTER_H_
#define COLLIE_SIMD_FORCE_FMA3_AVX_REGISTER_H_
#endif

// Include ./fma3_avx.hpp but s/avx/avx2
#define avx avx2
#include <collie/simd/arch/fma3_avx.h>
#undef avx
#undef COLLIE_SIMD_ARCH_FMA3_AVX_H_

// Carefully restore guards
#ifdef COLLIE_SIMD_FORCE_FMA3_AVX_H_
#define COLLIE_SIMD_ARCH_FMA3_AVX_H_
#undef COLLIE_SIMD_FORCE_FMA3_AVX_H_
#endif

#ifdef COLLIE_SIMD_FORCE_FMA3_AVX_REGISTER_H_
#undef COLLIE_SIMD_TYPES_FMA3_AVX_REGISTER_H_
#undef COLLIE_SIMD_FORCE_FMA3_AVX_REGISTER_H_
#endif

#endif  // COLLIE_SIMD_ARCH_FMA3_AVX2_H_
