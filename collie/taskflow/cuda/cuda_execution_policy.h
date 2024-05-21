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

/**
@file cuda_execution_policy.hpp
@brief CUDA execution policy include file
*/

namespace collie::tf {

/**
@class cudaExecutionPolicy

@brief class to define execution policy for CUDA standard algorithms

@tparam NT number of threads per block
@tparam VT number of work units per thread

Execution policy configures the kernel execution parameters in CUDA algorithms.
The first template argument, @c NT, the number of threads per block should
always be a power-of-two number.
The second template argument, @c VT, the number of work units per thread
is recommended to be an odd number to avoid bank conflict.

Details can be referred to @ref CUDASTDExecutionPolicy.
*/
template<unsigned NT, unsigned VT>
class cudaExecutionPolicy {

  static_assert(is_pow2(NT), "max # threads per block must be a power of two");

  public:

  /** @brief static constant for getting the number of threads per block */
  const static unsigned nt = NT;

  /** @brief static constant for getting the number of work units per thread */
  const static unsigned vt = VT;

  /** @brief static constant for getting the number of elements to process per block */
  const static unsigned nv = NT*VT;

  /**
  @brief constructs an execution policy object with default stream
   */
  cudaExecutionPolicy() = default;

  /**
  @brief constructs an execution policy object with the given stream
   */
  explicit cudaExecutionPolicy(cudaStream_t s) : _stream{s} {}
  
  /**
  @brief queries the associated stream
   */
  cudaStream_t stream() noexcept { return _stream; };

  /**
  @brief assigns a stream
   */
  void stream(cudaStream_t stream) noexcept { _stream = stream; }
  
  /**
  @brief queries the number of blocks to accommodate N elements
  */
  static unsigned num_blocks(unsigned N) { return (N + nv - 1) / nv; } 
  
  // --------------------------------------------------------------------------
  // Buffer Sizes for Standard Algorithms
  // --------------------------------------------------------------------------
  
  /**
  @brief queries the buffer size in bytes needed to call reduce kernels
  
  @tparam T value type
  
  @param count number of elements to reduce
  
  The function is used to allocate a buffer for calling collie::tf::cuda_reduce,
  collie::tf::cuda_uninitialized_reduce, collie::tf::cuda_transform_reduce, and
  collie::tf::cuda_uninitialized_transform_reduce.
  */
  template <typename T>
  static unsigned reduce_bufsz(unsigned count);

  /**
  @brief queries the buffer size in bytes needed to call collie::tf::cuda_min_element
  
  @tparam T value type
  
  @param count number of elements to search
  
  The function is used to decide the buffer size in bytes for calling
  collie::tf::cuda_min_element.
  */
  template <typename T>
  static unsigned min_element_bufsz(unsigned count);

  /**
  @brief queries the buffer size in bytes needed to call collie::tf::cuda_max_element
  
  @tparam T value type
  
  @param count number of elements to search
  
  The function is used to decide the buffer size in bytes for calling
  collie::tf::cuda_max_element.
  */
  template <typename T>
  static unsigned max_element_bufsz(unsigned count);

  /**
  @brief queries the buffer size in bytes needed to call scan kernels
  
  @tparam T value type
  
  @param count number of elements to scan
  
  The function is used to allocate a buffer for calling
  collie::tf::cuda_inclusive_scan, collie::tf::cuda_exclusive_scan,
  collie::tf::cuda_transform_inclusive_scan, and collie::tf::cuda_transform_exclusive_scan.
  */
  template <typename T>
  static unsigned scan_bufsz(unsigned count);
  
  /**
  @brief queries the buffer size in bytes needed for CUDA merge algorithms

  @param a_count number of elements in the first vector to merge
  @param b_count number of elements in the second vector to merge

  The buffer size of merge algorithm does not depend on the data type.
  The buffer is purely used only for storing temporary indices 
  (of type @c unsigned) required during the merge process.

  The function is used to allocate a buffer for calling
  collie::tf::cuda_merge and collie::tf::cuda_merge_by_key.
  */
  inline static unsigned merge_bufsz(unsigned a_count, unsigned b_count);

  private:

  cudaStream_t _stream {0};
};

/**
@brief default execution policy
 */
using cudaDefaultExecutionPolicy = cudaExecutionPolicy<512, 7>;

}  // end of namespace collie::tf -----------------------------------------------------



