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

#pragma once

#include <collie/simd/types/fma3_sse_register.h>
#include <collie/simd/types/fma4_register.h>
#include <collie/simd/types/sse2_register.h>
#include <collie/simd/types/sse3_register.h>
#include <collie/simd/types/sse4_1_register.h>
#include <collie/simd/types/sse4_2_register.h>

#include <collie/simd/types/avx2_register.h>
#include <collie/simd/types/avx_register.h>
#include <collie/simd/types/avxvnni_register.h>
#include <collie/simd/types/fma3_avx2_register.h>
#include <collie/simd/types/fma3_avx_register.h>

#include <collie/simd/types/avx512vnni_avx512bw_register.h>
#include <collie/simd/types/avx512vnni_avx512vbmi_register.h>

#include <collie/simd/types/avx512ifma_register.h>
#include <collie/simd/types/avx512vbmi_register.h>

#include <collie/simd/types/avx512er_register.h>
#include <collie/simd/types/avx512pf_register.h>

#include <collie/simd/types/avx512bw_register.h>
#include <collie/simd/types/avx512cd_register.h>
#include <collie/simd/types/avx512dq_register.h>
#include <collie/simd/types/avx512f_register.h>

#include <collie/simd/types/neon64_register.h>
#include <collie/simd/types/neon_register.h>

#include <collie/simd/types/sve_register.h>

#include <collie/simd/types/rvv_register.h>

#include <collie/simd/types/wasm_register.h>
