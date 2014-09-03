# Copyright (c) Monetas AG, 2014

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

if(MSVC)
  include_directories(
    ${PROJECT_SOURCE_DIR}/../opentxs/include
    ${PROJECT_SOURCE_DIR}/../opentxs/deps
  )
  link_directories(
    ${PROJECT_SOURCE_DIR}/../opentxs/build/lib
  )
  set(core opentxs-core)
  set(ext opentxs-ext)
  set(cash opentxs-cash)
else()
  find_path(opentxsIncludePath opentxs/core/OTPseudonym.hpp)

  include_directories(
    ${opentxsIncludePath}
    ${opentxsIncludePath}/opentxs/core/
  )

  find_library(core opentxs-core)
  find_library(ext opentxs-ext)
  find_library(cash opentxs-cash)
endif(MSVC)

add_subdirectory(base)
add_subdirectory(server)
add_subdirectory(createmint)
