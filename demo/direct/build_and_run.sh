#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start this file from inside demo/direct. Keeping that rule explicit makes all
# paths in the shared build script short and easy to follow.

set -eu

if [ ! -f "./main.c" ] || [ ! -f "./triceConfig.h" ]; then
  echo "ERROR: Start this script from inside demo/direct:" >&2
  echo "  cd demo/direct" >&2
  echo "  ./build_and_run.sh" >&2
  exit 1
fi

# The shared helper is one directory above this direct demo.
../build_demo.sh
