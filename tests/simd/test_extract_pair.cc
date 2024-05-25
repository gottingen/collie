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

namespace collie::simd
{
    template <typename T, std::size_t N>
    struct init_extract_pair_base
    {
        using extract_vector_type = std::array<T, N>;
        extract_vector_type lhs_in, rhs_in, exped;

        std::vector<extract_vector_type> create_extract_vectors(const int index)
        {
            std::vector<extract_vector_type> vects;
            vects.reserve(3);

            int num = static_cast<int>(N);
            /* Generate input data: lhs, rhs */
            for (int i = 0; i < num; ++i)
            {
                lhs_in[i] = 2 * i + 1;
                rhs_in[i] = 2 * i + 2;
            }
            vects.push_back(std::move(lhs_in));
            vects.push_back(std::move(rhs_in));

            /* Expected shuffle data */
            for (int i = 0; i < (num - index); ++i)
            {
                exped[i] = rhs_in[i + index];
                if (i < index)
                {
                    exped[num - 1 - i] = lhs_in[index - 1 - i];
                }
            }
            vects.push_back(std::move(exped));

            return vects;
        }
    };
}

template <class B>
struct extract_pair_test
{
    using batch_type = B;
    using value_type = typename B::value_type;
    static constexpr size_t size = B::size;

    void extract_pair_128()
    {
        collie::simd::init_extract_pair_base<value_type, size> extract_pair_base;
        auto extract_pair_vecs = extract_pair_base.create_extract_vectors(1);
        auto v_lhs = extract_pair_vecs[0];
        auto v_rhs = extract_pair_vecs[1];
        auto v_exped = extract_pair_vecs[2];

        B b_lhs = B::load_unaligned(v_lhs.data());
        B b_rhs = B::load_unaligned(v_rhs.data());
        B b_exped = B::load_unaligned(v_exped.data());

        /* Only Test 128bit */
        if ((sizeof(value_type) * size) == 16)
        {
            B b_res = collie::simd::extract_pair(b_lhs, b_rhs, 1);
            CHECK_BATCH_EQ(b_res, b_exped);
        }
    }
};

TEST_CASE_TEMPLATE("[extract pair]", B, BATCH_TYPES)
{
    extract_pair_test<B> Test;
    SUBCASE("extract_pair_128")
    {
        Test.extract_pair_128();
    }
}
#endif
