# Copyright (c) Monetas AG, 2014

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PEDANTIC_CXX_FLAGS}")

include_directories(${OPENTXS_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/../deps/)

add_subdirectory(notary)
