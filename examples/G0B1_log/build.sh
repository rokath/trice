#!/usr/bin/env bash
# SPDX-License-Identifier: MIT

set -euo pipefail

# Keep all relative paths stable when the script is invoked from elsewhere.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"
cd "${SCRIPT_DIR}"

command -v trice >/dev/null 2>&1 || {
  echo "ERROR: Put the trice executable in PATH first." >&2
  exit 1
}
command -v arm-none-eabi-gcc >/dev/null 2>&1 || {
  echo "ERROR: Put the Arm GNU toolchain in PATH first." >&2
  exit 1
}

# Reuse the repository's platform-specific parallelism selection. On Windows
# this deliberately chooses a bounded job count instead of GNU Make's bare -j.
# shellcheck source=../../scripts/_150_setup_build_environment.sh
source "${ROOT}/scripts/_150_setup_build_environment.sh"

# These files contain the live Trice producers. The shared triceCheck.c is the
# same corpus compiled by G0B1_inst and the PC target tests. bind assigns stable
# IDs, and logC emits exactly this target's entries for local formatting.
trice bind \
  -src Core/Src/main.c \
  -src Core/Src/stm32g0xx_it.c \
  -src ../../_test/testdata/triceCheck.c \
  -IDMin 1100 -IDMax 16383 -IDMethod upward

trice generate \
  -src Core/Src/main.c \
  -src Core/Src/stm32g0xx_it.c \
  -src ../../_test/testdata/triceCheck.c \
  -logC=build/til.c

# Generated dependency files keep unchanged objects valid between invocations.
# Source or generated-code changes are rebuilt by make as usual.
make ${MAKE_JOBS} gcc
