# Copyright (c) Monetas AG, 2014

cmake_minimum_required(VERSION 2.8)
project(opentxs-server)

set (VERSION_MAJOR  0)
set (VERSION_MINOR  0)
set (VERSION_BUGFIX 1)


#-----------------------------------------------------------------------------
# Options for building

option(BUILD_VERBOSE       "Verbose build output." ON)
option(BUILD_TESTS         "Build the unit tests." ON)
option(DISABLE_LIBCXX      "Disable using libc++ when compiling with clang." OFF )
option(RPM                 "Build a RPM instead of a tarball" OFF )

if(BUILD_VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE true)
endif()


#-----------------------------------------------------------------------------
# Print system information

message(STATUS "CMake version:   ${CMAKE_VERSION}")
message(STATUS "System:          ${CMAKE_SYSTEM}")
message(STATUS "Processor:       ${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "Verbose:         ${BUILD_VERBOSE}")
message(STATUS "Testing:         ${BUILD_TESTS}")
message(STATUS "Documentation:   ${BUILD_DOCUMENTATION}")
message(STATUS "Build RPM:       ${RPM}")


#-----------------------------------------------------------------------------
# Collect all binaries into bin subdirectory

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


#-----------------------------------------------------------------------------
# Check which compiler to use

if (${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if (HAVE_STD11)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "No advanced standard C++ support (-std=c++11 not defined).")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if (HAVE_STD11)
    if (DISABLE_LIBCXX)
      #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    else()
      #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -Wall")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
      set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -stdlib=libc++")
    endif()
  else()
    message(FATAL_ERROR "No C++11 support for Clang version. Please upgrade Clang to a version supporting C++11.")
  endif()
endif()


message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Flags:       ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ link flags:  ${CMAKE_CXX_LINK_FLAGS}")


#-----------------------------------------------------------------------------
# System libraries used for linking.

if(UNIX AND NOT APPLE)
  list(APPEND OPENTXS_AUDITOR_SYSTEM_LIBRARIES rt)
  list(APPEND OPENTXS_AUDITOR_SYSTEM_LIBRARIES dl)
endif()

if (MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif(MSVC)
if (WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)


#-----------------------------------------------------------------------------
# Third party libraries

find_package( opentxs )	#lclc


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

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/lib)

############################################################################
## Build Unit tests
#
# if(BUILD_TESTS)
#   enable_testing()
#   if(MSVC11)
#     add_definitions(-D_VARIADIC_MAX=10)
#   endif(MSVC11)
# 
#   if(MSVC)
#     set(gtest_force_shared_crt ON CACHE BOOL "Override gtest option.")
#   endif(MSVC)
# 
#   # gmock automatically searches for gtest
#   add_subdirectory(deps/gmock)
# 
#   set(GTEST_ROOT ${opentxs-server_SOURCE_DIR}/deps/gtest)
#   set(GTEST_FOUND ON)
#   set(GTEST_INCLUDE_DIRS ${GTEST_ROOT}/include)
#   set(GTEST_LIBRARY gtest)
#   set(GTEST_MAIN_LIBRARY gtest_main)
#   set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})
# 
#   set(GMOCK_ROOT ${opentxs-server_SOURCE_DI}/deps/gmock)
#   set(GMOCK_FOUND ON)
#   set(GMOCK_INCLUDE_DIRS ${GTEST_INCLUDE_DIRS} ${GMOCK_ROOT}/include)
#   set(GMOCK_LIBRARY ${GTEST_LIBRARIES} gmock)
#   set(GMOCK_MAIN_LIBRARY gmock_main)
#   set(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARY} ${GMOCK_MAIN_LIBRARY})
# endif()


############################################################################
## Build Documentation
#
# if(BUILD_DOCUMENTATION)
#    find_package(Doxygen)
#    if (DOXYGEN_FOUND)
#      configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
#      add_custom_target(doc
#        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating API documentation with Doxygen" VERBATIM)
#      set(CPP-NETLIB_GENERATE_DOCS ON)
#    endif(DOXYGEN_FOUND)
# endif(BUILD_DOCUMENTATION)

add_subdirectory(src)

# if(BUILD_TESTS)
#   enable_testing()
#   add_subdirectory(tests)
# endif(BUILD_TESTS)


############################################################################
### Install
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/opentxs-server ${CMAKE_CURRENT_BINARY_DIR}/bin/createmint
        DESTINATION bin)

###Packaging
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_NAME "opentxs-server")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "Lucas Betschart")
set(CPACK_PACKAGE_VENDOR "Monetas AG")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")

#RPM
if(RPM)
  set(CPACK_GENERATOR "RPM")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/opentxs-server.spec.in" "${CMAKE_CURRENT_BINARY_DIR}/opentxs-server.spec" @ONLY IMMEDIATE)
  set(CPACK_RPM_USER_BINARY_SPECFILE "${CMAKE_CURRENT_BINARY_DIR}/opentxs-server.spec")
endif(RPM)


include(CPack)
