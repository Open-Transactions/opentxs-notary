include(FindPkgConfig)
  PKG_CHECK_MODULES(PC_OPENTXS "libopentxs")

  find_path(
      OPENTXS_INCLUDE_DIRS
      NAMES opentxs
      HINTS ${PC_OPENTXS_INCLUDE_DIRS}
  )

  find_library(
      OPENTXS_LIBRARIES
      NAMES libopentxs.dylib libopentxs.so libopentxs.a
      HINTS ${PC_OPENTXS_LIBRARY_DIRS}
  )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OPENTXS DEFAULT_MSG OPENTXS_LIBRARIES OPENTXS_INCLUDE_DIRS)
  mark_as_advanced(OPENTXS_LIBRARIES OPENTXS_INCLUDE_DIRS)
