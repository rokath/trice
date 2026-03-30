#!/usr/bin/env bash
#
# Step 12: Builds the ARM examples with TRICE_OFF and TRICE_ON.
#
# Direct invocation:
# - ./_testAll_12_GccExampleBuilds.sh
# If arm-none-eabi-gcc is missing, the step is marked as SKIP.
#
# Log file:
# - ./temp/testAll/_testAll_12_GccExampleBuilds.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

cleanup_step() {
  cd "$TESTALL_ROOT" >/dev/null 2>&1 || true
  if [ "${TESTALL_IDS_INSERTED:-0}" -eq 1 ]; then
    "$TESTALL_ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$TESTALL_STEP_LOG" 2>&1 || true
  fi
}

main() {
  local rc
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  trap cleanup_step EXIT
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  source "$TESTALL_ROOT/build_environment.sh" >>"$TESTALL_STEP_LOG" 2>&1 || fail "build_environment.sh failed"
  if ! has_command arm-none-eabi-gcc; then
    skip "arm-none-eabi-gcc not installed"
    exit 0
  fi
  run_cmd arm-none-eabi-gcc --version || true
  run_cmd which arm-none-eabi-gcc || true
  run_cmd "$TESTALL_ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || fail "insert IDs failed"
  TESTALL_IDS_INSERTED=1
  run_cmd "$TESTALL_ROOT/examples/cleanAllTargets.sh" || fail "cleanAllTargets.sh failed before TRICE_OFF"
  (
    cd "$TESTALL_ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_OFF.sh
  ) 2>&1 | tee -a "$TESTALL_STEP_LOG"
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    fail "TRICE_OFF builds failed" "$rc"
  fi
  if grep_log '(warning|error)' "$TESTALL_STEP_LOG"; then
    fail "TRICE_OFF builds reported warnings or errors" 2
  fi
  run_cmd "$TESTALL_ROOT/examples/cleanAllTargets.sh" || fail "cleanAllTargets.sh failed after TRICE_OFF"
  (
    cd "$TESTALL_ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_ON.sh
  ) 2>&1 | tee -a "$TESTALL_STEP_LOG"
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    fail "TRICE_ON builds failed" "$rc"
  fi
  if grep_log '(warning|error)' "$TESTALL_STEP_LOG"; then
    fail "TRICE_ON builds reported warnings or errors" 2
  fi
  run_cmd "$TESTALL_ROOT/examples/cleanAllTargets.sh" || fail "cleanAllTargets.sh failed after TRICE_ON"
}

TESTALL_IDS_INSERTED=0
main "$@"
