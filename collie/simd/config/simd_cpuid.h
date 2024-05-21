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

#ifndef COLLIE_CONFIG_SIMD_CPUID_H_
#define COLLIE_CONFIG_SIMD_CPUID_H_

#include <collie/base/cpuid.h>
#include <collie/simd/types/all_registers.h>

namespace collie::simd {
    namespace detail {
        struct supported_arch : public collie::Cpuid {
            unsigned best{0};

            inline supported_arch() noexcept : collie::Cpuid(){

#if defined(__aarch64__) || defined(_M_ARM64)
                best = neon64::version();
#elif defined(__ARM_NEON) || defined(_M_ARM)
                best = neon::version() * neon;

#elif defined(__ARM_FEATURE_SVE) && defined(__ARM_FEATURE_SVE_BITS) && __ARM_FEATURE_SVE_BITS > 0
                best = sve::version() * sve;
#elif defined(__riscv_vector) && defined(__riscv_v_fixed_vlen) && __riscv_v_fixed_vlen > 0
                best = ::collie::simd::rvv::version() * rvv;
#elif defined(__x86_64__) || defined(__i386__) || defined(_M_AMD64) || defined(_M_IX86)
                best = std::max(best, sse2::version() * sse2);
                best = std::max(best, sse3::version() * sse3);
                best = std::max(best, ssse3::version() * ssse3);
                best = std::max(best, sse4_1::version() * sse4_1);
                best = std::max(best, sse4_2::version() * sse4_2);
                if (sse4_2)
                    best = std::max(best, fma3<collie::simd::sse4_2>::version() * fma3_sse);
                best = std::max(best, avx::version() * avx);
                best = std::max(best, fma3<collie::simd::avx>::version() * fma3_avx);
                best = std::max(best, fma4::version() * fma4);
                best = std::max(best, avx2::version() * avx2);
                best = std::max(best, avxvnni::version() * avxvnni * avx2);
                best = std::max(best, fma3<collie::simd::avx2>::version() * fma3_avx2);
                best = std::max(best, avx512f::version() * avx512f);
                best = std::max(best, avx512cd::version() * avx512cd * avx512f);
                best = std::max(best, avx512dq::version() * avx512dq * avx512cd * avx512f);
                best = std::max(best, avx512bw::version() * avx512bw * avx512dq * avx512cd * avx512f);
                best = std::max(best, avx512er::version() * avx512er * avx512cd * avx512f);
                best = std::max(best, avx512pf::version() * avx512pf * avx512er * avx512cd * avx512f);
                best = std::max(best, avx512ifma::version() * avx512ifma * avx512bw * avx512dq * avx512cd * avx512f);
                best = std::max(best, avx512vbmi::version() * avx512vbmi * avx512ifma * avx512bw * avx512dq * avx512cd * avx512f);
                best = std::max(best, avx512vnni<collie::simd::avx512bw>::version() * avx512vnni_bw * avx512bw * avx512dq * avx512cd * avx512f);
                best = std::max(best, avx512vnni<collie::simd::avx512vbmi>::version() * avx512vnni_vbmi);
#endif
            }
        };
    }

    inline detail::supported_arch available_architectures() noexcept
    {
        static detail::supported_arch supported;
        return supported;
    }
}

#endif  // COLLIE_CONFIG_SIMD_CPUID_H_
