# Copyright (c) 2018 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(MODULE_NAME opentxs-notary)

set(cxx-sources
  Client.cpp
  Client.hpp
  main.cpp
)

if (WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/winexe.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
    @ONLY
  )

  add_executable(
    ${MODULE_NAME}
    ${cxx-sources}
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
  )
else()
  add_executable(${MODULE_NAME} ${cxx-sources})
endif()

target_link_libraries(
  ${MODULE_NAME}
  PRIVATE
  Threads::Threads
  ${OPENTXS_PROTO_LIBRARIES}
  ${OPENTXS_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

target_include_directories(
  ${MODULE_NAME}
  PRIVATE
  ${OPENTXS_PROTO_INCLUDE_DIRS}
  ${OPENTXS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 17)

install(TARGETS ${MODULE_NAME} DESTINATION bin)
