# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

set(cxx-sources
  ../base/AcctFunctor_PayDividend.cpp
  ../base/MessageProcessor.cpp
  ../base/ClientConnection.cpp
  ../base/OTServer.cpp
  main.cpp
)

add_executable(${name} ${cxx-sources})
if(MSVC)
  target_link_libraries(${name} ${core} ${ext} irrxml)
else()
  target_link_libraries(${name} ${core} ${ext})
endif(MSVC)

install(TARGETS opentxs-server DESTINATION bin)
