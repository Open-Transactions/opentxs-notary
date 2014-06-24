# Copyright (c) Monetas AG, 2014

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/opentxs
)

link_directories(
  ${PROJECT_SOURCE_DIR}/opentxs
)
   
set(cxx-sources
  OTClientConnection.cpp
  OTServer.cpp
  main.cpp
)

add_executable(opentxs-server ${cxx-sources})
target_link_libraries(opentxs-server opentxs-api opentxs-ext opentxs-core)


add_subdirectory(createmint)
