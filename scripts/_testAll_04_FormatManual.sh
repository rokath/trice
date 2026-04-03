#!/usr/bin/env bash
#
# Step 04: Updates the manual TOC.
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
  run_cmd "$SCRIPTS_DIR/format_dumeng_toc.sh" || {
    log "FAIL: format_dumeng_toc.sh failed"
    exit 1
  }
}

main "$@"
