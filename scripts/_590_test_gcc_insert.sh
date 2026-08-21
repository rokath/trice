#!/usr/bin/env bash
#
# Test 590: Builds the TRICE_ON GCC Example matrix in the Inserted state.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"
# shellcheck source=./_140_trice_test_state.sh
source "$SCRIPT_DIR/_140_trice_test_state.sh"

main() {
  init_logfile
  export LOGFILE
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    return 0
  fi
  trice_test_run_managed_workflow insert example "$SCRIPT_DIR/_200_gcc_example_build_worker.sh" on
}

main "$@"
