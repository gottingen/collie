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

#ifndef COLLIE_SIMD_ARCH_SSE4_2_H_
#define COLLIE_SIMD_ARCH_SSE4_2_H_

#include <limits>

#include <collie/simd/types/sse4_2_register.h>

namespace collie::simd {

    namespace kernel {
        using namespace types;

        // lt
        template<class A>
        inline batch_bool<int64_t, A>
        lt(batch<int64_t, A> const &self, batch<int64_t, A> const &other, requires_arch<sse4_2>) noexcept {
            return _mm_cmpgt_epi64(other, self);
        }

        template<class A>
        inline batch_bool<uint64_t, A>
        lt(batch<uint64_t, A> const &self, batch<uint64_t, A> const &other, requires_arch<sse4_2>) noexcept {
            auto xself = _mm_xor_si128(self, _mm_set1_epi64x(std::numeric_limits<int64_t>::lowest()));
            auto xother = _mm_xor_si128(other, _mm_set1_epi64x(std::numeric_limits<int64_t>::lowest()));
            return _mm_cmpgt_epi64(xother, xself);
        }

    }

}  // namespace collie::simd

#endif  // COLLIE_SIMD_ARCH_SSE4_2_H_
