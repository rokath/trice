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

# 2. Use the native linker's spelling for removing unused functions and data.
# Details: ../../docs/TriceReferenceManual.md#trice-project-image-size-optimization
link_unused=-Wl,--gc-sections
case "$(uname -s)" in Darwin) link_unused=-Wl,-dead_strip ;; esac
echo "INFO: PC_log uses local output; no project RTT header or J-Link installation is needed."

# 3. Compile the program, shared sample calls and generated format table.
# ../../src/*.c includes the complete library; default_conf supplies RTT defaults.
"$compiler" -std=c11 -Wall -Wextra -Werror \
  -ffunction-sections -fdata-sections "$link_unused" \
  -I. -Igenerated -I../../src -I../../src/default_conf \
  main.c ../../_test/testdata/triceCheck.c build/til.c ../../src/*.c \
  -o "build/pc_log${suffix}"

# 4. Run it. Unlike PC_features, this prints text without a separate decoder.
"./build/pc_log${suffix}"
