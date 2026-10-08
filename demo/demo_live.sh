#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start from the demo directory: ./demo_live.sh
# Leave this terminal running and decode the capture in a second terminal.

# Stop on a failed build instead of starting an old executable.
set -e
# Keep all paths relative to this demo, even when started from elsewhere.
cd "$(dirname "$0")"

# Use the native C compiler available on this computer.
compiler="cc"
if ! command -v cc >/dev/null 2>&1; then
  compiler="gcc"
fi

# Use the native linker's spelling for removing unused functions and data.
# Details: ../docs/TriceReferenceManual.md#trice-project-image-size-optimization
link_unused=-Wl,--gc-sections
case "$(uname -s)" in Darwin) link_unused=-Wl,-dead_strip ;; esac

# 1. Assign IDs and create the headers and til.json used by both processes.
echo "Bind the Trice IDs"
trice bind

# 2. Build the live application using the existing deferred configuration.
# -I adds header directories; -o names the executable.
# ../src/*.c adds the complete library; default_conf supplies its RTT fallback.
echo "Build the live demo"
mkdir -p live/build
"$compiler" \
  -ffunction-sections -fdata-sections "$link_unused" \
  -Ideferred \
  -Igenerated \
  -I../src \
  -I../src/default_conf \
  live/main.c \
  ../src/*.c \
  -o live/build/demo_live.exe

# 3. The application writes and flushes one record per second to log.bin.
echo "In a second terminal, enter the demo directory and run:"
echo "  trice log -p FILE -args live/build/log.bin"
echo "Stop each terminal with Ctrl+C. Stop both before restarting the demo."
cd live/build
./demo_live.exe
