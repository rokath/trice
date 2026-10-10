#!/bin/sh
# Generate Trice sidecars and build the firmware; no flashing is performed.
set -e
cd "$(dirname "$0")"
touch li.json
trice bind -src Core -genDir out/sidecars
make "$@"
