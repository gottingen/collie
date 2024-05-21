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

#ifndef COLLIE_SIMD_TYPES_WASM_REGISTER_H_
#define COLLIE_SIMD_TYPES_WASM_REGISTER_H_

#include <collie/simd/types/simd_generic_arch.h>
#include <collie/simd/types/register.h>

#if COLLIE_SIMD_WASM
#include <wasm_simd128.h>
#endif

namespace collie::simd {
    /**
     * @ingroup architectures
     *
     * WASM instructions
     */
    struct wasm : generic {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_WASM; }

        static constexpr bool available() noexcept { return true; }

        static constexpr bool requires_alignment() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(10, 0, 0); }

        static constexpr std::size_t alignment() noexcept { return 16; }

        static constexpr char const *name() noexcept { return "wasm"; }
    };

#if COLLIE_SIMD_WASM
    namespace types {
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(signed char, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned char, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(char, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned short, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(short, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long long int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long long int, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(float, wasm, v128_t);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(double, wasm, v128_t);
    }
#endif
}

#endif  // COLLIE_SIMD_TYPES_WASM_REGISTER_H_
