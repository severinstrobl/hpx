# Copyright (c) 2019 The STE||AR-Group
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

set(tests
    execution_context
   )

foreach(test ${tests})
  set(sources
      ${test}.cpp)

  source_group("Source Files" FILES ${sources})

  # add example executable
  add_hpx_executable(${test}_test
                     SOURCES ${sources}
                     NOLIBS
                     DEPENDENCIES hpx_execution hpx_testing ${BOOST_UNDERLYING_THREAD_LIBRARY}
                     EXCLUDE_FROM_ALL
                     FOLDER "Tests/Unit/Modules/Execution")

  add_hpx_unit_test("modules.execution" ${test} ${${test}_PARAMETERS})

endforeach()

