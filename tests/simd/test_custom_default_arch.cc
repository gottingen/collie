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
#ifdef __STSE2__

#define COLLIE_SIMD_DEFAULT_ARCH collie::simd::sse2
#include <collie/simd/simd.h>

#include "test_utils.hpp"

// Could be different than sse2 if we compile for other architecture avx
static_assert(std::is_same<collie::simd::default_arch, collie::simd::sse2>::value, "default arch correctly hooked");

#else

#define COLLIE_SIMD_DEFAULT_ARCH collie::simd::unsupported
#include <collie/simd/simd.h>

#endif
