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

#include <collie/taskflow/cuda/cuda_error.h>

namespace collie::tf {

// ----------------------------------------------------------------------------
// row-wise matrix transpose
// ----------------------------------------------------------------------------
//
template <typename T>
__global__ void cuda_transpose(
  const T* d_in,
  T* d_out,
  size_t rows,
  size_t cols
) {
  __shared__ T tile[32][32];
  size_t x = blockIdx.x * 32 + threadIdx.x;
  size_t y = blockIdx.y * 32 + threadIdx.y;

  for(size_t i = 0; i < 32; i += 8) {
    if(x < cols && (y + i) < rows) {
      tile[threadIdx.y + i][threadIdx.x] = d_in[(y + i) * cols + x];
    }
  }

  __syncthreads();

  x = blockIdx.y * 32 + threadIdx.x;
  y = blockIdx.x * 32 + threadIdx.y;

  for(size_t i = 0; i < 32; i += 8) {
    if(x < rows && (y + i) < cols) {
      d_out[(y + i) * rows + x] = tile[threadIdx.x][threadIdx.y + i];
    }
  }
}

}  // end of namespace --------------------------------------------------------

