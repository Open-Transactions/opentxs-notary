# Copyright (c) Monetas AG, 2014

set(cxx-sources
	OTClientConnection.cpp
	OTServer.cpp
	main.cpp
   )


add_subdirectory(createmint)
   
add_executable(opentxs-server ${cxx-sources})


include_directories("/usr/local/include/opentxs/" ${CMAKE_CURRENT_SOURCE_DIR})	#lclc

link_directories("/usr/local/lib/")	#lclc

target_link_libraries(opentxs-server otapi otextensions ot)	#lclc