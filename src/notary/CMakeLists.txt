# Copyright (c) 2018 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(MODULE_NAME opentxs-notary)

set(cxx-sources
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

target_link_libraries(${MODULE_NAME} ${OPENTXS_LIBRARIES})
target_link_libraries(${MODULE_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_include_directories(${MODULE_NAME} PUBLIC ${OPENTXS_INCLUDE_DIRS} )
target_include_directories(${MODULE_NAME} PUBLIC ${Boost_INCLUDE_DIRS} )
install(TARGETS ${MODULE_NAME} DESTINATION bin)
