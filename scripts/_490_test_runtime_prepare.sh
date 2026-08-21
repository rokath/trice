#!/usr/bin/env bash
#
# Test 490: Writes environment information and prepares the canonical Bind state.
#
# Direct invocation:
# - ./scripts/_490_test_runtime_prepare.sh
#
# Log file:
# - ./temp/log/_490_test_runtime_prepare.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

log_runtime_context() {
  # Write the most relevant environment settings into this step log to make
  # later troubleshooting easier.
  log "SELECTED: ${SELECTED:-unknown}"
  log "OSTYPE: ${OSTYPE:-unknown}"
  log "GOCACHE: ${GOCACHE:-unset}"
  log "TRICE_TMP_DIR: ${TRICE_TMP_DIR:-unset}"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
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
    run_cmd trice version -v || {
      log "FAIL: trice version failed"
      exit 1
    }
  else
    log "MISSING TOOL: trice"
    log "WARN: trice not found in PATH"
  fi

  # Both selections start from the checked-in Bind state. Full-mode Legacy
  # checks run later in isolated or transactionally restored test workflows.
  # Keeping ID renewal out of this shared preparation preserves stable file keys.
  run_cmd "$ROOT/trice_bindIDs_in_examples_and_test_folder.sh" || {
    log "FAIL: preparing canonical Bind state failed"
    exit 1
  }
}

main "$@"
