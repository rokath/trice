#!/usr/bin/env bash
set -euo pipefail

# Resolve this script's directory (works with symlinks as well)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"

failCount=0

# Directories are relative to this script's location
targets=(
  "F030_bare"
  "G0B1_bare"
  "L432_bare"
  "F030_inst"
  "G0B1_inst"
  "L432_inst"
)

for d in "${targets[@]}"; do
  echo "--------------------------------------------------------------------------------------------------------"
  echo "${d} with TRICE_OFF=0"

  (
    cd -- "${SCRIPT_DIR}/${d}"

    if [[ ! -x ./build.sh ]]; then
      echo "FAIL: ${d} (missing or non-executable build.sh)"
      exit 2
    fi

    ./build.sh
  ) || {
    rc=$?
    failCount=$((failCount + 1))
    echo "FAIL: ${d} (exit code ${rc})"
  }
done

if (( failCount != 0 )); then
  echo "${failCount} times FAIL"
  exit 1
fi
