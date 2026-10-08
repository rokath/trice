#!/usr/bin/env bash
# Build the uninstrumented STM32L432 firmware; no flashing is performed.
# Usage: ./build.sh. Requires Bash, make and the Arm GNU toolchain.
# The shared setup selects the compiler and parallel make job count.
# Stop immediately if a build command fails or a required variable is missing.
set -euo pipefail

# 1. Work beside the Makefile, even when called from another directory.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

# 2. Load the shared toolchain setup into this shell.
# shellcheck disable=SC1090
source ../../scripts/_150_setup_build_environment.sh

# 3. Compile. MAKE_JOBS supplies make's parallel-job option, for example -j4.
make ${MAKE_JOBS}
