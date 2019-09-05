# Copyright (c) 2019 The STE||AR-Group
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This files is including the external targets which are imported by the modules
# as they are not available in the main CMakeLists.txt scope, those targets
# exist in the module scope.
# We decided to leave it that way and not make the
# include(HPX_BoostProgramOptions) global to the whole HPX project because it
# seems more clean
include(HPX_SetupBoostProgramOptions)
include(HPX_SetupBoostFilesystem)
