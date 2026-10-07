#!/bin/sh
# SPDX-License-Identifier: MIT
# First run ./build_and_run.sh to create the capture and its decoding tables.
# Usage: ./show_json.sh           or: ./show_json.sh -logLevel wrn
set -eu
cd "$(dirname "$0")"

# Replay the file once using the framing and value width configured in main.c.
# til.json supplies formats; li.json supplies source locations. Target stamps
# represent phase (16-bit) or milliseconds (32-bit); host timestamps are hidden.
# JSON means one complete JSON event per line (NDJSON). sensor has severity 450;
# extra arguments in "$@" let you try other severities or log filters.
trice log \
  -p FILEBUFFER -args capture.bin -pf TCOBSv1 -d16 \
  -til til.json -li li.json -hs off \
  -ts16 'phase:%d' -ts32 ms -ts32delta ms \
  -ulabel sensor:450 -logFormat json "$@"
