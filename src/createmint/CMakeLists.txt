# Copyright (c) Monetas AG, 2014

set(name createmint)

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/opentxs
)

link_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/lib
)

set(cxx-sources
  ../OTServer.cpp
  ../OTClientConnection.cpp
  main.cpp
)
  
add_executable(${name} ${cxx-sources})
target_link_libraries(${name} opentxs-api opentxs-ext opentxs-core)
