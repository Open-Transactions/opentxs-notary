# Copyright (c) Monetas AG, 2014

set(cxx-sources
	../OTServer.cpp
	../OTClientConnection.cpp
	main.cpp
   )
  
add_executable(createmint ${cxx-sources})


include_directories(${CMAKE_CURRENT_SOURCE_DIR} "./../" "/usr/local/include/opentxs/" )	#lclc

link_directories("/usr/local/lib/")	#lclc

target_link_libraries(createmint otapi otextensions ot)	#lclc