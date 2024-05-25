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

#include <type_traits>
#include <vector>

#include <collie/testing/doctest.h>

#include <collie/simd/memory/allocator.h>

struct mock_container
{
};

TEST_CASE("[alignment]")
{
    using u_vector_type = std::vector<double>;
    using a_vector_type = std::vector<double, collie::simd::default_allocator<double>>;

    using u_vector_align = collie::simd::container_alignment_t<u_vector_type>;
    using a_vector_align = collie::simd::container_alignment_t<a_vector_type>;
    using mock_align = collie::simd::container_alignment_t<mock_container>;

    CHECK_UNARY((std::is_same<u_vector_align, collie::simd::unaligned_mode>::value));
    CHECK_UNARY((std::is_same<a_vector_align, collie::simd::aligned_mode>::value));
    CHECK_UNARY((std::is_same<mock_align, collie::simd::unaligned_mode>::value));
}

TEST_CASE("[is_aligned]")
{
    float f[100];
    void* unaligned_f = static_cast<void*>(&f[0]);
    constexpr std::size_t alignment = collie::simd::default_arch::alignment();
    std::size_t aligned_f_size;
    void* aligned_f = std::align(alignment, sizeof(f), unaligned_f, aligned_f_size);
    CHECK_UNARY(collie::simd::is_aligned(aligned_f));

    // GCC does not generate correct alignment on ARM
    // (see https://godbolt.org/z/obv1n8bWq)
#if !(COLLIE_SIMD_WITH_NEON && defined(__GNUC__) && !defined(__clang__))
    alignas(alignment) char aligned[8];
    CHECK_UNARY(collie::simd::is_aligned(&aligned[0]));
    CHECK_UNARY(!collie::simd::is_aligned(&aligned[3]));
#endif
}
#endif
