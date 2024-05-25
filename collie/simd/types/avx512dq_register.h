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


#ifndef COLLIE_SIMD_TYPES_AVX512DQ_REGISTER_H_
#define COLLIE_SIMD_TYPES_AVX512DQ_REGISTER_H_

#include <collie/simd/types/avx512cd_register.h>

namespace collie::simd {

    /**
     * @ingroup architectures
     *
     * AVX512DQ instructions
     */
    struct avx512dq : avx512cd {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_AVX512DQ; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(3, 3, 0); }

        static constexpr char const *name() noexcept { return "avx512dq"; }
    };

#if COLLIE_SIMD_AVX512DQ

    namespace types
    {
        template <class T>
        struct get_bool_simd_register<T, avx512dq>
        {
            using type = simd_avx512_bool_register<T>;
        };

        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(avx512dq, avx512cd);

    }
#endif
}
#endif  // COLLIE_SIMD_TYPES_AVX512DQ_REGISTER_H_
