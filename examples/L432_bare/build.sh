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

# Repo root is two levels above: <repo>/examples/<target>/build.sh -> <repo>
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Ensure we build from the example directory (where the Makefile is expected).
cd "${SCRIPT_DIR}"

# ------------------------------------------------------------------------------
# 2) Load build environment and build
# ------------------------------------------------------------------------------

# Source the environment file from repo root (absolute path).
# shellcheck disable=SC1090
source "${REPO_ROOT}/build_environment.sh"

# Optional: provide a default if build_environment.sh does not set MAKE_JOBS.
# : "${MAKE_JOBS:=-j2}"

make ${MAKE_JOBS}
