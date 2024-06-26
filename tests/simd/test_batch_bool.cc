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

#include <functional>
#include <vector>

#include "test_utils.hpp"

namespace collie::simd
{

    int popcount(int v)
    {
        // from https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
        int c; // c accumulates the total bits set in v
        for (c = 0; v; c++)
        {
            v &= v - 1; // clear the least significant bit set
        }
        return c;
    }

    template <class T, std::size_t N>
    struct get_bool_base
    {
        using vector_type = std::array<bool, N>;

        std::vector<vector_type> almost_all_false()
        {
            std::vector<vector_type> vectors;
            vectors.reserve(N);
            for (size_t i = 0; i < N; ++i)
            {
                vector_type v;
                v.fill(false);
                v[i] = true;
                vectors.push_back(std::move(v));
            }
            return vectors;
        }

        std::vector<vector_type> almost_all_true()
        {
            auto vectors = almost_all_false();
            flip(vectors);
            return vectors;
        }

        void flip(vector_type& vec)
        {
            std::transform(vec.begin(), vec.end(), vec.begin(), std::logical_not<bool> {});
        }

        void flip(std::vector<vector_type>& vectors)
        {
            for (auto& vec : vectors)
            {
                flip(vec);
            }
        }
    };

    template <class T, size_t N = T::size>
    struct get_bool;

    template <class T>
    struct get_bool<batch_bool<T>, 2> : public get_bool_base<T, 2>
    {
        using type = batch_bool<T>;
        type all_true = type(true);
        type all_false = type(false);
        type half = { 0, 1 };
        type ihalf = { 1, 0 };
        type interspersed = { 0, 1 };
    };

    template <class T>
    struct get_bool<batch_bool<T>, 4> : public get_bool_base<T, 4>
    {
        using type = batch_bool<T>;

        type all_true = true;
        type all_false = false;
        type half = { 0, 0, 1, 1 };
        type ihalf = { 1, 1, 0, 0 };
        type interspersed = { 0, 1, 0, 1 };
    };

    template <class T>
    struct get_bool<batch_bool<T>, 8> : public get_bool_base<T, 8>
    {
        using type = batch_bool<T>;
        type all_true = true;
        type all_false = false;
        type half = { 0, 0, 0, 0, 1, 1, 1, 1 };
        type ihalf = { 1, 1, 1, 1, 0, 0, 0, 0 };
        type interspersed = { 0, 1, 0, 1, 0, 1, 0, 1 };
    };

    template <class T>
    struct get_bool<batch_bool<T>, 16> : public get_bool_base<T, 16>
    {
        using type = batch_bool<T>;
        type all_true = true;
        type all_false = false;
        type half = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
        type ihalf = { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
        type interspersed = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
    };

    template <class T>
    struct get_bool<batch_bool<T>, 32> : public get_bool_base<T, 32>
    {
        using type = batch_bool<T>;
        type all_true = true;
        type all_false = false;
        type half = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        type ihalf = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        type interspersed = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
    };

    template <class T>
    struct get_bool<batch_bool<T>, 64> : public get_bool_base<T, 64>
    {
        using type = batch_bool<T>;
        type all_true = true;
        type all_false = false;
        type half = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        type ihalf = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        type interspersed = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
    };

}

template <class B>
struct batch_bool_test
{
    using batch_type = B;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;
    using batch_bool_type = typename B::batch_bool_type;
    using array_type = std::array<value_type, size>;
    using bool_array_type = std::array<bool, size>;

    array_type lhs;
    array_type rhs;
    bool_array_type all_true;
    bool_array_type ba;

    batch_bool_test()
    {
        for (size_t i = 0; i < size; ++i)
        {
            lhs[i] = value_type(i);
            rhs[i] = i == 0 % 2 ? lhs[i] : lhs[i] * value_type(2);
            all_true[i] = true;
            ba[i] = i == 0 % 2 ? true : false;
        }
    }

    template <size_t... Is>
    struct pack
    {
    };

    template <size_t... Values>
    void check_constructor_from_sequence(std::integral_constant<size_t, 0>, pack<Values...>) const
    {
        bool_array_type res = { bool(Values % 3)... };
        bool_array_type tmp;
        batch_bool_type b0(bool(Values % 3)...);
        b0.store_unaligned(tmp.data());
        INFO("batch_bool(values...)");
        CHECK_EQ(tmp, res);

        batch_bool_type b1 { bool(Values % 3)... };
        b1.store_unaligned(tmp.data());
        INFO("batch_bool{values...}");
        CHECK_EQ(tmp, res);
    }

    template <size_t I, size_t... Values>
    void check_constructor_from_sequence(std::integral_constant<size_t, I>, pack<Values...>) const
    {
        return check_constructor_from_sequence(std::integral_constant<size_t, I - 1>(), pack<Values..., I>());
    }

    void test_constructors() const
    {
        batch_bool_type a;
        // value uninitialized, cannot test it.
        (void)a;

        bool_array_type res;
        batch_bool_type b(true);
        b.store_unaligned(res.data());
        INFO("batch_bool{value}");
        CHECK_EQ(res, all_true);

        batch_bool_type c { true };
        c.store_unaligned(res.data());
        INFO("batch_bool{value}");
        CHECK_EQ(res, all_true);

        check_constructor_from_sequence(std::integral_constant<size_t, size>(), pack<>());
    }

    void test_load_store() const
    {
        bool_array_type res;
        batch_bool_type b(batch_bool_type::load_unaligned(ba.data()));
        b.store_unaligned(res.data());
        CHECK_EQ(res, ba);

        alignas(collie::simd::default_arch::alignment()) bool_array_type arhs(this->ba);
        alignas(collie::simd::default_arch::alignment()) bool_array_type ares;
        b = batch_bool_type::load_aligned(arhs.data());
        b.store_aligned(ares.data());
        CHECK_EQ(ares, arhs);
    }

    void test_any_all() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        // any
        {
            auto any_check_false = (batch_lhs() != batch_lhs());
            bool any_res_false = collie::simd::any(any_check_false);
            CHECK_FALSE(any_res_false);
            auto any_check_true = (batch_lhs() == batch_rhs());
            bool any_res_true = collie::simd::any(any_check_true);
            CHECK_UNARY(any_res_true);

            for (const auto& vec : bool_g.almost_all_false())
            {
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool any_res = collie::simd::any(b);
                CHECK_UNARY(any_res);
            }

            for (const auto& vec : bool_g.almost_all_true())
            {
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool any_res = collie::simd::any(b);
                CHECK_UNARY(any_res);
            }
        }
        // all
        {
            auto all_check_false = (batch_lhs() == batch_rhs());
            bool all_res_false = collie::simd::all(all_check_false);
            CHECK_FALSE(all_res_false);
            auto all_check_true = (batch_lhs() == batch_lhs());
            bool all_res_true = collie::simd::all(all_check_true);
            CHECK_UNARY(all_res_true);

            for (const auto& vec : bool_g.almost_all_false())
            {
                // TODO: implement batch_bool(bool*)
                // It currently compiles (need to understand why) but does not
                // give expected result
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool all_res = collie::simd::all(b);
                CHECK_FALSE(all_res);
            }

            for (const auto& vec : bool_g.almost_all_true())
            {
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool all_res = collie::simd::all(b);
                CHECK_FALSE(all_res);
            }
        }
        // none
        {
            auto none_check_false = (batch_lhs() == batch_rhs());
            bool none_res_false = collie::simd::none(none_check_false);
            CHECK_FALSE(none_res_false);
            auto none_check_true = (batch_lhs() != batch_lhs());
            bool none_res_true = collie::simd::none(none_check_true);
            CHECK_UNARY(none_res_true);

            for (const auto& vec : bool_g.almost_all_false())
            {
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool none_res = collie::simd::none(b);
                CHECK_FALSE(none_res);
            }

            for (const auto& vec : bool_g.almost_all_true())
            {
                batch_bool_type b = batch_bool_type::load_unaligned(vec.data());
                bool none_res = collie::simd::none(b);
                CHECK_FALSE(none_res);
            }
        }
    }

    void test_logical_operations() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        size_t s = size;
        // operator!=
        {
            bool res = collie::simd::all(bool_g.half != bool_g.ihalf);
            CHECK_UNARY(res);
        }
        // operator==
        {
            CHECK_BATCH_EQ(bool_g.half, !bool_g.ihalf);
        }
        // operator &&
        {
            batch_bool_type res = bool_g.half && bool_g.ihalf;
            bool_array_type ares;
            res.store_unaligned(ares.data());
            size_t nb_false = std::count(ares.cbegin(), ares.cend(), false);
            CHECK_EQ(nb_false, s);
        }
        // operator ||
        {
            batch_bool_type res = bool_g.half || bool_g.ihalf;
            bool_array_type ares;
            res.store_unaligned(ares.data());
            size_t nb_true = std::count(ares.cbegin(), ares.cend(), true);
            CHECK_EQ(nb_true, s);
        }
        // operator ^
        {
            batch_bool_type res = bool_g.half ^ bool_g.ihalf;
            bool_array_type ares;
            res.store_unaligned(ares.data());
            size_t nb_true = std::count(ares.cbegin(), ares.cend(), true);
            CHECK_EQ(nb_true, s);
        }
        // bitwise_andnot
        {
            batch_bool_type res = collie::simd::bitwise_andnot(bool_g.half, bool_g.half);
            bool_array_type ares;
            res.store_unaligned(ares.data());
            size_t nb_false = std::count(ares.cbegin(), ares.cend(), false);
            CHECK_EQ(nb_false, s);
        }
    }

    void test_bitwise_operations() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        // operator version
        {
            INFO("operator~");
            CHECK_BATCH_EQ(bool_g.half, ~bool_g.ihalf);
        }
        {
            INFO("operator|");
            CHECK_BATCH_EQ(bool_g.half | bool_g.ihalf, bool_g.all_true);
        }
        {
            INFO("operator&");
            CHECK_BATCH_EQ(bool_g.half & bool_g.ihalf, bool_g.all_false);
        }
        // free function version
        {
            INFO("bitwise_not");
            CHECK_BATCH_EQ(bool_g.half, collie::simd::bitwise_not(bool_g.ihalf));
        }
        {
            INFO("bitwise_or");
            CHECK_BATCH_EQ(collie::simd::bitwise_or(bool_g.half, bool_g.ihalf), bool_g.all_true);
        }
        {
            INFO("bitwise_and");
            CHECK_BATCH_EQ(collie::simd::bitwise_and(bool_g.half, bool_g.ihalf), bool_g.all_false);
        }
    }

    void test_update_operations() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        {
            auto tmp = bool_g.half;
            tmp |= bool_g.ihalf;
            bool res = collie::simd::all(tmp);
            INFO("operator|=");
            CHECK_UNARY(res);
        }
        {
            auto tmp = bool_g.half;
            tmp &= bool_g.half;
            INFO("operator&=");
            CHECK_BATCH_EQ(tmp, bool_g.half);
        }
        {
            auto tmp = bool_g.half;
            tmp ^= bool_g.ihalf;
            bool res = collie::simd::all(tmp);
            INFO("operator^=");
            CHECK_UNARY(res);
        }
    }

    void test_mask() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        const uint64_t full_mask = ((uint64_t)-1) >> (64 - batch_bool_type::size);
        CHECK_EQ(bool_g.all_false.mask(), 0);
        CHECK_EQ(batch_bool_type::from_mask(bool_g.all_false.mask()).mask(), bool_g.all_false.mask());

        CHECK_EQ(bool_g.all_true.mask(), full_mask);
        CHECK_EQ(batch_bool_type::from_mask(bool_g.all_true.mask()).mask(), bool_g.all_true.mask());

        CHECK_EQ(bool_g.half.mask(), full_mask & ((uint64_t)-1) << (batch_bool_type::size / 2));
        CHECK_EQ(batch_bool_type::from_mask(bool_g.half.mask()).mask(), bool_g.half.mask());

        CHECK_EQ(bool_g.ihalf.mask(), full_mask & ~(((uint64_t)-1) << (batch_bool_type::size / 2)));
        CHECK_EQ(batch_bool_type::from_mask(bool_g.ihalf.mask()).mask(), bool_g.ihalf.mask());

        CHECK_EQ(bool_g.interspersed.mask(), full_mask & 0xAAAAAAAAAAAAAAAAul);
        CHECK_EQ(batch_bool_type::from_mask(bool_g.interspersed.mask()).mask(), bool_g.interspersed.mask());
    }

    void test_comparison() const
    {
        auto bool_g = collie::simd::get_bool<batch_bool_type> {};
        // eq
        {
            CHECK_BATCH_EQ(bool_g.half, !bool_g.ihalf);
            CHECK_BATCH_EQ(collie::simd::eq(bool_g.half, !bool_g.ihalf), bool_g.all_true);
        }
        // neq
        {
            CHECK_BATCH_EQ(collie::simd::neq(bool_g.half, bool_g.ihalf), bool_g.all_true);
        }
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

TEST_CASE_TEMPLATE("[simd batch bool]", B, BATCH_TYPES)
{
    batch_bool_test<B> Test;

    SUBCASE("constructors") { Test.test_constructors(); }

    SUBCASE("load store") { Test.test_load_store(); }

    SUBCASE("any all") { Test.test_any_all(); }

    SUBCASE("logical operations") { Test.test_logical_operations(); }

    SUBCASE("bitwise operations") { Test.test_bitwise_operations(); }

    SUBCASE("update operations") { Test.test_update_operations(); }

    SUBCASE("mask") { Test.test_mask(); }

    SUBCASE("eq neq") { Test.test_comparison(); }
}
#endif
