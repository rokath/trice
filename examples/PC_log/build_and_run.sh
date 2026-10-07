#!/bin/sh
# SPDX-License-Identifier: MIT

# Usage: ./build_and_run.sh. Requires trice and cc or gcc in PATH, no hardware.
# This example formats logs inside the C program and prints them directly.
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

# 1. Prepare the IDs and the C table of message formats.
# bind supplies IDs without writing numeric IDs into the source files.
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

# 2. Collect library sources for the native PC compiler.
# A range such as [a-z] also matches uppercase letters in some locales. Exclude
# the embedded-only RTT transport explicitly: this host example needs neither
# SEGGER_RTT_Conf.h nor an installed SEGGER/J-Link tool.
# set -- starts an empty argument list. Each iteration appends one source path;
# quoting keeps paths intact. "$@" below passes that list to the compiler.
set --
for source_file in ../../src/*.c; do
  case "$source_file" in
    */SEGGER_RTT.c) continue ;;
  esac
  set -- "$@" "$source_file"
done
echo "INFO: PC_log uses local output; SEGGER RTT sources, SEGGER_RTT_Conf.h and J-Link tools are not required."

# 3. Compile the program, shared sample calls and generated format table.
"$compiler" -std=c11 -Wall -Wextra -Werror \
  -I. -Igenerated -I../../src -I../../src/default_conf \
  main.c ../../_test/testdata/triceCheck.c build/til.c "$@" \
  -o "build/pc_log${suffix}"

# 4. Run it. Unlike PC_features, this prints text without a separate decoder.
"./build/pc_log${suffix}"
