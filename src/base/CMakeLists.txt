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

file(GLOB cxx-headers "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

set(MODULE_NAME opentxs-server-base)
if(WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/windll.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
    @ONLY
  )

  # we have an extra Windows-specific file to compile
  add_library(${MODULE_NAME}
    ${cxx-sources}
    ${cxx-headers}
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
  )
else()
  add_library(${MODULE_NAME}
    ${cxx-sources}
    ${cxx-headers}
  )
endif()

if(WIN32)
  target_link_libraries(${name} ${ext} ${cash} ${basket} ${core} irrxml)
else()
  target_link_libraries(${name} ${ext} ${cash} ${basket} ${core})
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
