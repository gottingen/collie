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

#ifndef COLLIE_SIMD_TYPES_AVX512F_REGISTER_H_
#define COLLIE_SIMD_TYPES_AVX512F_REGISTER_H_

#include <collie/simd/types/simd_generic_arch.h>

namespace collie::simd {

    /**
     * @ingroup architectures
     *
     * AVX512F instructions
     */
    struct avx512f : generic {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_AVX512F; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(3, 1, 0); }

        static constexpr std::size_t alignment() noexcept { return 64; }

        static constexpr bool requires_alignment() noexcept { return true; }

        static constexpr char const *name() noexcept { return "avx512f"; }
    };

#if COLLIE_SIMD_AVX512F

    namespace types
    {
        template <class T>
        struct simd_avx512_bool_register
        {
            using register_type = typename std::conditional<
                (sizeof(T) < 4), std::conditional<(sizeof(T) == 1), __mmask64, __mmask32>,
                std::conditional<(sizeof(T) == 4), __mmask16, __mmask8>>::type::type;
            register_type data;
            simd_avx512_bool_register() = default;
            simd_avx512_bool_register(register_type r) { data = r; }
            operator register_type() const noexcept { return data; }
        };
        template <class T>
        struct get_bool_simd_register<T, avx512f>
        {
            using type = simd_avx512_bool_register<T>;
        };

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(signed char, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned char, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(char, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned short, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(short, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long long int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long long int, avx512f, __m512i);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(float, avx512f, __m512);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(double, avx512f, __m512d);

    }
#endif
}

#endif  // COLLIE_SIMD_TYPES_AVX512F_REGISTER_H_
