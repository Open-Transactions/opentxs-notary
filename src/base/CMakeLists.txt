# Copyright (c) Monetas AG, 2014

set(name opentxs-server-base)

set(cxx-sources
  ServerSettings.cpp
  ConfigLoader.cpp
  AcctFunctor_PayDividend.cpp
  ClientConnection.cpp
  MessageProcessor.cpp
  MainFile.cpp
  UserCommandProcessor.cpp
  Notary.cpp
  Transactor.cpp
  OTServer.cpp
)

add_library(opentxs-server-base ${cxx-sources})

if(WIN32)
  target_link_libraries(${name} ${cash} ${core} ${ext} irrxml)
else()
  target_link_libraries(${name} ${cash} ${core} ${ext})
endif(WIN32)

if(WIN32)
  add_custom_command(TARGET opentxs-server-base POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/lib/$<CONFIGURATION>/opentxs-server-base.dll"
    "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/opentxs-server-base.dll")

  add_custom_command(TARGET opentxs-server-base POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_BINARY_DIR}/lib/$<CONFIGURATION>/opentxs-server-base.pdb"
    "${CMAKE_BINARY_DIR}/bin/$<CONFIGURATION>/opentxs-server-base.pdb")
endif(WIN32)

