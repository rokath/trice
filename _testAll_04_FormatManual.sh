#!/usr/bin/env bash
#
# Step 04: Updates the manual TOC.
#
# Direct invocation:
# - ./_testAll_04_FormatManual.sh
#
# Log file:
# - ./temp/testAll/_testAll_04_FormatManual.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  run_cmd "$ROOT/format-dumeng-toc.sh" || { log "FAIL: format-dumeng-toc.sh failed"; exit 1; }
}

main "$@"
