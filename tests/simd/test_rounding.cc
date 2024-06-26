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
struct rounding_test
{
    using batch_type = B;
    using arch_type = typename B::arch_type;
    using value_type = typename B::value_type;
    using int_value_type = collie::simd::as_integer_t<value_type>;
    using int_batch_type = collie::simd::batch<int_value_type, arch_type>;
    static constexpr size_t size = B::size;
    static constexpr size_t nb_input = 8;
    static constexpr size_t nb_batches = nb_input / size;

    std::array<value_type, nb_input> input;
    std::array<value_type, nb_input> expected;
    std::array<value_type, nb_input> res;

    rounding_test()
    {
        input[0] = value_type(-3.5);
        input[1] = value_type(-2.7);
        input[2] = value_type(-2.5);
        input[3] = value_type(-2.3);
        input[4] = value_type(2.3);
        input[5] = value_type(2.5);
        input[6] = value_type(2.7);
        input[7] = value_type(3.5);
    }

    void test_rounding_functions()
    {
        // ceil
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::ceil(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = ceil(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::ceil(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("ceil");
            CHECK_EQ(diff, 0);
        }
        // floor
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::floor(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = floor(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::floor(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("floor");
            CHECK_EQ(diff, 0);
        }
        // trunc
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::trunc(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = trunc(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::trunc(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("trunc");
            CHECK_EQ(diff, 0);
        }
        // round
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::round(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = round(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::round(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("round");
            CHECK_EQ(diff, 0);
        }
        // nearbyint
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::nearbyint(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = nearbyint(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::nearbyint(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("nearbyint");
            CHECK_EQ(diff, 0);
        }
        // nearbyint_as_int
        {
            std::array<int_value_type, nb_input> expected;
            std::array<int_value_type, nb_input> res;
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return collie::simd::nearbyint_as_int(v); });
            batch_type in;
            int_batch_type out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = nearbyint_as_int(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = collie::simd::nearbyint_as_int(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("nearbyint_as_int");
            CHECK_EQ(diff, 0);
        }
        // rint
        {
            std::transform(input.cbegin(), input.cend(), expected.begin(),
                           [](const value_type& v)
                           { return std::rint(v); });
            batch_type in, out;
            for (size_t i = 0; i < nb_batches; i += size)
            {
                detail::load_batch(in, input, i);
                out = rint(in);
                detail::store_batch(out, res, i);
            }
            for (size_t i = nb_batches; i < nb_input; ++i)
            {
                res[i] = std::rint(input[i]);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("rint");
            CHECK_EQ(diff, 0);
        }
    }
};

TEST_CASE_TEMPLATE("[rounding]", B, BATCH_FLOAT_TYPES)
{

    rounding_test<B> Test;
    Test.test_rounding_functions();
}
#endif
