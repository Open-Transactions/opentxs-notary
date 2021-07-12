# Copyright (c) 2011-2021 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.14)
project(opentxs-notary)
list(
  APPEND
  CMAKE_MODULE_PATH
  "${${PROJECT_NAME}_SOURCE_DIR}/cmake"
)
include(open-transactions-common)

# -----------------------------------------------------------------------------
# Setup

set_version_from_git()
set_build_type()
set_out_of_source_build()
update_git_submodules()

# -----------------------------------------------------------------------------
# Set option defaults

if(CMAKE_BUILD_TYPE
   STREQUAL
   "Debug"
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

# -----------------------------------------------------------------------------
# Set compiler options

set_compiler_flags(17 ${OPENTXS_NOTARY_PEDANTIC_BUILD})

# -----------------------------------------------------------------------------
# Print system information and build options

print_build_details(OPENTXS_NOTARY_PEDANTIC_BUILD "")

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

# Inherited by opentxs. May or may not be used but if opentxs links against them
# then these find_package calls must have been made.

find_package(
  Boost
  1.73.0
  QUIET
  COMPONENTS
    system
    filesystem
    thread
    iostreams
  OPTIONAL_COMPONENTS stacktrace_basic
)
find_package(ZLIB QUIET)
find_package(unofficial-zeromq QUIET)
find_package(
  OpenSSL
  1.0.1
  QUIET
)
find_package(unofficial-secp256k1 QUIET)
find_package(unofficial-sodium QUIET)
find_package(Protobuf QUIET)
find_package(lmdb QUIET)
find_package(
  Qt5
  COMPONENTS
    Core
    Gui
    Qml
    Widgets
  QUIET
)
find_package(SQLite3 QUIET)
find_package(opendht QUIET)

# -----------------------------------------------------------------------------
# Source Definitions

set_common_defines()

# -----------------------------------------------------------------------------
# Build source

add_subdirectory(src)

# -----------------------------------------------------------------------------
# Package

generate_uninstall_target()
