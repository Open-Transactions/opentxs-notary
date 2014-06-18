# Copyright (c) Monetas AG, 2014

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/otlib
  ${PROJECT_SOURCE_DIR}/opentxs/include/misc
)

link_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/
)

set(cxx-sources
  ../OTServer.cpp
  ../OTClientConnection.cpp
  main.cpp
)
  
add_executable(createmint ${cxx-sources})
target_link_libraries(createmint otapi otextensions ot)
