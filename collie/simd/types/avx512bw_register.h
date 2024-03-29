// Copyright 2024 The Elastic-AI Authors.
// part of Elastic AI Search
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef COLLIE_SIMD_TYPES_AVX512BW_REGISTER_H_
#define COLLIE_SIMD_TYPES_AVX512BW_REGISTER_H_

#include <collie/simd/types/avx512dq_register.h>

namespace collie::simd {

    /**
     * @ingroup architectures
     *
     * AVX512BW instructions
     */
    struct avx512bw : avx512dq {
        static constexpr bool supported() noexcept { return COLLIE_SIMD_AVX512BW; }

        static constexpr bool available() noexcept { return true; }

        static constexpr unsigned version() noexcept { return generic::version(3, 4, 0); }

        static constexpr char const *name() noexcept { return "avx512bw"; }
    };

#if COLLIE_SIMD_AVX512BW

    namespace types
    {
        template <class T>
        struct get_bool_simd_register<T, avx512bw>
        {
            using type = simd_avx512_bool_register<T>;
        };

        COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(avx512bw, avx512dq);

    }
#endif
}
#endif  // COLLIE_SIMD_TYPES_AVX512BW_REGISTER_H_