# Copyright (c) Monetas AG, 2014

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

set(dependency_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/../deps/
)

if(MSVC)
  include_directories(
    ${PROJECT_SOURCE_DIR}/../opentxs/include
    ${PROJECT_SOURCE_DIR}/../opentxs/include/opentxs/core
    ${PROJECT_SOURCE_DIR}/../opentxs/deps
  )
  link_directories(
    ${PROJECT_SOURCE_DIR}/../opentxs/build/lib
  )
  set(core opentxs-core)
  set(cash opentxs-cash)
  set(ext opentxs-ext)
  set(server opentxs-server)
else()
  find_package(opentxs REQUIRED)
  include_directories(${dependency_include_dir} ${OPENTXS_INCLUDE_DIRS})
  set(core opentxs-core)
  set(cash opentxs-cash)
  set(ext opentxs-ext)
  set(server opentxs-server)
endif()

add_subdirectory(notary)
add_subdirectory(createmint)
