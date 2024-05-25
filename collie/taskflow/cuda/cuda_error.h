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

#include <cuda.h>
#include <iostream>
#include <sstream>
#include <exception>

#include <collie/taskflow//utility/stream.h>

#define TF_CUDA_EXPAND( x ) x
#define TF_CUDA_REMOVE_FIRST_HELPER(N, ...) __VA_ARGS__
#define TF_CUDA_REMOVE_FIRST(...) TF_CUDA_EXPAND(TF_CUDA_REMOVE_FIRST_HELPER(__VA_ARGS__))
#define TF_CUDA_GET_FIRST_HELPER(N, ...) N
#define TF_CUDA_GET_FIRST(...) TF_CUDA_EXPAND(TF_CUDA_GET_FIRST_HELPER(__VA_ARGS__))

#define TF_CHECK_CUDA(...)                                       \
if(TF_CUDA_GET_FIRST(__VA_ARGS__) != cudaSuccess) {              \
  std::ostringstream oss;                                        \
  auto __ev__ = TF_CUDA_GET_FIRST(__VA_ARGS__);                  \
  oss << "[" << __FILE__ << ":" << __LINE__ << "] "              \
      << (cudaGetErrorString(__ev__)) << " ("                    \
      << (cudaGetErrorName(__ev__)) << ") - ";                   \
  collie::tf::ostreamize(oss, TF_CUDA_REMOVE_FIRST(__VA_ARGS__));        \
  throw std::runtime_error(oss.str());                           \
}

