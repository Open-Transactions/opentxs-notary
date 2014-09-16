# Copyright (c) Monetas AG, 2014

set(name unittests-opentxs-notary)

include_directories(
  ${GTEST_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/src
)

set(SRC
  Test_Example.cpp
)

add_executable(${name} ${SRC})
target_link_libraries(${name} ${GTEST_BOTH_LIBRARIES})
set_target_properties(${name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/tests)
add_test(${name} ${PROJECT_BINARY_DIR}/tests/${name} --gtest_output=xml:gtestresults.xml)
