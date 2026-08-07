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

# Accept the repository build-matrix option explicitly and reject misspellings
# instead of silently producing a binary in the wrong Trice mode.
trice_off=0
for option; do
  case "$option" in
    TRICE_OFF=0) trice_off=0 ;;
    TRICE_OFF=1) trice_off=1 ;;
    *)
      echo "Unsupported build option: $option (expected TRICE_OFF=0 or TRICE_OFF=1)" >&2
      exit 2
      ;;
  esac
done

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

# An outer test wrapper owns Insert/Bind preparation and exact restoration. A
# direct user invocation keeps the historical local Insert/Clean lifecycle.
if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" = "1" ]; then
  echo "Trice ID workflow owned by outer wrapper: ${TRICE_ID_WORKFLOW:-unknown}"
else
  bash "$repo_root/trice_cleanIDs_in_examples_and_test_folder.sh"
  bash "$repo_root/trice_insertIDs_in_examples_and_test_folder.sh"
  ids_inserted=1
fi

cd "$script_dir"

# CMAKE_BUILD_TYPE is used by single-configuration generators. --config selects
# Release for multi-configuration generators such as Visual Studio.
cmake -S . -B build -DTRICE_OFF="$trice_off"
cmake --build build --config Release
cmake --install build --config Release --prefix "$script_dir"

echo "Build completed in: $script_dir/build"
echo "Executable installed in: $script_dir/bin"
