#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start this script from the demo directory:
#   cd demo
#   ./demo.sh

# Optional safeguards; uncomment them for explicit preflight diagnostics.
# set -eu
# [ -f "../src/trice.h" ] || { echo "ERROR: Start this script inside demo." >&2; exit 1; }
# command -v trice >/dev/null 2>&1 || { echo "ERROR: Put trice in PATH." >&2; exit 1; }

# Linux and macOS commonly provide cc, while Windows often provides gcc.
compiler="cc"
if ! command -v "$compiler" >/dev/null 2>&1; then
  compiler="gcc"
fi
# command -v "$compiler" >/dev/null 2>&1 || { echo "ERROR: Install cc or gcc." >&2; exit 1; }

suffix=""
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) suffix=".exe" ;;
esac

echo "Bind the Trice IDs"
trice bind

for mode in deferred direct; do
  echo
  echo "Build and run $mode demo"
  mkdir -p "$mode/build"

  app="demo_${mode}${suffix}"
  "$compiler" \
    -I"$mode" \
    -Ibuild/triceIDs \
    -I../src \
    "$mode/main.c" \
    ../src/[a-z]*.c \
    -o "$mode/build/$app"

  # Run inside build so the application writes its log.bin there.
  (
    cd "$mode/build"
    "./$app"
  )
done

for mode in deferred direct; do
  echo
  echo "Decode $mode/build/log.bin"
  trice log -p FILEBUFFER -args "$mode/build/log.bin"
done
