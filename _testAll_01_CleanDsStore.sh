#!/usr/bin/env bash
#
# Step 01: Removes .DS_Store files.
#
# Direct invocation:
# - ./_testAll_01_CleanDsStore.sh
#
# Log file:
# - ./temp/testAll/_testAll_01_CleanDsStore.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  run_cmd "$TESTALL_ROOT/clean-dsstore.sh" || fail "clean-dsstore.sh failed"
}

main "$@"
