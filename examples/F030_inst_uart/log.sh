#!/bin/sh
# Decode UART records; select another port with tlog's -p switch.
set -e
cd "$(dirname "$0")"
tlog -p COM7 -baud 115200 "$@"
