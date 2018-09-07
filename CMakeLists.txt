# Copyright (c) 2018 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.5)
project(opentxs-notary)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM LAST)
find_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND git "describe" OUTPUT_VARIABLE GIT_VERSION WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${GIT_VERSION}")
    string(REGEX REPLACE "^${VERSION_MAJOR}\\.([0-9]+).*" "\\1" VERSION_MINOR "${GIT_VERSION}")
    string(REGEX REPLACE "^${VERSION_MAJOR}\\.${VERSION_MINOR}\\.([0-9]+).*" "\\1" VERSION_BUGFIX "${GIT_VERSION}")
    string(REGEX REPLACE "^${VERSION_MAJOR}\\.${VERSION_MINOR}\\.${VERSION_BUGFIX}-([0-9]+)-.*" "\\1" VERSION_NEW_COMMITS "${GIT_VERSION}")
    string(REGEX REPLACE "^${VERSION_MAJOR}\\.${VERSION_MINOR}\\.${VERSION_BUGFIX}-${VERSION_NEW_COMMITS}-(.*)" "\\1" VERSION_SHA1 "${GIT_VERSION}")
    if("${VERSION_NEW_COMMITS}" STREQUAL "${GIT_VERSION}")
        set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}")
        set(VERSION_NEW_COMMITS "")
        set(VERSION_SHA1 "")
    else()
        set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BUGFIX}-${VERSION_NEW_COMMITS}-${VERSION_SHA1}")
    endif()
    if ("${VERSION_STRING}" STREQUAL "..")
        message(FATAL_ERROR "Version string missing. Did you git clone using the --depth parameter?")
    endif()
else()
  message(FATAL_ERROR "Git not found.")
endif()


#-----------------------------------------------------------------------------
# Options for building

option(BUILD_VERBOSE       "Verbose build output." ON)
option(BUILD_TESTS         "Build the unit tests." ON)
option(KEYRING_FLATFILE    "Build with Flatfile Keyring" OFF)
option(RPM                 "Build a RPM" OFF)
option(DEB                 "Build a DEB" OFF)

set(PACKAGE_CONTACT        ""                              CACHE <TYPE>  "Package Maintainer")
set(PACKAGE_VENDOR         "Open Transactions Developers"  CACHE <TYPE>  "Package Vendor")

if(BUILD_VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE true)
endif()

if(RPM OR DEB)
    if(NOT PACKAGE_CONTACT)
        message(FATAL_ERROR "Specify a PACKAGE_CONTACT to build packages.")
    endif()
endif()


#-----------------------------------------------------------------------------
# Print system information

message(STATUS "opentxs-notary version:   ${VERSION_STRING}")
message(STATUS "CMake version:            ${CMAKE_VERSION}")
message(STATUS "System:                   ${CMAKE_SYSTEM}")
message(STATUS "Processor:                ${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "Verbose:                  ${BUILD_VERBOSE}")
message(STATUS "Testing:                  ${BUILD_TESTS}")
message(STATUS "Flatfile keyring:         ${KEYRING_FLATFILE}")
message(STATUS "Build RPM:                ${RPM}")
message(STATUS "Build DEB:                ${DEB}")
message(STATUS "Package Contact:          ${PACKAGE_CONTACT}")
message(STATUS "Package Vendor:           ${PACKAGE_VENDOR}")

#-----------------------------------------------------------------------------
# Collect all binaries into bin subdirectory

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/lib)


#-----------------------------------------------------------------------------
# Compile options

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PEDANTIC_CXX_FLAGS "-Werror")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  set(PEDANTIC_CXX_FLAGS "${PEDANTIC_CXX_FLAGS} -Wall -Wextra -pedantic -pedantic-errors -Wswitch-default -Wswitch-enum -Wcast-align -Wdisabled-optimization -Wmissing-declarations -Wmissing-field-initializers -Wunused-macros -Wundef -Wformat=2 -Wlogical-op -Wmissing-include-dirs")
  set(PEDANTIC_CXX_FLAGS "${PEDANTIC_CXX_FLAGS} -Weffc++ -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual -Wsynth -Wnoexcept -Wstrict-null-sentinel")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
  set(PEDANTIC_CXX_FLAGS "${PEDANTIC_CXX_FLAGS} -Weverything -Wno-c++98-compat-pedantic -Wno-padded -Wno-weak-vtables -Wno-disabled-macro-expansion -Wno-sign-conversion -Wno-float-conversion -Wno-covered-switch-default")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

message(STATUS "C Compiler ID:      ${CMAKE_C_COMPILER_ID}")
message(STATUS "C Flags:            ${CMAKE_C_FLAGS}")
message(STATUS "C link flags:       ${CMAKE_C_LINK_FLAGS}")
message(STATUS "C++ Compiler ID:    ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Flags:          ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ link flags:     ${CMAKE_CXX_LINK_FLAGS}")

#-----------------------------------------------------------------------------
# System libraries used for linking.

if(UNIX AND NOT APPLE)
  list(APPEND OPENTXS_NOTARY_SYSTEM_LIBRARIES rt)
  list(APPEND OPENTXS_NOTARY_SYSTEM_LIBRARIES dl)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(opentxs-proto REQUIRED)
find_package(opentxs REQUIRED)
find_package(Boost REQUIRED program_options)

#-----------------------------------------------------------------------------
# Force out-of-source build

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
message(FATAL_ERROR "CMake generation is not allowed within the source directory!
Remove the CMakeCache.txt file and try again from another folder, e.g.:

   rm CMakeCache.txt
   mkdir build
   cd build
   cmake ..
")
endif()


#-----------------------------------------------------------------------------
# Default to Release build

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()


#-----------------------------------------------------------------------------
# Unit Testing

if(BUILD_TESTS)
  include(CTest)
  find_package(GTest REQUIRED)
  enable_testing()
endif()


#-----------------------------------------------------------------------------
# Get submodules

execute_process(COMMAND git "submodule" "update" "--init" "--recursive")


#-----------------------------------------------------------------------------
# Source Definitions

add_definitions(-DOPENTXS_SERVER_VERSION_STRING="${VERSION_STRING}")

if(KEYRING_FLATFILE)
  add_definitions(-DOT_KEYRING_FLATFILE)
endif()

if(WIN32)
    add_definitions("-DEXPORT=__declspec(dllexport)")
else()
  add_definitions(-DEXPORT=)
endif()

#-----------------------------------------------------------------------------
# Build source

#Fix RPATH
if(APPLE)
  set(CMAKE_INSTALL_NAME_DIR @rpath)
endif()
if(NOT RPM)
  include(GNUInstallDirs)
  SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

LIST(APPEND CMAKE_PREFIX_PATH "/usr/share/cmake/Modules")
LIST(APPEND CMAKE_PREFIX_PATH "/usr/local/share/cmake/Modules")

add_subdirectory(src)
add_subdirectory(tests)


#-----------------------------------------------------------------------------
# Uninstal
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)


#-----------------------------------------------------------------------------
# Packaging
set(CPACK_GENERATOR "TGZ")

set(CPACK_PACKAGE_VERSION ${VERSION_STRING})
set(CPACK_PACKAGE_NAME "opentxs-notary")
set(CPACK_PACKAGE_CONTACT ${PACKAGE_CONTACT})
set(CPACK_PACKAGE_VENDOR ${PACKAGE_VENDOR})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Open Transactions Server")
set(CPACK_PACKAGE_DESCRIPTION "
Open-Transactions democratizes financial and monetary actions.
You can use it for issuing currencies/stock, paying dividends, creating asset accounts, sending/receiving digital cash, writing/depositing cheques, cashier's cheques, creating basket currencies, trading on markets, scripting custom agreements, recurring payments, escrow, etc.
")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(OPEN_TRANSACTIONS_WEBSITE "http://opentransactions.org")

if(RPM)
  set(CPACK_GENERATOR "RPM")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog")
endif(RPM)

if(DEB)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "opentxs-main")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${OPEN_TRANSACTIONS_WEBSITE})
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${PACKAGE_CONTACT})
endif(DEB)

include(CPack)
