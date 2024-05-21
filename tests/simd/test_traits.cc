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


#include <collie/simd/simd.h>
#ifndef COLLIE_SIMD_NO_SUPPORTED_ARCHITECTURE

#include "test_utils.hpp"

template <class B>
struct traits_test
{
    using batch_type = B;
    using value_type = typename B::value_type;

    void test_simd_traits()
    {
        using traits_type = collie::simd::simd_traits<value_type>;
        CHECK_EQ(traits_type::size, batch_type::size);
        constexpr bool same_type = std::is_same<B, typename traits_type::type>::value;
        CHECK_UNARY(same_type);
        using batch_bool_type = collie::simd::batch_bool<value_type>;
        constexpr bool same_bool_type = std::is_same<batch_bool_type, typename traits_type::bool_type>::value;
        CHECK_UNARY(same_bool_type);

        using vector_traits_type = collie::simd::simd_traits<std::vector<value_type>>;
        CHECK_EQ(vector_traits_type::size, 1);
        constexpr bool vec_same_type = std::is_same<typename vector_traits_type::type, std::vector<value_type>>::value;
        CHECK_UNARY(vec_same_type);
    }

    void test_revert_simd_traits()
    {
        using traits_type = collie::simd::revert_simd_traits<batch_type>;
        CHECK_EQ(traits_type::size, batch_type::size);
        constexpr bool same_type = std::is_same<value_type, typename traits_type::type>::value;
        CHECK_UNARY(same_type);
    }

    void test_simd_return_type()
    {
        using rtype1 = collie::simd::simd_return_type<value_type, float>;
        constexpr bool res1 = std::is_same<rtype1, collie::simd::batch<float>>::value;
        CHECK_UNARY(res1);

        using rtype2 = collie::simd::simd_return_type<bool, value_type>;
        constexpr bool res2 = std::is_same<rtype2, collie::simd::batch_bool<value_type>>::value;
        CHECK_UNARY(res2);
    }

    void test_mask_type()
    {
        using mtype0 = collie::simd::mask_type_t<batch_type>;
        constexpr bool res0 = std::is_same<mtype0, collie::simd::batch_bool<collie::simd::scalar_type_t<batch_type>>>::value;
        CHECK_UNARY(res0);

        using mtype1 = collie::simd::mask_type_t<value_type>;
        constexpr bool res1 = std::is_same<mtype1, bool>::value;
        CHECK_UNARY(res1);
    }
};

TEST_CASE_TEMPLATE("[traits]", B, BATCH_TYPES)
{
    traits_test<B> Test;

    SUBCASE("simd_traits")
    {
        Test.test_simd_traits();
    }

    SUBCASE("revert_simd_traits")
    {
        Test.test_revert_simd_traits();
    }

    SUBCASE("simd_return_type")
    {
        Test.test_simd_return_type();
    }

    SUBCASE("mask_type")
    {
        Test.test_mask_type();
    }
}

template <class B>
struct complex_traits_test
{
    using batch_type = B;
    using value_type = typename B::value_type;

    void test_simd_traits()
    {
        using traits_type = collie::simd::simd_traits<value_type>;
        CHECK_EQ(traits_type::size, batch_type::size);
        constexpr bool same_type = std::is_same<B, typename traits_type::type>::value;
        CHECK_UNARY(same_type);
        using batch_bool_type = collie::simd::batch_bool<typename value_type::value_type>;
        constexpr bool same_bool_type = std::is_same<batch_bool_type, typename traits_type::bool_type>::value;
        CHECK_UNARY(same_bool_type);

        using vector_traits_type = collie::simd::simd_traits<std::vector<value_type>>;
        CHECK_EQ(vector_traits_type::size, 1);
        constexpr bool vec_same_type = std::is_same<typename vector_traits_type::type, std::vector<value_type>>::value;
        CHECK_UNARY(vec_same_type);
    }

    void test_revert_simd_traits()
    {
        using traits_type = collie::simd::revert_simd_traits<batch_type>;
        CHECK_EQ(traits_type::size, batch_type::size);
        constexpr bool same_type = std::is_same<value_type, typename traits_type::type>::value;
        CHECK_UNARY(same_type);
    }

    void test_simd_return_type()
    {
        using rtype1 = collie::simd::simd_return_type<value_type, float>;
        constexpr bool res1 = std::is_same<rtype1, collie::simd::batch<std::complex<float>>>::value;
        CHECK_UNARY(res1);

        using rtype2 = collie::simd::simd_return_type<bool, value_type>;
        constexpr bool res2 = std::is_same<rtype2, collie::simd::batch_bool<typename value_type::value_type>>::value;
        CHECK_UNARY(res2);
    }

    void test_mask_type()
    {
        using mtype0 = collie::simd::mask_type_t<batch_type>;
        constexpr bool res0 = std::is_same<mtype0, collie::simd::batch_bool<collie::simd::scalar_type_t<typename batch_type::real_batch::value_type>>>::value;
        CHECK_UNARY(res0);

        using mtype1 = collie::simd::mask_type_t<value_type>;
        constexpr bool res1 = std::is_same<mtype1, bool>::value;
        CHECK_UNARY(res1);
    }
};

TEST_CASE_TEMPLATE("[complex traits]", B, BATCH_COMPLEX_TYPES)
{
    complex_traits_test<B> Test;

    SUBCASE("simd_traits")
    {
        Test.test_simd_traits();
    }

    SUBCASE("revert_simd_traits")
    {
        Test.test_revert_simd_traits();
    }

    SUBCASE("simd_return_type")
    {
        Test.test_simd_return_type();
    }

    SUBCASE("mask_type")
    {
        Test.test_mask_type();
    }
}
#endif
