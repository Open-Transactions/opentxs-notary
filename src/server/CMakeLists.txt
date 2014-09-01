# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

set(cxx-sources
  main.cpp
)

add_executable(${name} ${cxx-sources})

target_link_libraries(${name} opentxs-server-base ${core} ${ext})

install(TARGETS opentxs-server DESTINATION bin)
