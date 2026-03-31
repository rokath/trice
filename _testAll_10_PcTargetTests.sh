#!/usr/bin/env bash
#
# Step 10: Runs the host-side CGO/PC target tests.
#
# Direct invocation:
# - ./_testAll_10_PcTargetTests.sh
# - ./_testAll_10_PcTargetTests.sh full
#
# Selection:
# - direct invocation default: full test set under ./_test/...
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
    pc_trice_til_json="$TRICE_TIL_JSON"
  else
    pc_trice_til_json="$ROOT/demoTIL.json"
  fi

  if [ "${TRICE_LI_JSON+x}" = "x" ]; then
    pc_trice_li_json="$TRICE_LI_JSON"
  else
    pc_trice_li_json="$ROOT/demoLI.json"
  fi
}

cleanup_step() {
  cd "$ROOT" >/dev/null 2>&1 || true
  if [ "${ids_inserted:-0}" -eq 1 ]; then
    TRICE_TIL_JSON="$pc_trice_til_json" \
      TRICE_LI_JSON="$pc_trice_li_json" \
      "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$LOGFILE" 2>&1 || true
  fi
  if [ "${saved_c_include_path+x}" = "x" ]; then
    export C_INCLUDE_PATH="$saved_c_include_path"
  else
    unset C_INCLUDE_PATH || true
  fi
}

main() {
  local selected
  selected="$(get_mode "${1:-full}")"
  export SELECTED="$selected"
  select_pc_test_json_paths
  init_logfile
  trap cleanup_step EXIT
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  run_cmd env \
    TRICE_TIL_JSON="$pc_trice_til_json" \
    TRICE_LI_JSON="$pc_trice_li_json" \
    "$ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || { log "FAIL: insert IDs failed"; exit 1; }
  ids_inserted=1
  if [ "${C_INCLUDE_PATH:-}" != "" ]; then
    saved_c_include_path="$C_INCLUDE_PATH"
    log "WARN: Clearing C_INCLUDE_PATH temporarily for CGO tests"
    export C_INCLUDE_PATH=""
  fi
  cd "$ROOT/_test" || { log "FAIL: cannot enter _test directory"; exit 1; }
  if [ "$selected" = "quick" ]; then
    run_cmd go test ./be_dblB_de_tcobs_ua/... || { log "FAIL: quick PC target tests failed"; exit 1; }
  else
    run_cmd go test ./... || { log "FAIL: full PC target tests failed"; exit 1; }
  fi
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$LOGFILE"; then
    log "FAIL: PC target test log contains FAIL markers"
    exit 2
  fi
}

ids_inserted=0
main "$@"
