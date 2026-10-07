#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start from the demo directory: ./demo_deferred.sh
# This example buffers Trice records, then writes them with TriceTransfer().

# Stop on an error instead of continuing with an old executable or log file.
set -e

# Use the native C compiler: usually cc on macOS/Linux, or gcc on Windows.
compiler="cc"
if ! command -v cc >/dev/null 2>&1; then
  compiler="gcc"
fi

# 1. Assign IDs and save the format strings in til.json.
# Bind also creates the compiler headers in generated/.
echo "Bind the Trice IDs"
trice bind

# 2. Compile the deferred example and the Trice library.
# -I adds a directory for header files; -o names the executable.
# trice*.c selects the library sources without the vendor SEGGER_RTT.c.
# TCOBS frames the records. cobsEncode.c is also needed by TriceEncode().
# Backslashes continue this single compiler command onto the following lines.
# The .exe filename works on Windows, Linux and macOS.
echo "Build the deferred demo"
mkdir -p deferred/build
"$compiler" \
  -Ideferred \
  -Igenerated \
  -I../src \
  deferred/main.c \
  ../src/trice*.c \
  ../src/cobsEncode.c \
  ../src/tcobsv1Encode.c \
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
