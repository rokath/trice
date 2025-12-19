#!/usr/bin/env bash
#
# Simple and CI-safe build script.
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

# ------------------------------------------------------------------------------
# 2) Build TRICE_FLAGS (same logic as your original, but safe and predictable)
# ------------------------------------------------------------------------------

flags=""
for arg in "$@"; do
  flags="${flags}-D${arg} "
done

# ------------------------------------------------------------------------------
# 3) Run TRICE scripts (use absolute paths, so CWD does not matter)
# ------------------------------------------------------------------------------

# If these scripts are not executable, call them via bash (CI checkout can be strict).
bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh"
bash "${REPO_ROOT}/trice_insertIDs_in_examples_and_test_folder.sh"

# ------------------------------------------------------------------------------
# 4) Load build environment and build
# ------------------------------------------------------------------------------

# Source the environment file from repo root (absolute path).
# shellcheck disable=SC1090
source "${REPO_ROOT}/build_environment.sh"

# Optional: provide a default if build_environment.sh does not set MAKE_JOBS.
# : "${MAKE_JOBS:=-j2}"

# Ensure we build from the example directory (where the Makefile is expected).
cd "${SCRIPT_DIR}"

make ${MAKE_JOBS} TRICE_FLAGS="${flags}"

# ------------------------------------------------------------------------------
# 5) Post-build cleanup (leave repository in cleaned state)
# ------------------------------------------------------------------------------

bash "${REPO_ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh"
