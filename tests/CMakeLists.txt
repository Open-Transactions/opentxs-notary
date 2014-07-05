set(NAME unittests-opentxs-server)

include_directories(
  ${opentxs-server_SOURCE_DIR}/deps/gtest/include
  ${opentxs-server_SOURCE_DIR}/src
)

set(SRC
  Test_Example.cpp
)

add_executable(${NAME} ${SRC})

target_link_libraries(${NAME}
  ${GTEST_BOTH_LIBRARIES}
  ${opentxs-server_SYSTEM_LIBRARIES}
)

set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${opentxs-server_BINARY_DIR}/tests)

add_test(${NAME} ${opentxs-server_BINARY_DIR}/tests/${NAME} --gtest_output=xml:gtestresults.xml)
