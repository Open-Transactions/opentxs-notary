# Copyright (c) Monetas AG, 2014

set(name createmint)

include_directories(
  ${PROJECT_SOURCE_DIR}/opentxs/include
  ${PROJECT_SOURCE_DIR}/opentxs/include/opentxs
)

set(cxx-sources
  ../OTServer.cpp
  ../OTClientConnection.cpp
  main.cpp
)

find_library(core opentxs-core)
find_library(ext opentxs-ext)

add_executable(${name} ${cxx-sources})
target_link_libraries(${name} ${core} ${ext})

install(TARGETS createmint DESTINATION bin)
