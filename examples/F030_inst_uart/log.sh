#!/bin/sh
# The first argument is the serial port; remaining arguments go to tlog.
set -e
cd "$(dirname "$0")"
port=${1:-COM7}
if [ "$#" -gt 0 ]; then
  shift
fi
tlog -p "$port" -baud 115200 "$@"
