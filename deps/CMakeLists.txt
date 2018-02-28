### Build anyoption as library
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(anyoption
  STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/anyoption/anyoption.cpp
)
