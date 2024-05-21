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


#ifndef COLLIE_SIMD_TYPES_SVE_REGISTER_H_
#define COLLIE_SIMD_TYPES_SVE_REGISTER_H_

#include <collie/simd/types/simd_generic_arch.h>
#include <collie/simd/types/register.h>

#if COLLIE_SIMD_WITH_SVE
#include <arm_sve.h>
#endif

namespace collie::simd {
    namespace detail {
        /**
         * @ingroup architectures
         *
         * SVE instructions (fixed vector size) for arm64
         */
        template<size_t Width>
        struct sve : collie::simd::generic {
            static constexpr bool supported() noexcept { return Width == COLLIE_SIMD_SVE_BITS; }

            static constexpr bool available() noexcept { return true; }

            static constexpr bool requires_alignment() noexcept { return true; }

            static constexpr std::size_t alignment() noexcept { return 16; }

            static constexpr unsigned version() noexcept { return generic::version(9, Width / 32, 0); }

            static constexpr char const *name() noexcept { return "arm64+sve"; }
        };
    }

#if COLLIE_SIMD_WITH_SVE

    using sve = detail::sve<__ARM_FEATURE_SVE_BITS>;

    namespace types
    {
        namespace detail
        {
// define fixed size alias per SVE sizeless type
#define SVE_TO_FIXED_SIZE(ty) ty __attribute__((arm_sve_vector_bits(__ARM_FEATURE_SVE_BITS)))
            using sve_int8_t = SVE_TO_FIXED_SIZE(svint8_t);
            using sve_uint8_t = SVE_TO_FIXED_SIZE(svuint8_t);
            using sve_int16_t = SVE_TO_FIXED_SIZE(svint16_t);
            using sve_uint16_t = SVE_TO_FIXED_SIZE(svuint16_t);
            using sve_int32_t = SVE_TO_FIXED_SIZE(svint32_t);
            using sve_uint32_t = SVE_TO_FIXED_SIZE(svuint32_t);
            using sve_int64_t = SVE_TO_FIXED_SIZE(svint64_t);
            using sve_uint64_t = SVE_TO_FIXED_SIZE(svuint64_t);
            using sve_float32_t = SVE_TO_FIXED_SIZE(svfloat32_t);
            using sve_float64_t = SVE_TO_FIXED_SIZE(svfloat64_t);
            using sve_bool_t = SVE_TO_FIXED_SIZE(svbool_t);
#undef SVE_TO_FIXED_SIZE

            template <size_t S>
            struct sve_vector_type_impl;

            template <>
            struct sve_vector_type_impl<8>
            {
                using signed_type = sve_int8_t;
                using unsigned_type = sve_uint8_t;
                using floating_point_type = void;
            };

            template <>
            struct sve_vector_type_impl<16>
            {
                using signed_type = sve_int16_t;
                using unsigned_type = sve_uint16_t;
                using floating_point_type = void;
            };

            template <>
            struct sve_vector_type_impl<32>
            {
                using signed_type = sve_int32_t;
                using unsigned_type = sve_uint32_t;
                using floating_point_type = sve_float32_t;
            };

            template <>
            struct sve_vector_type_impl<64>
            {
                using signed_type = sve_int64_t;
                using unsigned_type = sve_uint64_t;
                using floating_point_type = sve_float64_t;
            };

            template <class T>
            using signed_int_sve_vector_type = typename sve_vector_type_impl<8 * sizeof(T)>::signed_type;

            template <class T>
            using unsigned_int_sve_vector_type = typename sve_vector_type_impl<8 * sizeof(T)>::unsigned_type;

            template <class T>
            using floating_point_sve_vector_type = typename sve_vector_type_impl<8 * sizeof(T)>::floating_point_type;

            template <class T>
            using signed_int_or_floating_point_sve_vector_type = typename std::conditional<std::is_floating_point<T>::value,
                                                                                           floating_point_sve_vector_type<T>,
                                                                                           signed_int_sve_vector_type<T>>::type;

            template <class T>
            using sve_vector_type = typename std::conditional<std::is_signed<T>::value,
                                                              signed_int_or_floating_point_sve_vector_type<T>,
                                                              unsigned_int_sve_vector_type<T>>::type;
        } // namespace detail

        COLLIE_SIMD_DECLARE_SIMD_REGISTER(signed char, sve, detail::sve_vector_type<signed char>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned char, sve, detail::sve_vector_type<unsigned char>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(char, sve, detail::sve_vector_type<char>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(short, sve, detail::sve_vector_type<short>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned short, sve, detail::sve_vector_type<unsigned short>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(int, sve, detail::sve_vector_type<int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned int, sve, detail::sve_vector_type<unsigned int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long int, sve, detail::sve_vector_type<long int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long int, sve, detail::sve_vector_type<unsigned long int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(long long int, sve, detail::sve_vector_type<long long int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(unsigned long long int, sve, detail::sve_vector_type<unsigned long long int>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(float, sve, detail::sve_vector_type<float>);
        COLLIE_SIMD_DECLARE_SIMD_REGISTER(double, sve, detail::sve_vector_type<double>);

        namespace detail
        {
            struct sve_bool_simd_register
            {
                using register_type = sve_bool_t;
                register_type data;
                operator register_type() const noexcept { return data; }
            };
        } // namespace detail

        template <class T>
        struct get_bool_simd_register<T, sve>
        {
            using type = detail::sve_bool_simd_register;
        };
    } // namespace types
#endif
} // namespace collie::simd

#endif  // COLLIE_SIMD_TYPES_SVE_REGISTER_H_
