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

select_pc_test_json_paths() {
  # The CGO tests read demoTIL.json and demoLI.json from the repository root.
  # Therefore the insert/clean helper scripts in this step must target the same
  # files by default.
  #
  # At the same time we still want to respect an explicit outer override, for
  # example from CI or from a developer invoking the step with custom paths.
  if [ "${TRICE_TIL_JSON+x}" = "x" ]; then
    TESTALL_PC_TRICE_TIL_JSON="$TRICE_TIL_JSON"
  else
    TESTALL_PC_TRICE_TIL_JSON="$TESTALL_ROOT/demoTIL.json"
  fi

  if [ "${TRICE_LI_JSON+x}" = "x" ]; then
    TESTALL_PC_TRICE_LI_JSON="$TRICE_LI_JSON"
  else
    TESTALL_PC_TRICE_LI_JSON="$TESTALL_ROOT/demoLI.json"
  fi
}

cleanup_step() {
  cd "$TESTALL_ROOT" >/dev/null 2>&1 || true
  if [ "${TESTALL_IDS_INSERTED:-0}" -eq 1 ]; then
    TRICE_TIL_JSON="$TESTALL_PC_TRICE_TIL_JSON" \
      TRICE_LI_JSON="$TESTALL_PC_TRICE_LI_JSON" \
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
  select_pc_test_json_paths
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
  run_cmd env \
    TRICE_TIL_JSON="$TESTALL_PC_TRICE_TIL_JSON" \
    TRICE_LI_JSON="$TESTALL_PC_TRICE_LI_JSON" \
    "$TESTALL_ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || fail "insert IDs failed"
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
