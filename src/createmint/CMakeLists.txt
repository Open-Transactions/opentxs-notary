# Copyright (c) Monetas AG, 2014

set(name createmint)

set(cxx-sources
  ../AcctFunctor_PayDividend.cpp
  ../OTServer.cpp
  ../ClientConnection.cpp
  main.cpp
)

add_executable(${name} ${cxx-sources})
if(MSVC)
  target_link_libraries(${name} ${core} ${ext} irrxml)
else()
  target_link_libraries(${name} ${core} ${ext})
endif(MSVC)

install(TARGETS createmint DESTINATION bin)
