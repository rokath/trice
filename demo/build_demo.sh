#!/bin/sh
# SPDX-License-Identifier: MIT
#
# This helper is called from direct/build_and_run.sh or
# deferred/build_and_run.sh. All paths are relative to that demo directory.

set -eu

# Catch the most common mistake: starting outside direct or deferred.
if [ ! -f "main.c" ] || [ ! -f "triceConfig.h" ] || [ ! -f "../../src/trice.h" ]; then
  echo "ERROR: This build must be started inside a demo directory." >&2
  echo "Example:" >&2
  echo "  cd demo/direct" >&2
  echo "  ./build_and_run.sh" >&2
  exit 1
fi

# The installed PC tool is intentionally not built by this demo. Requiring it
# in PATH keeps the example independent of the Trice tool's Go source tree.
if ! command -v trice >/dev/null 2>&1; then
  echo "ERROR: The 'trice' command was not found." >&2
  echo "Install the Trice tool and add its directory to PATH." >&2
  exit 1
fi

# Use the conventional C compiler command available on Linux, macOS, and
# common Windows POSIX environments such as Git Bash or MSYS2.
if ! command -v cc >/dev/null 2>&1; then
  echo "ERROR: The C compiler command 'cc' was not found." >&2
  echo "Install a C compiler and add its directory to PATH." >&2
  exit 1
fi

echo "1/4  Bind the Trice IDs"
trice bind \
  -src "main.c" \
  -til "../til.json" \
  -li "../li.json" \
  -liRoot ".."

# Name the executable after the selected demo directory.
app="demo_$(basename "$PWD")"
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) app="$app.exe" ;;
esac

echo "2/4  Compile $app"
# Compile the demo together with all C files in the Trice target source folder.
cc \
  -I. \
  -I.. \
  -Ibuild/triceIDs \
  -I../../src \
  main.c \
  ../../src/*.c \
  -o "build/$app"

echo "3/4  Run $app"
# Running from inside build makes the relative filename "log.bin" land beside
# the executable instead of cluttering the demo's source directory.
(
  cd "build"
  "./$app"
)

if [ ! -s "build/log.bin" ]; then
  echo "ERROR: The application did not create a non-empty build/log.bin." >&2
  exit 1
fi

echo "4/4  Decode build/log.bin with trice log"
trice log \
  -p FILEBUFFER \
  -args "build/log.bin" \
  -til "../til.json" \
  -li "../li.json"
