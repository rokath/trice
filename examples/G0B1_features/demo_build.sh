#!/bin/sh
# Build the FreeRTOS feature tour; this does not flash or run the board.
# Usage: ./demo_build.sh [gcc|clang]. Requires Trice, make and Arm GNU libraries.
# First bind generates this project's tables and headers, then make compiles.
# Calls marked ctx: gain the current task handle through osThreadGetId().
# Library/image details: ../../docs/rm/code_size.md
# SPDX-License-Identifier: MIT
set -eu
cd "$(dirname "$0")"

# Both compiler entry points share the same project-local Bind preparation.
build_target=${1:-gcc}
case "$build_target" in
  gcc | clang) ;;
  *)
    echo "ERROR: Expected gcc or clang, got '$build_target'." >&2
    exit 1
    ;;
esac
if [ "$build_target" = clang ]; then
  command -v clang >/dev/null 2>&1 || {
    echo "ERROR: Put Clang in PATH before building this example." >&2
    exit 1
  }
fi

# 1. Check the tools before generating the decoding tables and compiler headers.
command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put trice in PATH before building this example." >&2
  exit 1
}
command -v arm-none-eabi-gcc >/dev/null 2>&1 || {
  echo "ERROR: Put the Arm GNU toolchain in PATH before building." >&2
  exit 1
}

# 2. Bind this project and its shared example-data producers into a private TIL.
# -alias/-salias describe the project's custom Trice wrappers; -exclude prevents
# their definitions from being mistaken for actual calls. -liRoot stores paths
# relative to the repository so the location table works on another computer.
# The selected call executes in both tasks, but its CE expression is evaluated
# only where the generated adapter is called, with the current FreeRTOS task.
# Retain local ID ownership evidence; initialize LI only on the first build.
[ -e li.json ] || touch li.json
trice bind -src Core/Src -src ../exampleData \
  -exclude Core/Inc/triceCustomAliases.h \
  -alias CUSTOM_PRINT -salias CUSTOM_ASSERT -alias printi -salias prints \
  -genDir generated -til til.json -li li.json -liRoot ../.. \
  -IDMin 13000 -IDMax 16383 \
  -ce 'ctx:", task={task:%p}", osThreadGetId()'

# 3. Compile with the generated headers on the include search path.
# Keep any existing include directories. Native Windows GCC separates these
# paths with semicolons; Unix GCC uses colons.
path_separator=:
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) path_separator=';' ;;
esac
include_path=generated
if [ -n "${C_INCLUDE_PATH:-}" ]; then
  include_path="${include_path}${path_separator}${C_INCLUDE_PATH}"
fi
# This environment assignment applies only to make and the compilers it starts.
C_INCLUDE_PATH="$include_path" make "$build_target"
printf 'Built %s\n' "out.${build_target}/G0B1.elf"
