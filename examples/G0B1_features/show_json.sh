#!/bin/sh
# SPDX-License-Identifier: MIT
set -eu
cd "$(dirname "$0")"
trice log -p FILEBUFFER -args temp/trice.bin -pf none -d16 \
  -til til.json -li li.json -hs off -ts16 us -ts32 ms -ulabel sensor:450 -logFormat json "$@"
