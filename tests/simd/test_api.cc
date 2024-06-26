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

#include <random>

#include "test_utils.hpp"

template <class B>
struct simd_api_test
{
    using batch_type = B;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;
    using array_type = std::array<value_type, size>;
    using int8_vector_type = std::vector<int8_t, collie::simd::default_allocator<int8_t>>;
    using uint8_vector_type = std::vector<uint8_t, collie::simd::default_allocator<uint8_t>>;
    using int16_vector_type = std::vector<int16_t, collie::simd::default_allocator<int16_t>>;
    using uint16_vector_type = std::vector<uint16_t, collie::simd::default_allocator<uint16_t>>;
    using int32_vector_type = std::vector<int32_t, collie::simd::default_allocator<int32_t>>;
    using uint32_vector_type = std::vector<uint32_t, collie::simd::default_allocator<uint32_t>>;
    using int64_vector_type = std::vector<int64_t, collie::simd::default_allocator<int64_t>>;
    using uint64_vector_type = std::vector<uint64_t, collie::simd::default_allocator<uint64_t>>;
    using float_vector_type = std::vector<float, collie::simd::default_allocator<float>>;
    using double_vector_type = std::vector<double, collie::simd::default_allocator<double>>;

    int8_vector_type i8_vec;
    uint8_vector_type ui8_vec;
    int16_vector_type i16_vec;
    uint16_vector_type ui16_vec;
    int32_vector_type i32_vec;
    uint32_vector_type ui32_vec;
    int64_vector_type i64_vec;
    uint64_vector_type ui64_vec;
    float_vector_type f_vec;
    double_vector_type d_vec;

    array_type expected;

    simd_api_test()
    {
        init_test_vector(i8_vec);
        init_test_vector(ui8_vec);
        init_test_vector(i16_vec);
        init_test_vector(ui16_vec);
        init_test_vector(i32_vec);
        init_test_vector(ui32_vec);
        init_test_vector(i64_vec);
        init_test_vector(ui64_vec);
        init_test_vector(f_vec);
#if COLLIE_SIMD_WITH_NEON64 || !COLLIE_SIMD_WITH_NEON
        init_test_vector(d_vec);
#endif
    }

    void test_load()
    {
        test_load_impl(i8_vec, "load int8_t");
        test_load_impl(ui8_vec, "load uint8_t");
        test_load_impl(i16_vec, "load int16_t");
        test_load_impl(ui16_vec, "load uint16_t");
        test_load_impl(i32_vec, "load int32_t");
        test_load_impl(ui32_vec, "load uint32_t");
        test_load_impl(i64_vec, "load int64_t");
        test_load_impl(ui64_vec, "load uint64_t");
        test_load_impl(f_vec, "load float");
#if COLLIE_SIMD_WITH_NEON64 || !COLLIE_SIMD_WITH_NEON
        test_load_impl(d_vec, "load double");
#endif
    }

    void test_store()
    {
        test_store_impl(i8_vec, "load int8_t");
        test_store_impl(ui8_vec, "load uint8_t");
        test_store_impl(i16_vec, "load int16_t");
        test_store_impl(ui16_vec, "load uint16_t");
        test_store_impl(i32_vec, "load int32_t");
        test_store_impl(ui32_vec, "load uint32_t");
        test_store_impl(i64_vec, "load int64_t");
        test_store_impl(ui64_vec, "load uint64_t");
        test_store_impl(f_vec, "load float");
#if COLLIE_SIMD_WITH_NEON64 || !COLLIE_SIMD_WITH_NEON
        test_store_impl(d_vec, "load double");
#endif
    }

    void test_set()
    {
        test_set_impl<int8_t>("set int8_t");
        test_set_impl<uint8_t>("set uint8_t");
        test_set_impl<int16_t>("set int16_t");
        test_set_impl<uint16_t>("set uint16_t");
        test_set_impl<int32_t>("set int32_t");
        test_set_impl<uint32_t>("set uint32_t");
        test_set_impl<int64_t>("set int64_t");
        test_set_impl<uint64_t>("set uint64_t");
        test_set_impl<float>("set float");
#if COLLIE_SIMD_WITH_NEON64 || !COLLIE_SIMD_WITH_NEON
        test_set_impl<double>("set double");
#endif
    }

private:
    template <class V>
    void test_load_impl(const V& v, const std::string& name)
    {
        batch_type b;
        std::copy(v.cbegin(), v.cend(), expected.begin());

        b = batch_type::load(v.data(), collie::simd::unaligned_mode());
        INFO(name, " unaligned");
        CHECK_BATCH_EQ(b, expected);

        b = batch_type::load(v.data(), collie::simd::aligned_mode());
        INFO(name, " aligned");
        CHECK_BATCH_EQ(b, expected);
    }

    template <class V>
    void test_store_impl(const V& v, const std::string& name)
    {
        batch_type b = batch_type::load(v.data(), collie::simd::aligned_mode());
        V res(size);

        collie::simd::store_as(res.data(), b, collie::simd::unaligned_mode());
        INFO(name, " unaligned");
        CHECK_VECTOR_EQ(res, v);

        collie::simd::store_as(res.data(), b, collie::simd::aligned_mode());
        INFO(name, " aligned");
        CHECK_VECTOR_EQ(res, v);
    }

    template <class T>
    void test_set_impl(const std::string& name)
    {
        T v = T(1);
        batch_type expected(v);
        batch_type res = collie::simd::broadcast<value_type>(v);
        INFO(name);
        CHECK_BATCH_EQ(res, expected);
    }

    template <class V>
    void init_test_vector(V& vec)
    {
        vec.resize(size);

        value_type min = value_type(0);
        value_type max = value_type(100);

        std::default_random_engine generator;
        std::uniform_int_distribution<int> distribution(min, max);

        auto gen = [&distribution, &generator]()
        {
            return static_cast<value_type>(distribution(generator));
        };

        std::generate(vec.begin(), vec.end(), gen);
    }
};

TEST_CASE_TEMPLATE("[basic api]", B, BATCH_TYPES)
{
    simd_api_test<B> Test;
    SUBCASE("load")
    {
        Test.test_load();
    }

    SUBCASE("store")
    {
        Test.test_store();
    }

    SUBCASE("set")
    {
        Test.test_set();
    }
}
#endif
