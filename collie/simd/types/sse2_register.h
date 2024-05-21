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


#ifndef COLLIE_SIMD_TYPES_SSE2_REGISTER_H_
#define COLLIE_SIMD_TYPES_SSE2_REGISTER_H_

#include <collie/simd/types/simd_generic_arch.h>
#include <collie/simd/types/register.h>

#if COLLIE_SIMD_WITH_SSE2
#include <emmintrin.h>
#include <xmmintrin.h>
#endif

namespace collie::simd {
    /**
     * @ingroup architectures
     *
     * SSE2 instructions
     */
    struct sse2 : generic {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WITH_SSE2; }
        static constexpr bool available() noexcept { return true; }
        static constexpr bool requires_alignment() noexcept { return true; }
        static constexpr unsigned version() noexcept { return generic::version(1, 2, 0); }
        static constexpr std::size_t alignment() noexcept { return 16; }
        static constexpr char const* name() noexcept { return "sse2"; }
    };

#if COLLIE_SIMD_WITH_SSE2
    namespace types
    {
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(signed char, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned char, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(char, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned short, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(short, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long long int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long long int, sse2, __m128i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(float, sse2, __m128);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(double, sse2, __m128d);
    }
#endif
}

#endif  // COLLIE_SIMD_TYPES_SSE2_REGISTER_H_
