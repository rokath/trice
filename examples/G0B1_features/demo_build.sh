#!/bin/sh
# SPDX-License-Identifier: MIT
set -eu
cd "$(dirname "$0")"

command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put trice in PATH before building this example." >&2
  exit 1
}
command -v arm-none-eabi-gcc >/dev/null 2>&1 || {
  echo "ERROR: Put the Arm GNU toolchain in PATH before building." >&2
  exit 1
}

# Bind only this project and its shared example-data producers into a private TIL.
# The selected call executes in both tasks, but its CE expression is evaluated
# only where the generated adapter is called, with the current FreeRTOS task.
trice bind -src Core/Src -src ../exampleData \
  -exclude Core/Inc/triceCustomAliases.h \
  -alias CUSTOM_PRINT -salias CUSTOM_ASSERT -alias printi -salias prints \
  -genDir generated -til til.json -li li.json -liRoot ../.. \
  -IDMin 13000 -IDMax 16383 \
  -ce 'ctx:", task={task:%p}", osThreadGetId()'

# GNU Make compiles from this directory, so a relative include path is enough.
path_separator=:
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*) path_separator=';' ;;
esac
C_INCLUDE_PATH="generated${C_INCLUDE_PATH:+${path_separator}${C_INCLUDE_PATH}}" \
  make gcc
printf 'Built %s\n' 'out.gcc/G0B1.elf'
