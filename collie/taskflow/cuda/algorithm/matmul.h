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

#include <collie/taskflow/cuda/cudaflow.h>

namespace collie::tf {

// ----------------------------------------------------------------------------
// row-major matrix multiplication
// ----------------------------------------------------------------------------

template <typename T>
__global__ void cuda_matmul(
  const T* A,
  const T* B,
  T* C,
  size_t M,
  size_t K,
  size_t N
) {
  __shared__ T A_tile[32][32];
  __shared__ T B_tile[32][32];

  size_t x = blockIdx.x * blockDim.x + threadIdx.x;
  size_t y = blockIdx.y * blockDim.y + threadIdx.y;

  T res = 0;

  for(size_t k = 0; k < K; k += 32) {
    if((threadIdx.x + k) < K && y < M) {
      A_tile[threadIdx.y][threadIdx.x] = A[y * K + threadIdx.x + k];
    }
    else{
      A_tile[threadIdx.y][threadIdx.x] = 0;
    }

    if((threadIdx.y + k) < K && x < N) {
      B_tile[threadIdx.y][threadIdx.x] = B[(threadIdx.y + k) * N + x];
    }
    else{
      B_tile[threadIdx.y][threadIdx.x] = 0;
    }

    __syncthreads();

    for(size_t i = 0; i < 32; ++i) {
      res += A_tile[threadIdx.y][i] * B_tile[i][threadIdx.x];
    }
    __syncthreads();
  }

  if(x < N && y < M) {
    C[y * N + x] = res;
  }

}

} // end of namespace collie::tf ---------------------------------------------------------
