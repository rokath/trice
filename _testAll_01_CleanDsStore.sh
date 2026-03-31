#!/usr/bin/env bash
#
# Step 01: Removes .DS_Store files.
#
# Direct invocation:
# - ./_testAll_01_CleanDsStore.sh
#
# Log file:
# - ./temp/testAll/_testAll_01_CleanDsStore.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  run_cmd "$ROOT/clean-dsstore.sh" || { log "FAIL: clean-dsstore.sh failed"; exit 1; }
}

main "$@"
