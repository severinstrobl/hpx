# Copyright (c) 2019-2020 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(init_runtime_headers
    hpx/init.hpp
    hpx/hpx_finalize.hpp
    hpx/hpx_init.hpp
    hpx/hpx_init_impl.hpp
    hpx/hpx_init_params.hpp
    hpx/hpx_main_winsocket.hpp
    hpx/hpx_start.hpp
    hpx/hpx_start_impl.hpp
    hpx/hpx_suspend.hpp
    hpx/hpx_user_main_config.hpp
    hpx/init_runtime/detail/run_or_start.hpp
    hpx/init_runtime/detail/init_logging.hpp
)

set(init_runtime_sources hpx_init.cpp hpx_main_winsocket.cpp)

if(HPX_WITH_DISTRIBUTED_RUNTIME)
  set(init_runtime_headers ${init_runtime_headers}
                           hpx/init_runtime/pre_main.hpp
  )

  set(init_runtime_sources ${init_runtime_sources} pre_main.cpp)

  set(init_runtime_optional_module_dependencies
      hpx_async_distributed hpx_naming hpx_performance_counters
      hpx_runtime_distributed
  )
endif()

if((HPX_WITH_NETWORKING AND HPX_WITH_PARCELPORT_MPI) OR HPX_WITH_ASYNC_MPI)
  list(APPEND init_runtime_optional_module_dependencies hpx_mpi_base)
endif()

include(HPX_AddModule)
add_hpx_module(
  full init_runtime
  GLOBAL_HEADER_GEN OFF
  HEADERS ${init_runtime_headers}
  SOURCES ${init_runtime_sources}
  DEPENDENCIES hpx_core hpx_parallelism
  MODULE_DEPENDENCIES
    hpx_init_runtime_local hpx_program_options hpx_runtime_local
    hpx_threadmanager ${init_runtime_optional_module_dependencies}
  CMAKE_SUBDIRS examples tests
)
