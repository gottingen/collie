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

#if !COLLIE_SIMD_WITH_NEON || COLLIE_SIMD_WITH_NEON64
template <class CP>
struct bitwise_cast_test
{
    static constexpr size_t N = CP::size;

    using int32_batch = collie::simd::batch<int32_t>;
    using int64_batch = collie::simd::batch<int64_t>;
    using float_batch = collie::simd::batch<float>;
    using double_batch = collie::simd::batch<double>;

    using int32_vector = std::vector<int32_t, collie::simd::default_allocator<int32_t>>;
    using int64_vector = std::vector<int64_t, collie::simd::default_allocator<int64_t>>;
    using float_vector = std::vector<float, collie::simd::default_allocator<float>>;
    using double_vector = std::vector<double, collie::simd::default_allocator<double>>;

    int32_vector ftoi32_res;
    int32_vector dtoi32_res;
    int64_vector ftoi64_res;
    int64_vector dtoi64_res;
    float_vector i32tof_res;
    float_vector i64tof_res;
    float_vector dtof_res;
    double_vector i32tod_res;
    double_vector i64tod_res;
    double_vector ftod_res;

    bitwise_cast_test()
        : ftoi32_res(2 * N)
        , dtoi32_res(2 * N)
        , ftoi64_res(N)
        , dtoi64_res(N)
        , i32tof_res(2 * N)
        , i64tof_res(2 * N)
        , dtof_res(2 * N)
        , i32tod_res(N)
        , i64tod_res(N)
        , ftod_res(N)
    {
        {
            int32_batch input = i32_input();
            bitcast b;
            b.i32[0] = input.get(0);
            b.i32[1] = input.get(1);
            std::fill(i32tof_res.begin(), i32tof_res.end(), b.f[0]);
            std::fill(i32tod_res.begin(), i32tod_res.end(), b.d);
        }
        {
            int64_batch input = i64_input();
            bitcast b;
            b.i64 = input.get(0);
            std::fill(i64tod_res.begin(), i64tod_res.end(), b.d);
            for (size_t i = 0; i < N; ++i)
            {
                i64tof_res[2 * i] = b.f[0];
                i64tof_res[2 * i + 1] = b.f[1];
            }
        }
        {
            float_batch input = f_input();
            bitcast b;
            b.f[0] = input.get(0);
            b.f[1] = input.get(1);
            std::fill(ftoi32_res.begin(), ftoi32_res.end(), b.i32[0]);
            std::fill(ftoi64_res.begin(), ftoi64_res.end(), b.i64);
            std::fill(ftod_res.begin(), ftod_res.end(), b.d);
        }
        {
            double_batch input = d_input();
            bitcast b;
            b.d = input.get(0);
            // std::fill(dtoi32_res.begin(), dtoi32_res.end(), b.i32[0]);
            std::fill(dtoi64_res.begin(), dtoi64_res.end(), b.i64);
            for (size_t i = 0; i < N; ++i)
            {
                dtoi32_res[2 * i] = b.i32[0];
                dtoi32_res[2 * i + 1] = b.i32[1];
                dtof_res[2 * i] = b.f[0];
                dtof_res[2 * i + 1] = b.f[1];
            }
        }
    }

    void test_to_int32()
    {
        int32_vector i32vres(int32_batch::size);
        {
            int32_batch i32bres = collie::simd::bitwise_cast<int32_t>(f_input());
            i32bres.store_aligned(i32vres.data());
            INFO("to_int32(float)");
            CHECK_VECTOR_EQ(i32vres, ftoi32_res);
        }
        {
            int32_batch i32bres = collie::simd::bitwise_cast<int32_t>(d_input());
            i32bres.store_aligned(i32vres.data());
            INFO("to_int32(double)");
            CHECK_VECTOR_EQ(i32vres, dtoi32_res);
        }
    }

    void test_to_int64()
    {
        int64_vector i64vres(int64_batch::size);
        {
            int64_batch i64bres = collie::simd::bitwise_cast<int64_t>(f_input());
            i64bres.store_aligned(i64vres.data());
            INFO("to_int64(float)");
            CHECK_VECTOR_EQ(i64vres, ftoi64_res);
        }
        {
            int64_batch i64bres = collie::simd::bitwise_cast<int64_t>(d_input());
            i64bres.store_aligned(i64vres.data());
            INFO("to_int64(double)");
            CHECK_VECTOR_EQ(i64vres, dtoi64_res);
        }
    }

    void test_to_float()
    {
        float_vector fvres(float_batch::size);
        {
            float_batch fbres = collie::simd::bitwise_cast<float>(i32_input());
            fbres.store_aligned(fvres.data());
            INFO("to_float(int32_t)");
            CHECK_VECTOR_EQ(fvres, i32tof_res);
        }
        {
            float_batch fbres = collie::simd::bitwise_cast<float>(i64_input());
            fbres.store_aligned(fvres.data());
            INFO("to_float(int64_t)");
            CHECK_VECTOR_EQ(fvres, i64tof_res);
        }
        {
            float_batch fbres = collie::simd::bitwise_cast<float>(d_input());
            fbres.store_aligned(fvres.data());
            INFO("to_float(double)");
            CHECK_VECTOR_EQ(fvres, dtof_res);
        }
    }

    void test_to_double()
    {
        double_vector dvres(double_batch::size);
        {
            double_batch dbres = collie::simd::bitwise_cast<double>(i32_input());
            dbres.store_aligned(dvres.data());
            INFO("to_double(int32_t)");
            CHECK_VECTOR_EQ(dvres, i32tod_res);
        }
        {
            double_batch dbres = collie::simd::bitwise_cast<double>(i64_input());
            dbres.store_aligned(dvres.data());
            INFO("to_double(int64_t)");
            CHECK_VECTOR_EQ(dvres, i64tod_res);
        }
        {
            double_batch dbres = collie::simd::bitwise_cast<double>(f_input());
            dbres.store_aligned(dvres.data());
            INFO("to_double(float)");
            CHECK_VECTOR_EQ(dvres, ftod_res);
        }
    }

private:
    int32_batch i32_input() const
    {
        return int32_batch(2);
    }

    int64_batch i64_input() const
    {
        return int64_batch(2);
    }

    float_batch f_input() const
    {
        return float_batch(3.);
    }

    double_batch d_input() const
    {
        return double_batch(2.5e17);
    }

    union bitcast
    {
        float f[2];
        int32_t i32[2];
        int64_t i64;
        double d;
    };
};

TEST_CASE_TEMPLATE("[bitwise cast]", B, CONVERSION_TYPES)
{
    bitwise_cast_test<B> Test;
    SUBCASE("to_int32") { Test.test_to_int32(); }

    SUBCASE("to_int64") { Test.test_to_int64(); }

    SUBCASE("to_float") { Test.test_to_float(); }

    SUBCASE("to_double") { Test.test_to_double(); }
}
#endif
#endif
