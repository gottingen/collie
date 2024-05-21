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
struct power_test
{
    using batch_type = B;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;
    using vector_type = std::vector<value_type>;

    size_t nb_input;
    vector_type zlhs_input;
    vector_type lhs_input;
    vector_type rhs_input;
    vector_type expected;
    vector_type res;

    power_test()
    {
        nb_input = size * 10000;
        zlhs_input.resize(nb_input);
        lhs_input.resize(nb_input);
        rhs_input.resize(nb_input);
        for (size_t i = 0; i < nb_input; ++i)
        {
            lhs_input[i] = value_type(i) / 4 + value_type(1.2) * std::sqrt(value_type(i + 0.25));
            zlhs_input[i] = lhs_input[i] * (i % 2);
            rhs_input[i] = value_type(10.2) / (i + 2) + value_type(0.25);
        }

        expected.resize(nb_input);
        res.resize(nb_input);
    }

    void test_power_functions()
    {
        // pow
        {
            std::transform(lhs_input.cbegin(), lhs_input.cend(), rhs_input.cbegin(), expected.begin(),
                           [](const value_type& l, const value_type& r)
                           { return std::pow(l, r); });
            batch_type lhs_in, rhs_in, out;
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(lhs_in, lhs_input, i);
                detail::load_batch(rhs_in, rhs_input, i);
                out = pow(lhs_in, rhs_in);
                detail::store_batch(out, res, i);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("pow");
            CHECK_EQ(diff, 0);
        }
        // pow zero
        {
            std::transform(zlhs_input.cbegin(), zlhs_input.cend(), rhs_input.cbegin(), expected.begin(),
                           [](const value_type& l, const value_type& r)
                           { return std::pow(l, r); });
            batch_type zlhs_in, rhs_in, out;
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(zlhs_in, zlhs_input, i);
                detail::load_batch(rhs_in, rhs_input, i);
                out = pow(zlhs_in, rhs_in);
                detail::store_batch(out, res, i);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("pow");
            CHECK_EQ(diff, 0);

// use of undeclared identifier '_MM_SET_EXCEPTION_MASK for emscripten
#if defined(__SSE__) && !defined(EMSCRIPTEN)
            // Test with FE_INVALID...
            unsigned mask = _MM_GET_EXCEPTION_MASK();
            _MM_SET_EXCEPTION_MASK(mask & ~_MM_MASK_INVALID);
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(zlhs_in, zlhs_input, i);
                detail::load_batch(rhs_in, rhs_input, i);
                out = pow(zlhs_in, rhs_in);
                detail::store_batch(out, res, i);
            }
            _MM_SET_EXCEPTION_MASK(mask);
            diff = detail::get_nb_diff(res, expected);
            INFO("pow");
            CHECK_EQ(diff, 0);
#endif
        }
        // ipow
        {
            long k = 0;
            std::transform(lhs_input.cbegin(), lhs_input.cend(), expected.begin(),
                           [&k, this](const value_type& l)
                           { auto arg = k / size - nb_input / size / 2; ++k; return std::pow(l, arg); });
            batch_type lhs_in, out;
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(lhs_in, lhs_input, i);
                out = pow(lhs_in, i / size - nb_input / size / 2);
                detail::store_batch(out, res, i);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("ipow");
            CHECK_EQ(diff, 0);
        }
        // hypot
        {
            std::transform(lhs_input.cbegin(), lhs_input.cend(), rhs_input.cbegin(), expected.begin(),
                           [](const value_type& l, const value_type& r)
                           { return std::hypot(l, r); });
            batch_type lhs_in, rhs_in, out;
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(lhs_in, lhs_input, i);
                detail::load_batch(rhs_in, rhs_input, i);
                out = hypot(lhs_in, rhs_in);
                detail::store_batch(out, res, i);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("hypot");
            CHECK_EQ(diff, 0);
        }
        // cbrt
        {
            std::transform(lhs_input.cbegin(), lhs_input.cend(), expected.begin(),
                           [](const value_type& l)
                           { return std::cbrt(l); });
            batch_type lhs_in, out;
            for (size_t i = 0; i < nb_input; i += size)
            {
                detail::load_batch(lhs_in, lhs_input, i);
                out = cbrt(lhs_in);
                detail::store_batch(out, res, i);
            }
            size_t diff = detail::get_nb_diff(res, expected);
            INFO("cbrt");
            CHECK_EQ(diff, 0);
        }
    }
};

TEST_CASE_TEMPLATE("[power]", B, BATCH_FLOAT_TYPES)
{
    power_test<B> Test;
    Test.test_power_functions();
}
#endif
