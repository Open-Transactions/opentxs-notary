# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/opentxs
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
