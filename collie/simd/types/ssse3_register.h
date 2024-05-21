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

#ifndef COLLIE_SIMD_TYPES_SSSE3_REGISTER_H_
#define COLLIE_SIMD_TYPES_SSSE3_REGISTER_H_

#include <collie/simd/types/sse3_register.h>

#if COLLIE_SIMD_WITH_SSSE3

#include <tmmintrin.h>

#endif

namespace collie::simd {
    /**
     * @ingroup architectures
     *
     * SSSE3 instructions
     */
    struct ssse3 : sse3 {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WITH_SSSE3; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(1, 3, 1); }

        static constexpr char const *name() noexcept { return "ssse3"; }
    };

#if COLLIE_SIMD_WITH_SSSE3
    namespace types {
        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(ssse3, sse3);
    }
#endif
}

#endif  // COLLIE_SIMD_TYPES_SSSE3_REGISTER_H_
