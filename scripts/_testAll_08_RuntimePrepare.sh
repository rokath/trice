#!/usr/bin/env bash
#
# Step 08: Writes environment information to the log and prepares the shared JSON files.
#
# Direct invocation:
# - ./_testAll_08_RuntimePrepare.sh
#
# Log file:
# - ./temp/log/_testAll_08_RuntimePrepare.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

log_runtime_context() {
  # Write the most relevant environment settings into this step log to make
  # later troubleshooting easier.
  log "SELECTED: ${SELECTED:-unknown}"
  log "OSTYPE: ${OSTYPE:-unknown}"
  log "GOCACHE: ${GOCACHE:-unset}"
  log "TRICE_TMP_DIR: ${TRICE_TMP_DIR:-unset}"
  log "TRICE_TIL_JSON: ${TRICE_TIL_JSON:-unset}"
  log "TRICE_LI_JSON: ${TRICE_LI_JSON:-unset}"
  if has_command uname; then
    run_cmd uname -a || true
  fi
  if has_command go; then
    run_cmd go version || true
  fi
}

main() {
  init_logfile
  log_runtime_context
  if has_command trice; then
    run_cmd which trice || true
    run_cmd trice version -v || { log "FAIL: trice version failed"; exit 1; }
  else
    log "MISSING TOOL: trice"
    log "WARN: trice not found in PATH"
  fi
  run_cmd "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" || { log "FAIL: initial clean IDs failed"; exit 1; }
  run_cmd rm -f "$TRICE_TIL_JSON" "$TRICE_LI_JSON" || { log "FAIL: failed to reset shared json files"; exit 1; }
  run_cmd touch "$TRICE_TIL_JSON" "$TRICE_LI_JSON" || { log "FAIL: failed to create shared json files"; exit 1; }
  run_cmd "$SCRIPTS_DIR/_renewIDs_in_examples_and_refresh_test_folder.sh" || { log "FAIL: renew IDs failed"; exit 1; }
}

main "$@"
