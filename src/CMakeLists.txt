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
else()
  find_path(opentxsIncludePath opentxs/core/OTglobal.h)

  include_directories(
    ${opentxsIncludePath}
  )

  find_library(core opentxs-core)
  find_library(ext opentxs-ext)
endif(MSVC)
   
add_subdirectory(server)
add_subdirectory(createmint)
