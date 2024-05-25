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


#ifndef COLLIE_SIMD_TYPES_AVX_REGISTER_H_
#define COLLIE_SIMD_TYPES_AVX_REGISTER_H_

#include <collie/simd/types/simd_generic_arch.h>

namespace collie::simd {

    /**
     * @ingroup architectures
     *
     * AVX instructions
     */
    struct avx : generic {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WITH_AVX; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(2, 1, 0); }

        static constexpr std::size_t alignment() noexcept { return 32; }

        static constexpr bool requires_alignment() noexcept { return true; }

        static constexpr char const *name() noexcept { return "avx"; }
    };
}

#if COLLIE_SIMD_WITH_AVX

#include <immintrin.h>

namespace collie::simd {
    namespace types {

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(signed char, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned char, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(char, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned short, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(short, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long long int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long long int, avx, __m256i);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(float, avx, __m256);

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(double, avx, __m256d);
    }
}
#endif
#endif  // COLLIE_SIMD_TYPES_AVX_REGISTER_H_
