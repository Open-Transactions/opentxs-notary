# Copyright (c) 2010-2023 The Open-Transactions developers This Source Code Form
# is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of
# the MPL was not distributed with this file, You can obtain one at
# http://mozilla.org/MPL/2.0/.

macro(opentxs_notary_print_build_details)
  include(otcommon-print-build-details)
  otcommon_print_build_details(
    OPENTXS_NOTARY_PEDANTIC_BUILD OPENTXS_NOTARY_BUILD_TESTS
  )
endmacro()
