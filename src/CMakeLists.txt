# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

find_path(opentxsIncludePath opentxs/core/OTglobal.h)

include_directories(
  ${opentxsIncludePath}
)
   
set(cxx-sources
  OTClientConnection.cpp
  OTServer.cpp
  main.cpp
)

find_library(core opentxs-core)
find_library(ext opentxs-ext)

add_executable(${name} ${cxx-sources})
target_link_libraries(${name} ${core} ${ext})

add_subdirectory(createmint)

install(TARGETS opentxs-server DESTINATION bin)
