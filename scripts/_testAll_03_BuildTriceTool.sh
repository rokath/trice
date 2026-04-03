#!/usr/bin/env bash
#
# Step 03: Builds the Trice tool.
#
# Direct invocation:
# - ./_testAll_03_BuildTriceTool.sh
#
# Log file:
# - ./temp/log/_testAll_03_BuildTriceTool.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  run_cmd env GOBIN="$TRICE_BIN_DIR" "$SCRIPTS_DIR/buildTriceTool.sh" || {
    log "FAIL: buildTriceTool.sh failed"
    exit 1
  }
}

main "$@"
