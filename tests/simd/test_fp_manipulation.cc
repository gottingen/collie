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
struct fp_manipulation_test
{
    using batch_type = B;
    using arch_type = typename B::arch_type;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;
    using array_type = std::array<value_type, size>;
    using int_value_type = collie::simd::as_integer_t<value_type>;
    using int_batch_type = collie::simd::batch<int_value_type, arch_type>;

    array_type input;
    int_value_type exponent;

    fp_manipulation_test()
    {
        exponent = 5;
        for (size_t i = 0; i < size; ++i)
        {
            input[i] = value_type(i) / 4 + value_type(1.2) * std::sqrt(value_type(i + 0.25));
        }
    }

    void test_fp_manipulations() const
    {
        int_batch_type bexp(exponent);
        // ldexp
        {
            array_type expected;
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [this](const value_type& v)
                           { return std::ldexp(v, exponent); });
            batch_type res = collie::simd::ldexp(batch_input(), bexp);
            INFO("ldexp");
            CHECK_BATCH_EQ(res, expected);
        }
        // frexp
        {
            array_type expected;
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { int tmp; return std::frexp(v, &tmp); });
            batch_type res = collie::simd::frexp(batch_input(), bexp);
            INFO("frexp");
            CHECK_BATCH_EQ(res, expected);
        }
    }

private:
    batch_type batch_input() const
    {
        return batch_type::load_unaligned(input.data());
    }
};

TEST_CASE_TEMPLATE("[fp manipulation]", B, BATCH_FLOAT_TYPES)
{
    fp_manipulation_test<B> Test;
    Test.test_fp_manipulations();
}
#endif
