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

#ifndef COLLIE_SIMD_ARCH_FMA3_AVX_H_
#define COLLIE_SIMD_ARCH_FMA3_AVX_H_

#include <collie/simd/types/fma3_avx_register.h>

namespace collie::simd {

    namespace kernel {
        using namespace types;

        // fnma
        template<class A>
        inline batch<float, A> fnma(batch<float, A> const &x, batch<float, A> const &y, batch<float, A> const &z,
                                    requires_arch<fma3<avx>>) noexcept {
            return _mm256_fnmadd_ps(x, y, z);
        }

        template<class A>
        inline batch<double, A> fnma(batch<double, A> const &x, batch<double, A> const &y, batch<double, A> const &z,
                                     requires_arch<fma3<avx>>) noexcept {
            return _mm256_fnmadd_pd(x, y, z);
        }

        // fnms
        template<class A>
        inline batch<float, A> fnms(batch<float, A> const &x, batch<float, A> const &y, batch<float, A> const &z,
                                    requires_arch<fma3<avx>>) noexcept {
            return _mm256_fnmsub_ps(x, y, z);
        }

        template<class A>
        inline batch<double, A> fnms(batch<double, A> const &x, batch<double, A> const &y, batch<double, A> const &z,
                                     requires_arch<fma3<avx>>) noexcept {
            return _mm256_fnmsub_pd(x, y, z);
        }

        // fma
        template<class A>
        inline batch<float, A> fma(batch<float, A> const &x, batch<float, A> const &y, batch<float, A> const &z,
                                   requires_arch<fma3<avx>>) noexcept {
            return _mm256_fmadd_ps(x, y, z);
        }

        template<class A>
        inline batch<double, A> fma(batch<double, A> const &x, batch<double, A> const &y, batch<double, A> const &z,
                                    requires_arch<fma3<avx>>) noexcept {
            return _mm256_fmadd_pd(x, y, z);
        }

        // fms
        template<class A>
        inline batch<float, A> fms(batch<float, A> const &x, batch<float, A> const &y, batch<float, A> const &z,
                                   requires_arch<fma3<avx>>) noexcept {
            return _mm256_fmsub_ps(x, y, z);
        }

        template<class A>
        inline batch<double, A> fms(batch<double, A> const &x, batch<double, A> const &y, batch<double, A> const &z,
                                    requires_arch<fma3<avx>>) noexcept {
            return _mm256_fmsub_pd(x, y, z);
        }

    }

}  // namespace collie::simd

#endif  // COLLIE_SIMD_ARCH_FMA3_AVX_H_
