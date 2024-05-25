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

#ifndef COLLIE_SIMD_TYPES_AVX512PF_REGISTER_H_
#define COLLIE_SIMD_TYPES_AVX512PF_REGISTER_H_

#include <collie/simd/types/avx512er_register.h>

namespace collie::simd {

    /**
     * @ingroup architectures
     *
     * AVX512BW instructions
     */
    struct avx512pf : avx512er {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_AVX512PF; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(3, 4, 1); }

        static constexpr char const *name() noexcept { return "avx512pf"; }
    };

#if COLLIE_SIMD_AVX512PF

    namespace types
    {
        template <class T>
        struct get_bool_simd_register<T, avx512pf>
        {
            using type = simd_avx512_bool_register<T>;
        };

        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(avx512pf, avx512er);

    }
#endif
}
#endif  // COLLIE_SIMD_TYPES_AVX512PF_REGISTER_H_
