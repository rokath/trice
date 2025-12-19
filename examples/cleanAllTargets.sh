#!/usr/bin/env bash
#
# Simple and CI-safe "clean all targets" script.
#
# Main idea:
# - Always run relative to the script location (NOT the caller's working directory).
# - Keep the original structure for readability.
#

set -euo pipefail

# ------------------------------------------------------------------------------
# 1) Resolve stable directory (examples/)
# ------------------------------------------------------------------------------

# Absolute path of the directory containing this script (expected: <repo>/examples)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

failCount=0

# ------------------------------------------------------------------------------
# 2) Clean each target directory
# ------------------------------------------------------------------------------

for d in ./F030_bare/ ./G0B1_bare/ ./L432_bare/ ./F030_inst/ ./G0B1_inst/ ./L432_inst/; do
  (
    cd "${SCRIPT_DIR}/${d}"
    make clean
  )

  # If make clean failed, count it and continue (same intent as original).
  if [ $? -ne 0 ]; then
    failCount=$((failCount + 1))
    echo "FAIL: ${d}"
  fi
done

# ------------------------------------------------------------------------------
# 3) Summary / exit code
# ------------------------------------------------------------------------------

if [ "${failCount}" -ne 0 ]; then
  echo "${failCount} times FAIL"
  exit 1
fi
