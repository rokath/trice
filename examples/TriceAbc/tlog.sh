#!/usr/bin/env bash
# Usage: ./tlog.sh after ./demo.sh has produced abc.bus.
# Requires tlog in PATH. Replay the binary bus once and decode it using the
# shared format/location tables. COBS framing matches the simulated nodes.
# typeX0 selects the display for variable-length payloads; it does not alter them.

cd "$(dirname "$0")" || exit 1

tlog -port FILEBUFFER -args abc.bus \
  -li ../../demoLI.json \
  -liMaxDirs 1 \
  -til ../../demoTIL.json \
  -d16 \
  -pf COBS \
  -typeX0="counted:typeX0 buffer: %v\n"
