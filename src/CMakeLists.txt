# Copyright (c) Monetas AG, 2014

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

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
  set(basket opentxs-basket)
  set(cash opentxs-cash)
  set(ext opentxs-ext)
  set(server opentxs-server)
else()
  find_path(opentxsIncludePath opentxs/core/OTPseudonym.hpp)

  include_directories(
    ${opentxsIncludePath}
    ${opentxsIncludePath}/opentxs/core/
  )

  find_library(core opentxs-core)
  find_library(cash opentxs-cash)
  find_library(basket opentxs-basket)
  find_library(ext opentxs-ext)
  find_library(server opentxs-server)
endif()

add_subdirectory(notary)
add_subdirectory(createmint)
