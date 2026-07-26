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
script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH='' cd -- "$script_dir/../.." && pwd)

# Track whether this build inserted generated Trice IDs. The EXIT trap only
# cleans the repository after a successful insert, including failed builds or
# interrupted builds.
ids_inserted=0

cleanup_after_build() {
  status=$1
  trap - EXIT INT TERM

  if [ "$ids_inserted" -eq 1 ]; then
    echo "cleanup: running trice clean"
    if ! bash "$repo_root/trice_cleanIDs_in_examples_and_test_folder.sh"; then
      if [ "$status" -eq 0 ]; then
        status=1
      fi
    fi
  fi

  exit "$status"
}

trap 'cleanup_after_build "$?"' EXIT
trap 'cleanup_after_build 130' INT
trap 'cleanup_after_build 143' TERM

# Start from a clean repository state before generating IDs for this build.
bash "$repo_root/trice_cleanIDs_in_examples_and_test_folder.sh"
bash "$repo_root/trice_insertIDs_in_examples_and_test_folder.sh"
ids_inserted=1

cd "$script_dir"

# CMAKE_BUILD_TYPE is used by single-configuration generators. --config selects
# Release for multi-configuration generators such as Visual Studio.
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

echo "Build completed in: $script_dir/build"
