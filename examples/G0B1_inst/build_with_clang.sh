#!/usr/bin/env bash
#
# Simple and CI-safe clang build script.
#
# Main idea:
# - Always run relative to the script location (NOT the caller's working directory).
# - Use strict error handling, so CI fails immediately and clearly.
#

set -euo pipefail

# ------------------------------------------------------------------------------
# 1) Resolve stable directories
# ------------------------------------------------------------------------------

# Absolute path of the directory containing this script
# - Works regardless of where the script is called from (CI often calls from repo root).
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Repo root is two levels above: <repo>/examples/<target>/build_with_clang.sh -> <repo>
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Ensure we build from the example directory (where the Makefile is expected).
cd "${SCRIPT_DIR}"

# ------------------------------------------------------------------------------
# 2) Run TRICE scripts (use absolute paths, so CWD does not matter)
# ------------------------------------------------------------------------------

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this first to trigger the used editor to show the Trice IDs cleaned state.

bash "${REPO_ROOT}/trice_insertIDs_in_examples_and_test_folder.sh"

# ------------------------------------------------------------------------------
# 3) Load build environment and build
# ------------------------------------------------------------------------------

# Source the environment file from repo root (absolute path).
# shellcheck disable=SC1090
source "${REPO_ROOT}/build_environment.sh"

# Optional: provide a default if build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

make ${MAKE_JOBS} clang

# ------------------------------------------------------------------------------
# 4) Post-build cleanup (leave repo in cleaned state)
# ------------------------------------------------------------------------------

bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh" \
  # Run this again to get the Trice IDs cleaned state.
