#!/usr/bin/env bash
#
# Step 03: Builds the Trice tool.
#
# Direct invocation:
# - ./_testAll_03_BuildTriceTool.sh
#
# Log file:
# - ./temp/testAll/_testAll_03_BuildTriceTool.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  run_cmd "$TESTALL_ROOT/buildTriceTool.sh" || fail "buildTriceTool.sh failed"
}

main "$@"
