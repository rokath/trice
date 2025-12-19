#!/usr/bin/env bash
#
# Simple and CI-safe build script (pattern-preserving version).
#
# Main idea:
# - Always run relative to the script location (NOT the caller's working directory).
# - Use strict error handling, so CI fails immediately and clearly.
#

set -euo pipefail

# ------------------------------------------------------------------------------
# 1) Resolve stable directories
# ------------------------------------------------------------------------------

# Absolute path of the directory containing this build.sh
# - Works regardless of where the script is called from (CI often calls from repo root).
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Repo root is two levels above: <repo>/examples/<target>/build.sh
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Ensure we build from the example directory (where the Makefile is expected).
cd "${SCRIPT_DIR}"

# ------------------------------------------------------------------------------
# 2) Build TRICE_FLAGS and detect TRICE_OFF
# ------------------------------------------------------------------------------

# Initialize an empty string
flags=""

# Track TRICE_OFF=1 explicitly (used later for conditional insert/build)
triceOFF="0"

# Loop through all arguments
for arg in "$@"; do
  if [ "${arg}" = "TRICE_OFF=1" ]; then
    triceOFF="1"
    export triceOFF="1"   # keep compatibility with existing scripts/env if needed
  fi
  flags="${flags}-D${arg} "
done

# ------------------------------------------------------------------------------
# 3) Run TRICE scripts (use absolute paths, so CWD does not matter)
# ------------------------------------------------------------------------------

# Trice is called here and not within make, to guarantee, it is finished
# before any other job starts.
bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this first to trigger the used editor to show the Trice IDs cleaned state.

if [ "${triceOFF}" != "1" ]; then
  bash "${REPO_ROOT}/trice_insertIDs_in_examples_and_test_folder.sh" \
    # custom aliases should be excluded or without IDs, when translating with TRICE_OFF=1
fi

# ------------------------------------------------------------------------------
# 4) Load build environment and build
# ------------------------------------------------------------------------------

# Source the environment file from repo root (absolute path).
# shellcheck disable=SC1090
source "${REPO_ROOT}/build_environment.sh"

# Optional: provide a default if build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

# We translate here with inserted IDs, even in the triceOff case and expect no warnings.
make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc

bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this again to get the Trice IDs cleaned state.

# ------------------------------------------------------------------------------
# 5) Additional triceOff verification build (without IDs)
# ------------------------------------------------------------------------------

# Additionally we again translate the triceOff case without IDs and expect no warnings.
if [ "${triceOFF}" = "1" ]; then
  make clean
  make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc
fi
