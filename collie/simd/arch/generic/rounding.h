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

#ifndef COLLIE_SIMD_ARCH_GENERIC_ROUNDING_H_
#define COLLIE_SIMD_ARCH_GENERIC_ROUNDING_H_

#include <collie/simd/arch/generic/details.h>

namespace collie::simd {

    namespace kernel {

        using namespace types;

        // ceil
        template<class A, class T>
        inline batch<T, A> ceil(batch<T, A> const &self, requires_arch<generic>) noexcept {
            batch<T, A> truncated_self = trunc(self);
            return select(truncated_self < self, truncated_self + 1, truncated_self);
        }

        // floor
        template<class A, class T>
        inline batch<T, A> floor(batch<T, A> const &self, requires_arch<generic>) noexcept {
            batch<T, A> truncated_self = trunc(self);
            return select(truncated_self > self, truncated_self - 1, truncated_self);
        }

        // round
        template<class A, class T>
        inline batch<T, A> round(batch<T, A> const &self, requires_arch<generic>) noexcept {
            auto v = abs(self);
            auto c = ceil(v);
            auto cp = select(c - 0.5 > v, c - 1, c);
            return select(v > constants::maxflint<batch<T, A>>(), self, copysign(cp, self));
        }

        // trunc
        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> trunc(batch<T, A> const &self, requires_arch<generic>) noexcept {
            return self;
        }

        template<class A>
        inline batch<float, A> trunc(batch<float, A> const &self, requires_arch<generic>) noexcept {
            return select(abs(self) < constants::maxflint<batch<float, A>>(), to_float(to_int(self)), self);
        }

        template<class A>
        inline batch<double, A> trunc(batch<double, A> const &self, requires_arch<generic>) noexcept {
            return select(abs(self) < constants::maxflint<batch<double, A>>(), to_float(to_int(self)), self);
        }

    }

}

#endif  // COLLIE_SIMD_ARCH_GENERIC_ROUNDING_H_
