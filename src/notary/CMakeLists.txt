# Copyright (c) 2011-2023 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_executable(opentxs-notary "main.cpp")
opentxs_notary_configure_cxx_target("opentxs-notary")
target_link_libraries(
  opentxs-notary
  PUBLIC
    Threads::Threads
    opentxs::libopentxs
    Boost::program_options
)
install(TARGETS opentxs-notary DESTINATION bin)
