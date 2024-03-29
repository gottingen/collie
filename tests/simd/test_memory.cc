/***************************************************************************
 * Copyright (c) Johan Mabille, Sylvain Corlay, Wolf Vollprecht and         *
 * Martin Renou                                                             *
 * Copyright (c) QuantStack                                                 *
 * Copyright (c) Serge Guelton                                              *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

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
