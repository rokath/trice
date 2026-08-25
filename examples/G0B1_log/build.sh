#!/bin/sh
# SPDX-License-Identifier: MIT

set -eu

# Keep all relative paths stable when the script is invoked from elsewhere.
cd "$(dirname "$0")"

command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put the trice executable in PATH first." >&2
  exit 1
}
command -v arm-none-eabi-gcc >/dev/null 2>&1 || {
  echo "ERROR: Put the Arm GNU toolchain in PATH first." >&2
  exit 1
}

# Both files contain live Trice producers. bind assigns their stable IDs and
# logC emits exactly those five entries for target-side formatting.
trice bind \
  -src Core/Src/main.c \
  -src Core/Src/stm32g0xx_it.c \
  -IDMin 1100 -IDMax 1199 -IDMethod upward

trice generate \
  -src Core/Src/main.c \
  -src Core/Src/stm32g0xx_it.c \
  -logC=build/til.c

make clean
make gcc
