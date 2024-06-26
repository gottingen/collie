#include <collie/taskflow/taskflow.h>
#include <collie/taskflow/cuda/cudaflow.h>

int main() {

  // CUDA version
  std::cout << "========================================\n"
            << "CUDA version: " 
            << collie::tf::cuda_get_runtime_version() << '\n'
            << "CUDA driver version: " 
            << collie::tf::cuda_get_driver_version() << '\n';

  // Number of CUDA devices
  auto num_cuda_devices = collie::tf::cuda_get_num_devices();

  std::cout << "There are " << num_cuda_devices << " CUDA devices.\n";

  // Iterate each device and dump its property
  std::cout << "\nquerying device properties ...\n";
  for(size_t i = 0; i < num_cuda_devices; ++i) {
    std::cout << "CUDA device #" << i << '\n';
    collie::tf::cuda_dump_device_property(std::cout, collie::tf::cuda_get_device_property(i));
  }

  // we can also query each device property attribute by attribute
  std::cout << "\nquerying device attributes ...\n";
  for(size_t i = 0; i< num_cuda_devices; ++i) {
    std::cout << "CUDA device #" << i << '\n';
    std::cout << "Compute capability   : "
              << collie::tf::cuda_get_device_compute_capability_major(i) << '.'
              << collie::tf::cuda_get_device_compute_capability_minor(i) << '\n';
    std::cout << "max threads per block: "
              << collie::tf::cuda_get_device_max_threads_per_block(i) << '\n'
              << "max x-dim   per block: "
              << collie::tf::cuda_get_device_max_x_dim_per_block(i) << '\n'
              << "max y-dim   per block: "
              << collie::tf::cuda_get_device_max_y_dim_per_block(i) << '\n'
              << "max z-dim   per block: "
              << collie::tf::cuda_get_device_max_z_dim_per_block(i) << '\n'
              << "max x-dim   per grid : "
              << collie::tf::cuda_get_device_max_x_dim_per_grid(i) << '\n'
              << "max y-dim   per grid : "
              << collie::tf::cuda_get_device_max_y_dim_per_grid(i) << '\n'
              << "max z-dim   per grid : "
              << collie::tf::cuda_get_device_max_z_dim_per_grid(i) << '\n'
              << "warp size            : "
              << collie::tf::cuda_get_device_warp_size(i) << '\n'
              << "unified addressing?  : "
              << collie::tf::cuda_get_device_unified_addressing(i) << '\n';
  }

  return 0;
}



