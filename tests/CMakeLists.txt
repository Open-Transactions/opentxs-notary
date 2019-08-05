# Copyright (c) 2018 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(MODULE_NAME unittests-opentxs-notary)

set(cxx-sources
  Test_Example.cpp
)

add_executable(${MODULE_NAME} ${cxx-sources})

set_target_properties(${MODULE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)

target_link_libraries(
  ${MODULE_NAME}
  PRIVATE
  Threads::Threads
  ${OPENTXS_PROTO_LIBRARIES}
  ${OPENTXS_LIBRARIES}
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
)

if(OT_WITH_QT)
   target_link_libraries(${MODULE_NAME} PRIVATE ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} ${Qt5Qml_LIBRARIES} ${Qt5Widgets_LIBRARIES})
endif()

target_include_directories(
  ${MODULE_NAME}
  PRIVATE
  ${OPENTXS_PROTO_INCLUDE_DIRS}
  ${OPENTXS_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

set_property(TARGET ${MODULE_NAME} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_property(TARGET ${MODULE_NAME} PROPERTY CXX_STANDARD 17)

add_test(${MODULE_NAME} ${PROJECT_BINARY_DIR}/tests/${MODULE_NAME} --gtest_output=xml:gtestresults.xml)


