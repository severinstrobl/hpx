# Copyright (c) 2019 The STE||AR-Group
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if(HPX_WITH_CUDA)
  # keywords for target_link_libraries (cuda)
  set(CUDA_LINK_LIBRARIES_KEYWORD "PRIVATE")
  set(CMAKE_CUDA_STANDARD 14)
  find_package(CUDA REQUIRED) # No error message cause it is in FindCUDA.cmake
  set(HPX_WITH_COMPUTE ON)
  hpx_add_config_define(HPX_HAVE_CUDA)
  hpx_add_config_define(HPX_HAVE_COMPUTE)
  if(NOT HPX_WITH_CUDA_CLANG)
    if(NOT MSVC)
      set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-w)
    else()
      set(CUDA_PROPAGATE_HOST_FLAGS OFF)
      set(CUDA_NVCC_FLAGS_DEBUG ${CUDA_NVCC_FLAGS_DEBUG};-D_DEBUG;-O0;-g;-G;-Xcompiler=-MDd;-Xcompiler=-Od;-Xcompiler=-Zi;-Xcompiler=-bigobj)
      set(CUDA_NVCC_FLAGS_RELWITHDEBINFO ${CUDA_NVCC_FLAGS_RELWITHDEBINFO};-DNDEBUG;-O2;-g;-Xcompiler=-MD,-O2,-Zi;-Xcompiler=-bigobj)
      set(CUDA_NVCC_FLAGS_MINSIZEREL ${CUDA_NVCC_FLAGS_MINSIZEREL};-DNDEBUG;-O1;-Xcompiler=-MD,-O1;-Xcompiler=-bigobj)
      set(CUDA_NVCC_FLAGS_RELEASE ${CUDA_NVCC_FLAGS_RELEASE};-DNDEBUG;-O2;-Xcompiler=-MD,-Ox;-Xcompiler=-bigobj)
    endif()
    set(CUDA_SEPARABLE_COMPILATION ON)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};${CXX_FLAG})
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--expt-relaxed-constexpr)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--expt-extended-lambda)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--default-stream per-thread)
  else()
    hpx_add_target_compile_option(-DBOOST_THREAD_USES_MOVE PUBLIC)
  endif()
endif()
