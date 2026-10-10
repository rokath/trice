#!/bin/sh
# Build the uninstrumented STM32F030 firmware; no flashing is performed.
set -e
cd "$(dirname "$0")"
make "$@"
