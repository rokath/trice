#!/usr/bin/env bash
#
# Step 12: Builds the ARM examples with TRICE_OFF and TRICE_ON.
#
# Direct invocation:
# - ./_testAll_12_GccExampleBuilds.sh
# If arm-none-eabi-gcc is missing, the step is marked as SKIP.
#
# Log file:
# - ./temp/log/_testAll_12_GccExampleBuilds.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

cleanup_step() {
  cd "$ROOT" >/dev/null 2>&1 || true
  if [ "${ids_inserted:-0}" -eq 1 ]; then
    "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$LOGFILE" 2>&1 || true
  fi
}

main() {
  local rc
  init_logfile
  trap cleanup_step EXIT
  source "$SCRIPTS_DIR/_setup_build_environment.sh" >>"$LOGFILE" 2>&1 || {
    log "FAIL: _setup_build_environment.sh failed"
    exit 1
  }
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    exit 0
  fi
  run_cmd arm-none-eabi-gcc --version || true
  run_cmd which arm-none-eabi-gcc || true
  run_cmd "$ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || {
    log "FAIL: insert IDs failed"
    exit 1
  }
  ids_inserted=1
  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed before TRICE_OFF"
    exit 1
  }
  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_OFF.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "FAIL: TRICE_OFF builds failed"
    exit "$rc"
  fi
  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: TRICE_OFF builds reported warnings or errors"
    exit 2
  fi
  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed after TRICE_OFF"
    exit 1
  }
  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_ON.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "FAIL: TRICE_ON builds failed"
    exit "$rc"
  fi
  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: TRICE_ON builds reported warnings or errors"
    exit 2
  fi
  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed after TRICE_ON"
    exit 1
  }
}

ids_inserted=0
main "$@"
