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

#ifndef COLLIE_SIMD_ARCH_SSE3_H_
#define COLLIE_SIMD_ARCH_SSE3_H_

#include <collie/simd/types/sse3_register.h>
#include <type_traits>

namespace collie::simd {

    namespace kernel {
        using namespace types;

        // haddp
        template<class A>
        inline batch<float, A> haddp(batch<float, A> const *row, requires_arch<sse3>) noexcept {
            return _mm_hadd_ps(_mm_hadd_ps(row[0], row[1]),
                               _mm_hadd_ps(row[2], row[3]));
        }

        template<class A>
        inline batch<double, A> haddp(batch<double, A> const *row, requires_arch<sse3>) noexcept {
            return _mm_hadd_pd(row[0], row[1]);
        }

        // load_unaligned
        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> load_unaligned(T const *mem, convert<T>, requires_arch<sse3>) noexcept {
            return _mm_lddqu_si128((__m128i const *) mem);
        }

        // reduce_add
        template<class A>
        inline float reduce_add(batch<float, A> const &self, requires_arch<sse3>) noexcept {
            __m128 tmp0 = _mm_hadd_ps(self, self);
            __m128 tmp1 = _mm_hadd_ps(tmp0, tmp0);
            return _mm_cvtss_f32(tmp1);
        }

        template<class A>
        inline double reduce_add(batch<double, A> const &self, requires_arch<sse3>) noexcept {
            __m128d tmp0 = _mm_hadd_pd(self, self);
            return _mm_cvtsd_f64(tmp0);
        }

    }

}  // namespace collie::simd

#endif  // COLLIE_SIMD_ARCH_SSE3_H_
