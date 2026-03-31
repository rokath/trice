#!/usr/bin/env bash
#
# Step 06: Runs lychee for link checking.
#
# Direct invocation:
# - ./_testAll_06_LinkCheck.sh
# If lychee is not installed locally, the step is marked as SKIP
# instead of failing.
#
# Log file:
# - ./temp/testAll/_testAll_06_LinkCheck.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  if ! has_command lychee; then
    log "MISSING TOOL: lychee"
    log "SKIP: lychee not installed"
    exit 0
  fi
  run_cmd lychee . || { log "FAIL: lychee failed"; exit 1; }
}

main "$@"
