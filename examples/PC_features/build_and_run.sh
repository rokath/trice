#!/bin/sh
# SPDX-License-Identifier: MIT

set -eu
cd "$(dirname "$0")"

command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put trice in PATH before building this example." >&2
  exit 1
}

compiler=cc
command -v "$compiler" >/dev/null 2>&1 || compiler=gcc
command -v "$compiler" >/dev/null 2>&1 || {
  echo "ERROR: A C compiler named cc or gcc is required." >&2
  exit 1
}

suffix=
case "$(uname -s)" in MINGW* | MSYS* | CYGWIN*) suffix=.exe ;; esac

# Binding owns the ID table and adapters; the source keeps readable Trice calls.
trice bind -src main.c -genDir generated -til til.json -li li.json \
  -liRoot ../.. -IDMin 1000 -IDMax 1200 \
  -ce 'ctx:", cycle={cycle:%u}", pc_sample_phase'

"$compiler" -std=c11 -Wall -Wextra -Werror -I. -Igenerated -I../../src \
  main.c ../../src/trice.c ../../src/trice8.c ../../src/trice16.c ../../src/trice32.c \
  ../../src/trice64.c ../../src/triceStackBuffer.c \
  ../../src/cobsEncode.c ../../src/tcobsv1Encode.c -o "pc_features${suffix}"
"./pc_features${suffix}"
printf 'Created %s\n' capture.bin
