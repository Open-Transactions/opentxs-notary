#!/usr/bin/env bash

# define OT_FAIL as assert(false) such that cppcheck can recognize early exits
define="-DOT_FAIL=assert(false)"
# Exit code '1' is returned if arguments are not valid or if no input
# files are provided. Compare 'cppcheck --help'.
cppcheck -f -q --inline-suppr --error-exitcode=2 --enable=all $define "$@"
