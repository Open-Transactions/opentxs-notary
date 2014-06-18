# Copyright (c) Monetas AG, 2014

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/misc
  ${PROJECT_SOURCE_DIR}/opentxs/include/otlib
)

link_directories(
  ${PROJECT_SOURCE_DIR}/opentxs
)

add_subdirectory(createmint)
   
set(cxx-sources
  OTClientConnection.cpp
  OTServer.cpp
  main.cpp
)

add_executable(opentxs-server ${cxx-sources})
target_link_libraries(opentxs-server otapi otextensions ot)
