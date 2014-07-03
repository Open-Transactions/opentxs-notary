# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/opentxs
)

link_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/lib
)
   
set(cxx-sources
  OTClientConnection.cpp
  OTServer.cpp
  main.cpp
)

add_executable(${name} ${cxx-sources})
target_link_libraries(${name} opentxs-ext opentxs-core)

add_subdirectory(createmint)
