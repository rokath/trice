#!/usr/bin/env bash
#
# Test 600: Builds the same TRICE_ON GCC Example matrix in the Bound state.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"
# shellcheck source=./_140_trice_test_state.sh
source "$SCRIPT_DIR/_140_trice_test_state.sh"

main() {
  local selected

  selected="$(get_mode "${1:-full}")"
  init_logfile
  export LOGFILE
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    return 0
  fi
  trice_test_run_managed_workflow bind example "$SCRIPT_DIR/_200_gcc_example_build_worker.sh" on "$selected"
}

main "$@"
