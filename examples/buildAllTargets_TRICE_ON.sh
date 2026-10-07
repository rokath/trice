#!/usr/bin/env bash
# Repository-wide example build check; not needed to try one example.
# Usage: ./buildAllTargets_TRICE_ON.sh. Requires Bash and each target's build tools.
# Prepare shared Bind data once, then run the listed build.sh scripts in order.
# Each build runs in a subshell so its directory changes do not affect the next.
# Continue after a failed target and report the total failures at the end.
set -euo pipefail

# Resolve this script's directory (works with symlinks as well)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd -P)"

# shellcheck source=./prepareTriceBind.sh
source "$SCRIPT_DIR/prepareTriceBind.sh"
prepare_trice_bind_build "$REPO_ROOT"

failCount=0

# Directories are relative to this script's location
targets=(
  "DemoData_CSV"
  "DemoData_Trice"
  "F030_bare"
  "G0B1_bare"
  "L432_bare"
  "F030_inst"
  "G0B1_inst"
  "L432_inst"
  "DemoData_CSV"
  "DemoData_Trice"
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

if ((failCount != 0)); then
  echo "${failCount} times FAIL"
  exit 1
fi
