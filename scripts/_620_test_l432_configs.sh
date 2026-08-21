#!/usr/bin/env bash
#
# Test 620: Builds all L432 configurations.
#
# Direct invocation:
# - ./scripts/_620_test_l432_configs.sh
#
# Log file:
# - ./temp/log/_620_test_l432_configs.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"
# shellcheck source=./_140_trice_test_state.sh
source "$SCRIPT_DIR/_140_trice_test_state.sh"

# run_l432_matrix keeps the historical warning/error policy inside the managed
# workflow. This lets the workflow helper restore source files, metadata, and
# include paths even when one of the 101 configurations fails.
run_l432_matrix() {
  local output
  local rc=0

  # Capture this one matrix as a unit so diagnostics can be inspected before
  # run_cmd mirrors the output into LOGFILE. This avoids a race with tee during
  # direct, non-quiet invocation of the step.
  output="$(
    cd "$ROOT/examples/L432_inst" || exit 1
    ./all_configs_build.sh 2>&1
  )" || rc=$?
  printf '%s\n' "$output"
  if [ "$rc" -ne 0 ]; then
    printf 'FAIL: L432 configuration builds failed\n' >&2
    return "$rc"
  fi
  if grep -Eiq '(warning|error)' <<<"$output"; then
    printf 'FAIL: L432 configuration builds reported warnings or errors\n' >&2
    return 2
  fi
}

main() {
  init_logfile
  export LOGFILE
  source "$SCRIPTS_DIR/_150_setup_build_environment.sh" >>"$LOGFILE" 2>&1 || {
    log "FAIL: _150_setup_build_environment.sh failed"
    exit 1
  }
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    exit 0
  fi
  trice_test_run_managed_workflow bind example run_l432_matrix
}

main "$@"
