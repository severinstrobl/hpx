# Copyright (c) 2019 The STE||AR-Group
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.3.2 FATAL_ERROR)

set(execution_headers
  hpx/execution.hpp
  hpx/execution/execution_context.hpp
  hpx/execution/execution_context_base.hpp
  hpx/execution/register_locks.hpp
  hpx/execution/this_thread.hpp
)

set(concurrency_compat_headers
  hpx/util/register_locks.hpp
)

set(execution_sources
  execution_context.cpp
  register_locks.cpp
  this_thread.cpp
)

include(HPX_AddModule)
add_hpx_module(execution
  COMPATIBILITY_HEADERS ON
  DEPRECATION_WARNINGS
  GLOBAL_HEADER_GEN OFF
  FORCE_LINKING_GEN
  SOURCES ${execution_sources}
  HEADERS ${execution_headers}
  COMPAT_HEADERS ${execution_compat_headers}
  DEPENDENCIES hpx_assertion hpx_config hpx_errors hpx_format hpx_timing
  CMAKE_SUBDIRS examples tests
)
