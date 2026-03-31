#!/usr/bin/env bash
#
# Step 08: Writes environment information to the log and prepares shared temp files.
#
# Direct invocation:
# - ./_testAll_08_RuntimePrepare.sh
#
# Log file:
# - ./temp/testAll/_testAll_08_RuntimePrepare.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  log_environment
  if has_command trice; then
    run_cmd which trice || true
    run_cmd trice version || fail "trice version failed"
  else
    warn "trice not found in PATH"
  fi
  run_cmd "$TESTALL_ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" || fail "initial clean IDs failed"
  run_cmd rm -f "$TRICE_TIL_JSON" "$TRICE_LI_JSON" || fail "failed to reset shared json files"
  run_cmd touch "$TRICE_TIL_JSON" "$TRICE_LI_JSON" || fail "failed to create shared json files"
  run_cmd "$TESTALL_ROOT/renewIDs_in_examples_and_refresh_test_folder.sh" || fail "renew IDs failed"
}

main "$@"
