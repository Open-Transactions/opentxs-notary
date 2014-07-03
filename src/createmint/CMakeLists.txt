# Copyright (c) Monetas AG, 2014

set(name createmint)

find_path(opentxsIncludePath opentxs/core/OTglobal.h)

include_directories(
  ${opentxsIncludePath}
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
