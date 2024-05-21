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
struct batch_float_test
{
    using batch_type = B;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;
    using array_type = std::array<value_type, size>;
    using bool_array_type = std::array<bool, size>;

    array_type lhs;
    array_type rhs;

    batch_float_test()
    {
        for (size_t i = 0; i < size; ++i)
        {
            lhs[i] = value_type(i) / 4 + value_type(1.2) * std::sqrt(value_type(i + 0.25));
            if (lhs[i] == value_type(0))
            {
                lhs[i] += value_type(0.1);
            }
            rhs[i] = value_type(10.2) / (i + 2) + value_type(0.25);
        }
    }

    void test_reciprocal() const
    {
        // reciprocal
        {
            array_type res, expected;
            std::transform(lhs.cbegin(), lhs.cend(), expected.begin(),
                           [](const value_type& l)
                           { return value_type(1) / l; });
            batch_type res1 = reciprocal(batch_lhs());
            res1.store_unaligned(res.data());
            size_t diff = detail::get_nb_diff_near(res, expected, 1e-12f);
            INFO("reciprocal");
            CHECK_EQ(diff, 0);
        }
    }

    void test_rsqrt() const
    {
        // rsqrt
        {
            array_type res, expected;
            std::transform(lhs.cbegin(), lhs.cend(), expected.begin(),
                           [](const value_type& l)
                           { return std::ceil((value_type(1) / std::sqrt(l)) * value_type(100)); });
            batch_type res1 = ceil(rsqrt(batch_lhs()) * value_type(100));
            res1.store_unaligned(res.data());
            size_t diff = detail::get_nb_diff_near(res, expected, 1.5f * std::pow(2, 12));
            INFO("rsqrt");
            CHECK_EQ(diff, 0);
        }
    }

    void test_sqrt() const
    {
        // sqrt
        {
            array_type expected;
            std::transform(lhs.cbegin(), lhs.cend(), expected.begin(),
                           [](const value_type& l)
                           { return std::sqrt(l); });
            batch_type res = sqrt(batch_lhs());
            INFO("sqrt");
            CHECK_BATCH_EQ(res, expected);
        }
    }

    void test_haddp() const
    {
        batch_type haddp_input[size];
        for (size_t i = 0; i < size; i += 2)
        {
            haddp_input[i] = batch_lhs();
            if (i + 1 < size)
            {
                haddp_input[i + 1] = batch_rhs();
            }
        }
        array_type expected;
        std::fill(expected.begin(), expected.end(), value_type(0));
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < size; j += 2)
            {
                expected[j] += lhs[i];
                if (j + 1 < size)
                {
                    expected[j + 1] += rhs[i];
                }
            }
        }
        auto res = haddp(haddp_input);
        INFO("haddp");
        CHECK_BATCH_EQ(res, expected);
    }

private:
    batch_type batch_lhs() const
    {
        return batch_type::load_unaligned(lhs.data());
    }

    batch_type batch_rhs() const
    {
        return batch_type::load_unaligned(rhs.data());
    }
};

TEST_CASE_TEMPLATE("[simd batch float]", B, BATCH_FLOAT_TYPES)
{
    batch_float_test<B> Test;

    SUBCASE("reciprocal") { Test.test_reciprocal(); }

    SUBCASE("sqrt") { Test.test_sqrt(); }

    SUBCASE("rsqrt") { Test.test_rsqrt(); }

    SUBCASE("haddp") { Test.test_haddp(); }
}
#endif
