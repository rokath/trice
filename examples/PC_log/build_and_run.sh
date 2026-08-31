#!/bin/sh
# SPDX-License-Identifier: MIT

set -eu

# Run every command relative to this example, independent of the caller's
# current directory.
cd "$(dirname "$0")"

command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put the trice executable in PATH first." >&2
  exit 1
}

compiler="cc"
if ! command -v "$compiler" >/dev/null 2>&1; then
  compiler="gcc"
fi
command -v "$compiler" >/dev/null 2>&1 || {
  echo "ERROR: Install a C compiler named cc or gcc." >&2
  exit 1
}

# bind supplies stable IDs without writing numeric IDs into the source files.
# The shared triceCheck.c is the same producer corpus used by the PC target
# tests and the installed MCU examples. logC keeps only these two sources in
# the target-side format table.
trice bind \
  -src main.c \
  -src ../../_test/testdata/triceCheck.c \
  -IDMin 1000 -IDMax 16383 -IDMethod upward
trice generate \
  -src main.c \
  -src ../../_test/testdata/triceCheck.c \
  -logC=build/til.c

suffix=""
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) suffix=".exe" ;;
esac

"$compiler" -std=c11 -Wall -Wextra -Werror \
  -I. -Ibuild/triceIDs -I../../src \
  main.c ../../_test/testdata/triceCheck.c build/til.c ../../src/[a-z]*.c \
  -o "build/pc_log${suffix}"

"./build/pc_log${suffix}"
