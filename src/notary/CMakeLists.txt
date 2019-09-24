#0 Copyright (c) 2019 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

set(cxx-sources
  Client.cpp
  Client.hpp
  main.cpp
)

add_executable(opentxs-notary ${cxx-sources})

target_link_libraries(
  opentxs-notary
  PRIVATE
  Threads::Threads
  opentxs
  Boost::program_options
)

install(TARGETS opentxs-notary DESTINATION bin)
