#!/bin/sh
# Decode RTT records; start this before resetting the flashed board.
set -e
cd "$(dirname "$0")"
tlog -p J-LINK -args "-Device STM32F030R8" -pf none -d16 "$@"
