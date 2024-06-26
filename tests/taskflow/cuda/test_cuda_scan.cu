#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <collie/testing/doctest.h>
#include <collie/taskflow/taskflow.h>
#include <collie/taskflow/cuda/cudaflow.h>
#include <collie/taskflow/cuda/algorithm/scan.hpp>

// ----------------------------------------------------------------------------
// cuda_scan
// ----------------------------------------------------------------------------

template <typename T>
void cuda_scan() {

  collie::tf::Taskflow taskflow;
  collie::tf::Executor executor;
  
  for(int n=0; n<=1234567; n = (n<=100) ? n+1 : n*2 + 1) {

    taskflow.emplace([n](){
  
      auto data1 = collie::tf::cuda_malloc_shared<int>(n);
      auto data2 = collie::tf::cuda_malloc_shared<int>(n);
      auto scan1 = collie::tf::cuda_malloc_shared<int>(n);
      auto scan2 = collie::tf::cuda_malloc_shared<int>(n);

      // --------------------------------------------------------------------------
      // inclusive/exclusive scan
      // --------------------------------------------------------------------------

      // initialize the data
      std::iota(data1, data1 + n, 0);
      std::iota(data2, data2 + n, 0);
      
      collie::tf::cudaStream stream;
      collie::tf::cudaDefaultExecutionPolicy policy(stream);

      // declare the buffer
      void* buff;
      cudaMalloc(&buff, policy.scan_bufsz<int>(n));
      
      // create inclusive and exclusive scan tasks
      collie::tf::cuda_inclusive_scan(policy, data1, data1+n, scan1, collie::tf::cuda_plus<int>{}, buff);
      collie::tf::cuda_exclusive_scan(policy, data2, data2+n, scan2, collie::tf::cuda_plus<int>{}, buff);

      stream.synchronize();
      
      // inspect 
      for(int i=1; i<n; i++) {
        REQUIRE(scan1[i] == (scan1[i-1] + data1[i]));
        REQUIRE(scan2[i] == (scan2[i-1] + data2[i-1]));
      }
  
      // deallocate the data
      REQUIRE(cudaFree(data1) == cudaSuccess);
      REQUIRE(cudaFree(data2) == cudaSuccess);
      REQUIRE(cudaFree(scan1) == cudaSuccess);
      REQUIRE(cudaFree(scan2) == cudaSuccess);
      REQUIRE(cudaFree(buff)  == cudaSuccess);
    });
  }

  executor.run(taskflow).wait();
}

TEST_CASE("cuda_scan.int" * doctest::timeout(300)) {
  cuda_scan<int>();
}

// ----------------------------------------------------------------------------
// transform_scan
// ----------------------------------------------------------------------------

template <typename T>
void cuda_transform_scan() {

  collie::tf::Taskflow taskflow;
  collie::tf::Executor executor;
  
  for(int n=0; n<=1234567; n = (n<=100) ? n+1 : n*2 + 1) {
  
    taskflow.emplace([n](){

      auto data1 = collie::tf::cuda_malloc_shared<int>(n);
      auto data2 = collie::tf::cuda_malloc_shared<int>(n);
      auto scan1 = collie::tf::cuda_malloc_shared<int>(n);
      auto scan2 = collie::tf::cuda_malloc_shared<int>(n);

      // --------------------------------------------------------------------------
      // inclusive/exclusive scan
      // --------------------------------------------------------------------------

      collie::tf::cudaStream stream;
      collie::tf::cudaDefaultExecutionPolicy policy(stream);

      // declare the buffer
      void* buff;
      cudaMalloc(&buff, policy.scan_bufsz<int>(n));
      
      // initialize the data
      std::iota(data1, data1 + n, 0);
      std::iota(data2, data2 + n, 0);
      
      // transform inclusive scan
      collie::tf::cuda_transform_inclusive_scan(policy,
        data1, data1+n, scan1, collie::tf::cuda_plus<int>{},
        [] __device__ (int a) { return a*10; },
        buff
      );

      // transform exclusive scan
      collie::tf::cuda_transform_exclusive_scan(policy,
        data2, data2+n, scan2, collie::tf::cuda_plus<int>{},
        [] __device__ (int a) { return a*11; },
        buff
      );
      
      stream.synchronize();
  
      // inspect 
      for(int i=1; i<n; i++) {
        REQUIRE(scan1[i] == scan1[i-1] + data1[i] * 10);
        REQUIRE(scan2[i] == scan2[i-1] + data2[i-1] * 11);
      }
  
      // deallocate the data
      REQUIRE(cudaFree(data1) == cudaSuccess);
      REQUIRE(cudaFree(data2) == cudaSuccess);
      REQUIRE(cudaFree(scan1) == cudaSuccess);
      REQUIRE(cudaFree(scan2) == cudaSuccess);
      REQUIRE(cudaFree(buff)  == cudaSuccess);
    });
  }

  executor.run(taskflow).wait();
}

TEST_CASE("cuda_transform_scan.int" * doctest::timeout(300)) {
  cuda_transform_scan<int>();
}
