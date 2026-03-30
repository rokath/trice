#!/usr/bin/env bash
#
# Step 10: Runs the host-side CGO/PC target tests.
#
# Direct invocation:
# - ./_testAll_10_PcTargetTests.sh
# - ./_testAll_10_PcTargetTests.sh quick
# - ./_testAll_10_PcTargetTests.sh full
#
# Selection:
# - quick: only the smaller subset ./be_dblB_de_tcobs_ua/...
# - full: the full test set under ./_test/...
#
# Log file:
# - ./temp/testAll/_testAll_10_PcTargetTests.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

cleanup_step() {
  cd "$TESTALL_ROOT" >/dev/null 2>&1 || true
  if [ "${TESTALL_IDS_INSERTED:-0}" -eq 1 ]; then
    "$TESTALL_ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$TESTALL_STEP_LOG" 2>&1 || true
  fi
  if [ "${TESTALL_C_INCLUDE_PATH_SAVED+x}" = "x" ]; then
    export C_INCLUDE_PATH="$TESTALL_C_INCLUDE_PATH_SAVED"
  else
    unset C_INCLUDE_PATH || true
  fi
}

main() {
  local selected
  selected="$(require_mode_or_default "${1:-quick}")"
  ensure_testall_dirs
  prepare_shared_env "$selected"
  init_step_log "${BASH_SOURCE[0]}"
  trap cleanup_step EXIT
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  if ! has_command go; then
    skip "Go not installed"
    exit 0
  fi
  if [ "$selected" = "config" ]; then
    fail "Unsupported selection 'config'. Use quick or full." 2
  fi
  run_cmd "$TESTALL_ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || fail "insert IDs failed"
  TESTALL_IDS_INSERTED=1
  if [ "${C_INCLUDE_PATH:-}" != "" ]; then
    TESTALL_C_INCLUDE_PATH_SAVED="$C_INCLUDE_PATH"
    warn "Clearing C_INCLUDE_PATH temporarily for CGO tests"
    export C_INCLUDE_PATH=""
  fi
  cd "$TESTALL_ROOT/_test" || fail "cannot enter _test directory"
  if [ "$selected" = "quick" ]; then
    run_cmd go test ./be_dblB_de_tcobs_ua/... || fail "quick PC target tests failed"
  else
    run_cmd go test ./... || fail "full PC target tests failed"
  fi
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$TESTALL_STEP_LOG"; then
    fail "PC target test log contains FAIL markers" 2
  fi
}

TESTALL_IDS_INSERTED=0
main "$@"
