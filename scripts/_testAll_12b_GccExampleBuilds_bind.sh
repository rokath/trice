#!/usr/bin/env bash
#
# Step 12b: Builds the same TRICE_ON GCC Example matrix in the Bound state.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"
# shellcheck source=./_trice_test_state.sh
source "$SCRIPT_DIR/_trice_test_state.sh"

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
  trice_test_run_managed_workflow bind example "$SCRIPT_DIR/_testAll_12_GccExampleBuilds_common.sh" on "$selected"
}

main "$@"
