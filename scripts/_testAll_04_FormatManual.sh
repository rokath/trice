#!/usr/bin/env bash
#
# Step 04: Checks the Trice User Manual formatting when mdtoc is installed.
#
# Direct invocation:
# - ./_testAll_04_FormatManual.sh
#
# Log file:
# - ./temp/log/_testAll_04_FormatManual.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  if ! has_command mdtoc; then
    log "MISSING TOOL: mdtoc"
    log "SKIP: mdtoc not installed"
    exit 0
  fi
  run_cmd "$SCRIPTS_DIR/_refresh_trice_user_manual.sh" check || {
    log "FAIL: _refresh_trice_user_manual.sh check failed"
    exit 1
  }
}

main "$@"
