#!/bin/sh
# SPDX-License-Identifier: MIT
set -eu
cd "$(dirname "$0")"
trice log -p FILEBUFFER -args capture.bin -pf TCOBSv1 -d16 -til til.json -li li.json \
  -hs off -ts16 'phase:%d' -ts32 ms -ts32delta ms -ulabel sensor:450 -logFormat json "$@"
