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

#ifndef COLLIE_SIMD_TYPES_FMA3_AVX2_REGISTER_H_
#define COLLIE_SIMD_TYPES_FMA3_AVX2_REGISTER_H_

#include <collie/simd/types/avx2_register.h>

namespace collie::simd {
    template<typename arch>
    struct fma3;

    /**
     * @ingroup architectures
     *
     * AVX2 + FMA instructions
     */
    template<>
    struct fma3<avx2> : avx2 {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WITH_FMA3_AVX2; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(2, 2, 1); }

        static constexpr char const *name() noexcept { return "fma3+avx2"; }
    };

#if COLLIE_SIMD_WITH_FMA3_AVX2
    namespace types {

        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(fma3<avx2>, avx2);

    }
#endif

}
#endif  // COLLIE_SIMD_TYPES_FMA3_AVX2_REGISTER_H_
