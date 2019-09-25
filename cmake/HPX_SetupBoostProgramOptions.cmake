# Copyright (c) 2019 The STE||AR-Group
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

if(HPX_PROGRAM_OPTIONS_WITH_BOOST_PROGRAM_OPTIONS_COMPATIBILITY)

  hpx_add_config_define_namespace(
    DEFINE HPX_PROGRAM_OPTIONS_HAVE_BOOST_PROGRAM_OPTIONS_COMPATIBILITY
    NAMESPACE PROGRAM_OPTIONS)

  find_package(Boost ${Boost_MINIMUM_VERSION}
    QUIET MODULE
    COMPONENTS program_options)

  if(NOT Boost_PROGRAM_OPTIONS_FOUND)
    hpx_error("Could not find Boost.ProgramOptions. Provide a boost installation including the program_options library")
  endif()

  add_library(hpx::boost::program_options INTERFACE IMPORTED)
  set_property(TARGET hpx::boost::program_options PROPERTY INTERFACE_LINK_LIBRARIES ${Boost_PROGRAM_OPTIONS_LIBRARIES})

endif()
