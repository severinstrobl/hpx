# Copyright (c) 2019 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.6.3 FATAL_ERROR)

set(__filesystem_compatibility_default ON)
if(HPX_WITH_CXX17_FILESYSTEM)
  set(__filesystem_compatibility_default OFF)
endif()

include(HPX_Option)
# Compatibility with using Boost.FileSystem, introduced in V1.4.0
hpx_option(HPX_FILESYSTEM_WITH_BOOST_FILESYSTEM_COMPATIBILITY
  BOOL "Enable Boost.FileSystem compatibility. (default: ${__filesystem_compatibility_default})"
  ON ADVANCED CATEGORY "Modules")

set(__boost_filesystem)
if(HPX_FILESYSTEM_WITH_BOOST_FILESYSTEM_COMPATIBILITY)
  include(HPX_SetupBoostFilesystem)
  set(__boost_filesystem hpx::boost::filesystem)
endif()

set(filesystem_headers "hpx/filesystem.hpp")

include(HPX_AddModule)
add_hpx_module(filesystem
  FORCE_LINKING_GEN
  GLOBAL_HEADER_GEN OFF
  HEADERS ${filesystem_headers}
  DEPENDENCIES
    hpx_config
    ${__boost_filesystem}
  CMAKE_SUBDIRS examples tests
)

if(HPX_FILESYSTEM_WITH_BOOST_FILESYSTEM_COMPATIBILITY)
  hpx_info("    Boost library found: filesystem")
endif()
