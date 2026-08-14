#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start this file from inside demo/deferred. Keeping that rule explicit makes
# all paths in the shared build script short and easy to follow.

set -eu

if [ ! -f "./main.c" ] || [ ! -f "./triceConfig.h" ]; then
  echo "ERROR: Start this script from inside demo/deferred:" >&2
  echo "  cd demo/deferred" >&2
  echo "  ./build_and_run.sh" >&2
  exit 1
fi

# The shared helper is one directory above this deferred demo.
../build_demo.sh
