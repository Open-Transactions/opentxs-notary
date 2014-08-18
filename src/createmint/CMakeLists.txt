# Copyright (c) Monetas AG, 2014

set(name createmint)

set(cxx-sources
  main.cpp
)

add_executable(${name} ${cxx-sources})
if(MSVC)
  target_link_libraries(${name} opentxs-server-base ${core} ${ext} irrxml)
else()
  target_link_libraries(${name} opentxs-server-base ${core} ${ext})
endif(MSVC)

install(TARGETS createmint DESTINATION bin)
