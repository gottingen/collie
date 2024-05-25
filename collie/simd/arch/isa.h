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

#ifndef COLLIE_SIMD_ARCH_ISA_H_
#define COLLIE_SIMD_ARCH_ISA_H_

#include <collie/simd/config/simd_arch.h>

#include <collie/simd/arch/generic_fwd.h>

#if COLLIE_SIMD_WITH_SSE2
#include <collie/simd/arch/sse2.h>
#endif

#if COLLIE_SIMD_WITH_SSE3
#include <collie/simd/arch/sse3.h>
#endif

#if COLLIE_SIMD_WITH_SSSE3
#include <collie/simd/arch/ssse3.h>
#endif

#if COLLIE_SIMD_WITH_SSE4_1
#include <collie/simd/arch/sse4_1.h>
#endif

#if COLLIE_SIMD_WITH_SSE4_2
#include <collie/simd/arch/sse4_2.h>
#endif

#if COLLIE_SIMD_WITH_FMA3_SSE
#include <collie/simd/arch/fma3_sse.h>
#endif

#if COLLIE_SIMD_WITH_FMA4
#include <collie/simd/arch/fma4.h>
#endif

#if COLLIE_SIMD_WITH_AVX
#include <collie/simd/arch/avx.h>
#endif

#if COLLIE_SIMD_WITH_FMA3_AVX
#include <collie/simd/arch/fma3_avx.h>
#endif

#if COLLIE_SIMD_WITH_AVXVNNI
#include <collie/simd/arch/avxvnni.h>
#endif

#if COLLIE_SIMD_WITH_AVX2
#include <collie/simd/arch/avx2.h>
#endif

#if COLLIE_SIMD_WITH_FMA3_AVX2
#include <collie/simd/arch/fma3_avx2.h>
#endif

#if COLLIE_SIMD_AVX512F
#include <collie/simd/arch/avx512f.h>
#endif

#if COLLIE_SIMD_AVX512BW
#include <collie/simd/arch/avx512bw.h>
#endif

#if COLLIE_SIMD_AVX512ER
#include <collie/simd/arch/avx512er.h>
#endif

#if COLLIE_SIMD_AVX512PF
#include <collie/simd/arch/avx512pf.h>
#endif

#if COLLIE_SIMD_AVX512IFMA
#include <collie/simd/arch/avx512ifma.h>
#endif

#if COLLIE_SIMD_AVX512VBMI
#include <collie/simd/arch/avx512vbmi.h>
#endif

#if COLLIE_SIMD_WITH_AVX512VNNI_AVX512BW
#include <collie/simd/arch/avx512vnni_avx512bw.h>
#endif

#if COLLIE_SIMD_WITH_AVX512VNNI_AVX512VBMI
#include <collie/simd/arch/avx512vnni_avx512vbmi.h>
#endif

#if COLLIE_SIMD_WITH_NEON
#include <collie/simd/arch/neon.h>
#endif

#if COLLIE_SIMD_WITH_NEON64
#include <collie/simd/arch/neon64.h>
#endif

#if COLLIE_SIMD_WITH_SVE
#include <collie/simd/arch/sve.h>
#endif

#if COLLIE_SIMD_WITH_RVV
#include <collie/simd/arch/rvv.h>
#endif

#if COLLIE_SIMD_WASM
#include <collie/simd/arch/wasm.h>
#endif

// Must come last to have access to all conversion specializations.
#include <collie/simd/arch/generic.h>

#endif  // COLLIE_SIMD_ARCH_ISA_H_
