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


#ifndef COLLIE_SIMD_TYPES_SSE4_2_REGISTER_H_
#define COLLIE_SIMD_TYPES_SSE4_2_REGISTER_H_

#include <collie/simd/types/sse4_1_register.h>

#if COLLIE_SIMD_WITH_SSE4_2

#include <nmmintrin.h>

#endif

namespace collie::simd {
    /**
     * @ingroup architectures
     *
     * SSE4.2 instructions
     */
    struct sse4_2 : sse4_1 {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WITH_SSE4_2; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(1, 4, 2); }

        static constexpr char const *name() noexcept { return "sse4.2"; }
    };

#if COLLIE_SIMD_WITH_SSE4_2
    namespace types {
        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(sse4_2, sse4_1);
    }
#endif
}

#endif  // COLLIE_SIMD_TYPES_SSE4_2_REGISTER_H_
