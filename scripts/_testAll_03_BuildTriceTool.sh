#!/usr/bin/env bash
#
# Step 03: Builds the Trice tool.
#
# Direct invocation:
# - ./_testAll_03_BuildTriceTool.sh
#
# Notes:
# - This QA step installs into the temp-local TRICE_BIN_DIR prepared by the
#   shared test harness.
# - The build helper now protects existing installations by default and may ask
#   interactively before overwriting.
# - For automated test runs we therefore force a non-interactive overwrite and
#   disable backup creation. Backups are useful for real user installations,
#   but they would only create churn inside the disposable temp test bin folder.
#
# Log file:
# - ./temp/log/_testAll_03_BuildTriceTool.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local goexe
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  goexe="$(go env GOEXE)"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  run_cmd "$SCRIPTS_DIR/buildTriceTool.sh" --target-file "$TRICE_BIN_DIR/trice${goexe}" --force --no-backup --silent || {
    log "FAIL: buildTriceTool.sh failed"
    exit 1
  }
}

main "$@"
