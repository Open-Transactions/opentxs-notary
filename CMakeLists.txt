# Copyright (c) 2019 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)
project(otagent)
list(APPEND CMAKE_MODULE_PATH "${${PROJECT_NAME}_SOURCE_DIR}/cmake")
include(open-transactions-common)

# -----------------------------------------------------------------------------
# Setup

set_version_from_git()
set_build_type()
set_out_of_source_build()
update_git_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(OPENTXS_NTOARY_PEDANTIC_DEFAULT ON)
  set(CMAKE_VERBOSE_MAKEFILE ON)
else()
  set(OPENTXS_NTOARY_PEDANTIC_DEFAULT OFF)
endif()

# -----------------------------------------------------------------------------
# Options

option(OPENTXS_NTOARY_PEDANTIC_BUILD "Treat compiler warnings as errors."
       ${OPENTXS_NTOARY_PEDANTIC_DEFAULT})

# -----------------------------------------------------------------------------
# Set compiler options

set_compiler_flags(17 ${OPENTXS_NTOARY_PEDANTIC_BUILD})

# -----------------------------------------------------------------------------
# Print system information and build options

print_build_details(OPENTXS_NTOARY_PEDANTIC_BUILD "")

# -----------------------------------------------------------------------------
# Dependencies


find_package(Protobuf REQUIRED)
find_package(opentxs-proto REQUIRED)
find_package(opentxs REQUIRED)
find_package(Boost REQUIRED program_options)
find_package(
  Qt5
  COMPONENTS
    Core
    Gui
    Qml
    Widgets
)

# -----------------------------------------------------------------------------
# Source Definitions

set_common_defines()

# -----------------------------------------------------------------------------
# Build source

add_subdirectory(src)

# -----------------------------------------------------------------------------
# Package

generate_uninstall_target()
