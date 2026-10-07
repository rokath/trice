#!/bin/sh
# Replay a saved board capture as key=value pairs.
# Usage: ./show_kv.sh; optional filters follow, e.g. -logLevel wrn.
# First build/flash the feature tour and save its RTT bytes as temp/trice.bin.
# The build alone does not create a capture. til.json and li.json must match it.
# FILEBUFFER reads once and exits; -pf none matches the unframed RTT output.
# 16-bit target stamps use microseconds, 32-bit stamps use milliseconds.
# Host timestamps are hidden; sensor has severity 450. "$@" forwards your options.
# SPDX-License-Identifier: MIT
set -eu
cd "$(dirname "$0")"
trice log \
  -p FILEBUFFER -args temp/trice.bin -pf none -d16 \
  -til til.json -li li.json -hs off \
  -ts16 us -ts32 ms \
  -ulabel sensor:450 -logFormat kv "$@"
