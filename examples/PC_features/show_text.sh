#!/bin/sh
# SPDX-License-Identifier: MIT
# First run ./build_and_run.sh to create capture.bin, til.json and li.json.
# Usage: ./show_text.sh           or: ./show_text.sh -logLevel wrn
set -eu
cd "$(dirname "$0")"

# FILEBUFFER replays the capture once and exits. The decoder options must match
# main.c: TCOBSv1 framing and 16-bit default values. The tables supply log formats
# and source locations. Host timestamps are hidden; target stamps show phase
# (16-bit) and milliseconds (32-bit), including the interval between 32-bit stamps.
# sensor is an application tag with severity 450. "$@" forwards optional CLI
# arguments, so you can experiment with filtering without editing this script.
trice log \
  -p FILEBUFFER -args capture.bin -pf TCOBSv1 -d16 \
  -til til.json -li li.json -hs off \
  -ts16 'phase:%6d' -ts32 ms -ts32delta ms \
  -ulabel sensor:450 -logFormat text "$@"
