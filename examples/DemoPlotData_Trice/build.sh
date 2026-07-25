#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Portable CMake build entry point.
#
# Linux and macOS provide a suitable shell by default. On Windows, run this
# script from Git Bash, MSYS2, Cygwin, or WSL.

set -eu

# Stabilize the working directory so the ../../src relationship remains valid
# regardless of the directory from which this script was invoked.
script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$script_dir"

# CMAKE_BUILD_TYPE is used by single-configuration generators. --config selects
# Release for multi-configuration generators such as Visual Studio.
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

echo "Build completed in: $script_dir/build"
