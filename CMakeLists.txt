# Copyright (c) 2011-2023 The Open-Transactions developers This Source Code Form
# is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of
# the MPL was not distributed with this file, You can obtain one at
# http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.21.0)
cmake_policy(
  SET
  CMP0091
  NEW
)
set(CMAKE_OSX_DEPLOYMENT_TARGET 11)
project(opentxs-notary)
list(
  APPEND
  CMAKE_MODULE_PATH
  "${${PROJECT_NAME}_SOURCE_DIR}/tools/cmake"
)

# -----------------------------------------------------------------------------
# Setup

find_package(
  otcommon
  2
  CONFIG
  REQUIRED
)
otcommon_set_build_type()
otcommon_require_out_of_source_build()
otcommon_update_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
   OR CMAKE_BUILD_TYPE
      STREQUAL
      "RelWithDebInfo"
)
  set(OPENTXS_NOTARY_PEDANTIC_DEFAULT ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)
else()
  set(OPENTXS_NOTARY_PEDANTIC_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(
  OPENTXS_NOTARY_PEDANTIC_BUILD
  "Treat compiler warnings as errors."
  ${OPENTXS_NOTARY_PEDANTIC_DEFAULT}
)

option(
  OPENTXS_NOTARY_IWYU
  "Enable include-what-you-use static analysis"
  OFF
)

if(OPENTXS_NOTARY_IWYU)
  otcommon_make_iwyu_target()
endif()

# -----------------------------------------------------------------------------
# Set compiler options

include(opentxs-notary-configure-target)
otcommon_set_project_language_standard(
  99
  20
  ${OPENTXS_NOTARY_PEDANTIC_BUILD}
)

# -----------------------------------------------------------------------------
# Print system information and build options

include(opentxs-notary-print-build-details)
opentxs_notary_print_build_details()

# -----------------------------------------------------------------------------
# Dependencies

find_package(
  opentxs
  CONFIG
  REQUIRED
)
find_package(
  Boost
  REQUIRED
  program_options
)

# -----------------------------------------------------------------------------
# Build source

add_subdirectory(src)
