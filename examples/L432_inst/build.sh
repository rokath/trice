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

# Repo root is two levels above: <repo>/examples/<target>/build.sh -> <repo>
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Ensure we build from the example directory (where the Makefile is expected).
cd "${SCRIPT_DIR}"

# ------------------------------------------------------------------------------
# 2) Build TRICE_FLAGS (same logic as the original, but safe and predictable)
# ------------------------------------------------------------------------------

# Initialize an empty string
flags=""

# Loop through all arguments
for arg in "$@"; do
  flags="${flags}-D${arg} "
done

# ------------------------------------------------------------------------------
# 3) Run TRICE scripts (use absolute paths, so CWD does not matter)
# ------------------------------------------------------------------------------

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this first to trigger the used editor to show the Trice IDs cleaned state.

bash "${REPO_ROOT}/trice_insertIDs_in_examples_and_test_folder.sh"

# ------------------------------------------------------------------------------
# 4) Load build environment and build
# ------------------------------------------------------------------------------

# Source the environment file from repo root (absolute path).
# shellcheck disable=SC1090
source "${REPO_ROOT}/build_environment.sh"

# Optional: provide a default if build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc

# Preserve make's exit code exactly (your original behavior).
EXITCODE=$?

# ------------------------------------------------------------------------------
# 5) Post-build cleanup and exit
# ------------------------------------------------------------------------------

bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this again to get the Trice IDs cleaned state.

exit "${EXITCODE}"
