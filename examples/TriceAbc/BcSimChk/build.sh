#!/usr/bin/env bash
set -euo pipefail

# build.sh
#
# Build the BcSimChk example program against the standalone BcSim directory.
#
# The script is intentionally small and shell-only so it works in common local
# developer environments: Linux shells, macOS shells, WSL, MSYS2/MinGW, Git Bash
# with a compiler in PATH, and Cygwin. Git Bash alone does not guarantee that a
# compiler exists; the script checks this explicitly and prints a useful error.
#
# Override the compiler or flags if needed:
#
#   CC=clang CFLAGS="-O0 -g" ./build.sh
#   CFLAGS="-DBCSIM_READ_USES_LOCK=1" ./build.sh

# Move to the directory containing this script so relative paths below work
# regardless of the caller's current working directory.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Find a C compiler. Prefer an explicitly supplied CC. Otherwise try common
# command names in an order that works well on Windows/Git Bash, Linux, macOS,
# and CI images.
find_compiler() {
  if [ "${CC:-}" != "" ]; then
    if command -v "$CC" >/dev/null 2>&1; then
      printf '%s\n' "$CC"
      return 0
    fi
    echo "error: CC is set to '$CC', but that command was not found." >&2
    return 1
  fi

  for c in gcc clang cc; do
    if command -v "$c" >/dev/null 2>&1; then
      printf '%s\n' "$c"
      return 0
    fi
  done

  echo "error: no C compiler found in PATH." >&2
  echo "Git Bash provides a Unix-like shell, but not necessarily gcc/clang." >&2
  echo "Install MSYS2/MinGW gcc, clang, or run with CC=/path/to/compiler." >&2
  return 1
}

CC_BIN="$(find_compiler)"
EXE="bcSimDevice"

# Baseline flags: C99, useful warnings, optimized demo build, and include path
# to the protocol-neutral BcSim library directory.
BASE_CFLAGS=(-std=c99 -Wall -Wextra -pedantic -O2 -I../BcSim)

# Pick the executable suffix and any platform-specific compatibility defines.
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW*|MSYS*|CYGWIN*)
    EXE="bcSimDevice.exe"
    BASE_CFLAGS+=(-D_CRT_SECURE_NO_WARNINGS)
    ;;
  Darwin*|Linux*)
    ;;
  *)
    echo "warning: unknown OS; trying generic POSIX-like build" >&2
    ;;
esac

# Keep the binary out of the source directory.
mkdir -p build

# CFLAGS is intentionally expanded as shell words so callers can pass multiple
# compiler options. This is a small developer script, not a general build system.
# shellcheck disable=SC2086
"$CC_BIN" "${BASE_CFLAGS[@]}" ${CFLAGS:-} \
  -o "build/$EXE" \
  main.c ../BcSim/BcSim.c

echo "built BcSimChk/build/$EXE using $CC_BIN"
