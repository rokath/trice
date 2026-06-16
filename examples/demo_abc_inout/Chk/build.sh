#!/usr/bin/env bash
set -euo pipefail

# build.sh
#
# Build the Chk example program against the standalone Lib directory.
# This covers Linux, macOS, and Windows environments that provide a Unix-like
# shell and C compiler, for example WSL, MSYS2, MinGW Git Bash, or Cygwin.
#
# Override the compiler or flags if needed:
#
#   CC=clang CFLAGS="-O0 -g" ./build.sh
#   CFLAGS="-DDEMO_ABC_READ_USES_LOCK=1" ./build.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

CC_BIN="${CC:-cc}"
EXE="demoAbcDevice"
BASE_CFLAGS=(-std=c99 -Wall -Wextra -pedantic -O2 -I../Lib)

case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW*|MSYS*|CYGWIN*)
    EXE="demoAbcDevice.exe"
    BASE_CFLAGS+=(-D_CRT_SECURE_NO_WARNINGS)
    ;;
  Darwin*|Linux*)
    ;;
  *)
    echo "warning: unknown OS; trying generic POSIX-like build" >&2
    ;;
esac

mkdir -p build

# shellcheck disable=SC2086
"$CC_BIN" "${BASE_CFLAGS[@]}" ${CFLAGS:-} \
  -o "build/$EXE" \
  main.c ../Lib/demoAbcInOut.c

echo "built Chk/build/$EXE"
