#!/usr/bin/env bash
#
# Step 13: Builds all L432 configurations.
#
# Direct invocation:
# - ./_testAll_13_L432Configs.sh
#
# Log file:
# - ./temp/testAll/_testAll_13_L432Configs.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local rc
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  source "$TESTALL_ROOT/build_environment.sh" >>"$TESTALL_STEP_LOG" 2>&1 || fail "build_environment.sh failed"
  if ! has_command arm-none-eabi-gcc; then
    skip "arm-none-eabi-gcc not installed"
    exit 0
  fi
  (
    cd "$TESTALL_ROOT/examples/L432_inst" || exit 1
    ./all_configs_build.sh
  ) 2>&1 | tee -a "$TESTALL_STEP_LOG"
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    fail "L432 configuration builds failed" "$rc"
  fi
  if grep_log '(warning|error)' "$TESTALL_STEP_LOG"; then
    fail "L432 configuration builds reported warnings or errors" 2
  fi
}

main "$@"
