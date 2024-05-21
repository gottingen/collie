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

#ifndef COLLIE_SIMD_ARCH_GENERIC_COMPLEX_H_
#define COLLIE_SIMD_ARCH_GENERIC_COMPLEX_H_

#include <complex>
#include <collie/simd/arch/generic/details.h>

namespace collie::simd {

    namespace kernel {

        using namespace types;

        // real
        template<class A, class T>
        inline batch<T, A> real(batch<T, A> const &self, requires_arch<generic>) noexcept {
            return self;
        }

        template<class A, class T>
        inline batch<T, A> real(batch<std::complex<T>, A> const &self, requires_arch<generic>) noexcept {
            return self.real();
        }

        // imag
        template<class A, class T>
        inline batch<T, A> imag(batch<T, A> const & /*self*/, requires_arch<generic>) noexcept {
            return batch<T, A>(T(0));
        }

        template<class A, class T>
        inline batch<T, A> imag(batch<std::complex<T>, A> const &self, requires_arch<generic>) noexcept {
            return self.imag();
        }

        // arg
        template<class A, class T>
        inline real_batch_type_t<batch<T, A>> arg(batch<T, A> const &self, requires_arch<generic>) noexcept {
            return atan2(imag(self), real(self));
        }

        // conj
        template<class A, class T>
        inline complex_batch_type_t<batch<T, A>> conj(batch<T, A> const &self, requires_arch<generic>) noexcept {
            return {real(self), -imag(self)};
        }

        // norm
        template<class A, class T>
        inline real_batch_type_t<batch<T, A>> norm(batch<T, A> const &self, requires_arch<generic>) noexcept {
            return {fma(real(self), real(self), imag(self) * imag(self))};
        }

        // proj
        template<class A, class T>
        inline complex_batch_type_t<batch<T, A>> proj(batch<T, A> const &self, requires_arch<generic>) noexcept {
            using batch_type = complex_batch_type_t<batch<T, A>>;
            using real_batch = typename batch_type::real_batch;
            using real_value_type = typename real_batch::value_type;
            auto cond = collie::simd::isinf(real(self)) || collie::simd::isinf(imag(self));
            return select(cond,
                          batch_type(constants::infinity<real_batch>(),
                                     copysign(real_batch(real_value_type(0)), imag(self))),
                          batch_type(self));
        }

        template<class A, class T>
        inline batch_bool<T, A> isnan(batch<std::complex<T>, A> const &self, requires_arch<generic>) noexcept {
            return batch_bool<T, A>(isnan(self.real()) || isnan(self.imag()));
        }

        template<class A, class T>
        inline batch_bool<T, A> isinf(batch<std::complex<T>, A> const &self, requires_arch<generic>) noexcept {
            return batch_bool<T, A>(isinf(self.real()) || isinf(self.imag()));
        }

        template<class A, class T>
        inline batch_bool<T, A> isfinite(batch<std::complex<T>, A> const &self, requires_arch<generic>) noexcept {
            return batch_bool<T, A>(isfinite(self.real()) && isfinite(self.imag()));
        }
    }
}

#endif  // COLLIE_SIMD_ARCH_GENERIC_COMPLEX_H_
