#!/bin/sh
# SPDX-License-Identifier: MIT
# First run ./build_and_run.sh to create the capture and its decoding tables.
# Usage: ./show_kv.sh             or: ./show_kv.sh -logLevel wrn
set -eu
cd "$(dirname "$0")"

# Replay the file once with the encoding used by main.c. The tables supply
# formats and locations. Target stamps represent phase (16-bit) or milliseconds
# (32-bit); host timestamps are hidden. KV prints readable key=value pairs,
# including named fields. sensor has severity 450. "$@" forwards your CLI options.
trice log \
  -p FILEBUFFER -args capture.bin -pf TCOBSv1 -d16 \
  -til til.json -li li.json -hs off \
  -ts16 'phase:%d' -ts32 ms -ts32delta ms \
  -ulabel sensor:450 -logFormat kv "$@"
