# Copyright (c) Monetas AG, 2014

set(cxx-sources
  AcctFunctor_PayDividend.cpp
  ClientConnection.cpp
  MessageProcessor.cpp
  OTServer.cpp
)

add_library(opentxs-server-base ${cxx-sources})

target_link_libraries(opentxs-server-base ${core} ${ext})
