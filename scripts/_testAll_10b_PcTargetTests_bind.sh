#!/usr/bin/env bash
#
# Step 10b: Runs the shared PC target tests in the Bound state.

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
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    return 0
  fi
  trice_test_run_managed_workflow bind pc "$SCRIPT_DIR/_testAll_10_PcTargetTests_common.sh" "$selected"
}

main "$@"
