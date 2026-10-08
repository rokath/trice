#!/bin/sh
# Build the native CSV sample producer used by LabPlotDemo.
# Usage: ./build.sh. Requires CMake and a native C compiler.
# CMake configures in build/, compiles, then installs the executable here.
# --config Release also selects the configuration on Windows multi-config builds.
# SPDX-License-Identifier: MIT
#
# Portable CMake build entry point.
#
# Linux and macOS provide a suitable shell by default. On Windows, run this
# script from Git Bash, MSYS2, Cygwin, or WSL.

set -eu

# Stabilize the working directory so the script also works when invoked through
# a relative or absolute path from somewhere else.
script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)

cd "$script_dir"

# CMAKE_BUILD_TYPE is used by single-configuration generators. --config selects
# Release for multi-configuration generators such as Visual Studio.
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cmake --install build --config Release --prefix "$script_dir"

echo "Build completed in: $script_dir/build"
echo "Executable installed in: $script_dir/bin"
