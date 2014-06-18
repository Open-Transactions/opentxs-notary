
common_includes        		=  $(AM_CXXFLAGS) $(DEPS_CFLAGS)	

bin_PROGRAMS            	=  opentxsserver createmint

opentxsserver_dir		=   src

opentxsserver_common		=   $(opentxsserver_dir)/OTClientConnection.cpp		\
				    $(opentxsserver_dir)/OTServer.cpp

opentxsserver_headers       	=   $(opentxsserver_dir)/OTClientConnection.hpp		\
				    $(opentxsserver_dir)/OTServer.hpp

opentxsserver_SOURCES		=   $(opentxsserver_dir)/main.cpp			\
				    $(opentxsserver_common)				\
				    $(opentxsserver_headers)

opentxsserver_CXXFLAGS		=  $(common_includes)		\
				   -I$(opentxsserver_dir)	\
				   -Iopentxs/include		\
				   -Iopentxs/include/otlib	\
				   -Iopentxs/include/misc

if KEYRING_FLATFILE

opentxsserver_CXXFLAGS       	+= -DOT_KEYRING_FLATFILE

endif

opentxsserver_LDADD		=  $(DEPS_LIBS) -Lopentxs -lotextensions -lirrxml

#### Createmint  (includes files from opentxs-server)

createmint_sources      =   $(opentxsserver_dir)/createmint/main.cpp

createmint_SOURCES      =   $(createmint_sources)	\
			    $(opentxsserver_common)	\
			    $(opentxsserver_headers)

createmint_CXXFLAGS     =   $(common_includes)		\
			    -I$(opentxsserver_dir)	\
			    -I/usr/local/include/opentxs

createmint_LDADD        =   $(DEPS_LIBS)
