#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start from the demo directory: ./demo_deferred.sh
# This example buffers Trice records, then writes them with TriceTransfer().

# Stop on an error instead of continuing with an old executable or log file.
set -e
# Keep all paths relative to this demo, even when started from elsewhere.
cd "$(dirname "$0")"

# Use the native C compiler: usually cc on macOS/Linux, or gcc on Windows.
compiler="cc"
if ! command -v cc >/dev/null 2>&1; then
  compiler="gcc"
fi

# Use the native linker's spelling for removing unused functions and data.
# Details: ../docs/TriceReferenceManual.md#trice-project-image-size-optimization
link_unused=-Wl,--gc-sections
case "$(uname -s)" in Darwin) link_unused=-Wl,-dead_strip ;; esac

# 1. Assign IDs and save the format strings in til.json.
# Bind also creates the compiler headers in generated/.
echo "Bind the Trice IDs"
trice bind

# 2. Compile the deferred example and the Trice library.
# -I adds a directory for header files; -o names the executable.
# ../src/*.c adds the complete library; no source list needs maintaining.
# Project headers come first; default_conf supplies the fallback RTT header.
# Backslashes continue this single compiler command onto the following lines.
# The .exe filename works on Windows, Linux and macOS.
echo "Build the deferred demo"
mkdir -p deferred/build
"$compiler" \
  -ffunction-sections -fdata-sections "$link_unused" \
  -Ideferred \
  -Igenerated \
  -I../src \
  -I../src/default_conf \
  deferred/main.c \
  ../src/*.c \
  -o deferred/build/demo_deferred.exe

# 3. Run in the build directory, where the program writes log.bin.
# Return to the demo directory afterwards, where til.json is stored.
echo "Run the deferred demo"
cd deferred/build
./demo_deferred.exe
cd ../..

# 4. Read the binary file and recover the readable messages using til.json.
# FILEBUFFER selects a file as input instead of a serial port.
echo "Decode deferred/build/log.bin"
trice log -p FILEBUFFER -args deferred/build/log.bin
