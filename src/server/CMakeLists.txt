# Copyright (c) Monetas AG, 2014

set(name opentxs-server)

set(cxx-sources
  main.cpp
)

set(MODULE_NAME opentxs-server)
if (WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/winexe.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
    @ONLY
  )

  add_executable(
    ${MODULE_NAME}
    ${cxx-sources}
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
  )
else()
  add_executable(${MODULE_NAME} ${cxx-sources})
endif()

target_link_libraries(${name} opentxs-server-base ${ext} ${basket} ${core})

install(TARGETS opentxs-server DESTINATION bin)
