#!/usr/bin/env bash
#
# Step 01: Checks for tracked or stray .DS_Store files.
#
# Direct invocation:
# - ./_testAll_01_CleanDsStore.sh
#
# Log file:
# - ./temp/log/_testAll_01_CleanDsStore.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local ds_store_files
  init_logfile
  ds_store_files="$(find "$ROOT" \
    -path "$ROOT/.git" -prune -o \
    -name '.DS_Store' -print)"
  if [ -n "$ds_store_files" ]; then
    log "FAIL: .DS_Store files found"
    printf '%s\n' "$ds_store_files" | while IFS= read -r f; do
      [ -n "$f" ] && log "  $f"
    done
    log "Remove them locally, for example with: ./scripts/_clean-dsstore.sh"
    exit 1
  fi
  log "PASS: no .DS_Store files found"
}

main "$@"
