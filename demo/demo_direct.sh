#!/bin/sh
# SPDX-License-Identifier: MIT
#
# Start from the demo directory: ./demo_direct.sh
# This example writes each Trice record immediately to a binary file.

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

# 2. Compile the direct example and the Trice library.
# -I adds a directory for header files; -o names the executable.
# trice*.c selects the library sources without the vendor SEGGER_RTT.c.
# TCOBS frames the records. cobsEncode.c is also needed by TriceEncode().
# Backslashes continue this single compiler command onto the following lines.
# The .exe filename works on Windows, Linux and macOS.
echo "Build the direct demo"
mkdir -p direct/build
"$compiler" \
  -Idirect \
  -Igenerated \
  -I../src \
  direct/main.c \
  ../src/trice*.c \
  ../src/cobsEncode.c \
  ../src/tcobsv1Encode.c \
  -o direct/build/demo_direct.exe

# 3. Run in the build directory, where the program writes log.bin.
# Return to the demo directory afterwards, where til.json is stored.
echo "Run the direct demo"
cd direct/build
./demo_direct.exe
cd ../..

# 4. Read the binary file and recover the readable messages using til.json.
# FILEBUFFER selects a file as input instead of a serial port.
echo "Decode direct/build/log.bin"
trice log -p FILEBUFFER -args direct/build/log.bin
