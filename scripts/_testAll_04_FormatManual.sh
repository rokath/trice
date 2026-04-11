#!/usr/bin/env bash
#
# Step 04: Checks the Trice User Manual formatting.
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
  run_cmd "$SCRIPTS_DIR/_refresh_trice_user_manual.sh" check || {
    log "FAIL: _refresh_trice_user_manual.sh check failed"
    exit 1
  }
}

main "$@"
