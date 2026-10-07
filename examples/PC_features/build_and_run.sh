#!/bin/sh
# SPDX-License-Identifier: MIT

# Run ./build_and_run.sh first, then ./show_text.sh, ./show_kv.sh or ./show_json.sh.
# Requires trice and a native C compiler in PATH; no hardware is needed.
# Stop on a failed command (-e) or an unset variable (-u).
set -eu
# Relative paths refer to this example, even when called from another directory.
cd "$(dirname "$0")"

# 1. Check the tools before generating or compiling anything.
command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put trice in PATH before building this example." >&2
  exit 1
}

compiler=cc
if ! command -v "$compiler" >/dev/null 2>&1; then
  compiler=gcc
fi
command -v "$compiler" >/dev/null 2>&1 || {
  echo "ERROR: A C compiler named cc or gcc is required." >&2
  exit 1
}

# Windows compilers produce .exe files; Unix compilers need no suffix.
suffix=
case "$(uname -s)" in MINGW* | MSYS* | CYGWIN*) suffix=.exe ;; esac

# 2. Generate IDs and compiler headers without changing the readable log calls.
# Each option below has one job:
# -src main.c:       Scan this file for Trice calls.
# -genDir generated: Write the compiler headers into generated/.
# -til til.json:     Store the IDs and message formats used by the decoder.
# -li li.json:       Store the source file and line number for each ID.
# -liRoot ../..:     Store source paths relative to the repository root.
# -IDMin 1000:       Use IDs starting at 1000 for newly assigned calls.
# -IDMax 1200:       Use no IDs above 1200 for newly assigned calls.
# -ce:              Add context to calls whose format contains the ctx: selector.
#
# Read the -ce value as three parts:
#   ctx:             selects which calls receive extra context;
#   ", cycle={cycle:%u}" appends text with a named unsigned field called cycle;
#   pc_sample_phase  supplies that field's value when the C program logs.
# For emit_sample(3300) with pc_sample_phase=7, the decoded message becomes
# "Supply 3300 mV, cycle=7\n" and JSON/KV also contains the named cycle field.
# The original call in main.c stays unchanged; generated headers add the context.
trice bind \
  -src main.c \
  -genDir generated \
  -til til.json \
  -li li.json \
  -liRoot ../.. \
  -IDMin 1000 \
  -IDMax 1200 \
  -ce 'ctx:", cycle={cycle:%u}", pc_sample_phase'

# 3. Compile main.c with the Trice library. -I adds header search directories;
# the warning flags help catch mistakes while experimenting with the example.
# Project headers take precedence; library defaults are searched last.
"$compiler" -std=c11 -Wall -Wextra -Werror \
  -I. -Igenerated -I../../src -I../../src/default_conf \
  main.c ../../src/trice.c ../../src/trice8.c ../../src/trice16.c ../../src/trice32.c \
  ../../src/trice64.c ../../src/triceStackBuffer.c \
  ../../src/cobsEncode.c ../../src/tcobsv1Encode.c -o "pc_features${suffix}"
# 4. Run the program. It writes encoded events to capture.bin for the show scripts.
"./pc_features${suffix}"
printf 'Created %s\n' capture.bin
