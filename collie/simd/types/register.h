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

#ifndef COLLIE_SIMD_TYPES_REGISTER_H_
#define COLLIE_SIMD_TYPES_REGISTER_H_

#include <type_traits>

namespace collie::simd {
    namespace types {
        template<class T, class A>
        struct has_simd_register : std::false_type {
        };

        template<class T, class Arch>
        struct simd_register {
            struct register_type {
            };
        };

#define COLLIE_SIMD_DECLARE_SIMD_REGISTER(SCALAR_TYPE, ISA, VECTOR_TYPE) \
    template <>                                                    \
    struct simd_register<SCALAR_TYPE, ISA>                         \
    {                                                              \
        using register_type = VECTOR_TYPE;                         \
        register_type data;                                        \
        inline operator register_type() const noexcept             \
        {                                                          \
            return data;                                           \
        }                                                          \
    };                                                             \
    template <>                                                    \
    struct has_simd_register<SCALAR_TYPE, ISA> : std::true_type    \
    {                                                              \
    }

#define COLLIE_SIMD_DECLARE_INVALID_SIMD_REGISTER(SCALAR_TYPE, ISA)    \
    template <>                                                  \
    struct has_simd_register<SCALAR_TYPE, ISA> : std::false_type \
    {                                                            \
    }

#define COLLIE_SIMD_DECLARE_SIMD_REGISTER_ALIAS(ISA, ISA_BASE)                          \
    template <class T>                                                            \
    struct simd_register<T, ISA> : simd_register<T, ISA_BASE>                     \
    {                                                                             \
        using register_type = typename simd_register<T, ISA_BASE>::register_type; \
        simd_register(register_type reg) noexcept                                 \
            : simd_register<T, ISA_BASE> { reg }                                  \
        {                                                                         \
        }                                                                         \
        simd_register() = default;                                                \
    };                                                                            \
    template <class T>                                                            \
    struct has_simd_register<T, ISA> : has_simd_register<T, ISA_BASE>             \
    {                                                                             \
    }

        template<class T, class Arch>
        struct get_bool_simd_register {
            using type = simd_register<T, Arch>;
        };

        template<class T, class Arch>
        using get_bool_simd_register_t = typename get_bool_simd_register<T, Arch>::type;
    }

    namespace kernel {
        template<class A>
            // makes requires_arch equal to A const&, using type_traits functions
        using requires_arch = typename std::add_lvalue_reference<typename std::add_const<A>::type>::type;
        template<class T>
        struct convert {
        };
    }
}

#endif  // COLLIE_SIMD_TYPES_REGISTER_H_
