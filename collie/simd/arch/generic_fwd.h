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

#ifndef COLLIE_SIMD_ARCH_GENERIC_FWD_H_
#define COLLIE_SIMD_ARCH_GENERIC_FWD_H_

#include <collie/simd/types/batch_constant.h>

#include <type_traits>

namespace collie::simd {
    namespace kernel {
        // forward declaration
        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> abs(batch<T, A> const &self, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A>
        bitwise_lshift(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A>
        bitwise_rshift(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T>
        inline batch_bool<T, A> gt(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> mul(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> sadd(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline batch<T, A> ssub(batch<T, A> const &self, batch<T, A> const &other, requires_arch<generic>) noexcept;

        template<class A, class T, class = typename std::enable_if<std::is_integral<T>::value, void>::type>
        inline T hadd(batch<T, A> const &self, requires_arch<generic>) noexcept;

    }
}

#endif  // COLLIE_SIMD_ARCH_GENERIC_FWD_H_
