#!/usr/bin/env bash
#
# Step 02: Formats the C sources.
#
# Direct invocation:
# - ./_testAll_02_ClangFormat.sh
#
# Log file:
# - ./temp/testAll/_testAll_02_ClangFormat.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  if ! has_command clang-format; then
    log "MISSING TOOL: clang-format"
    log "SKIP: clang-format not installed"
    exit 0
  fi
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed (required by clang-format.sh)"
    exit 0
  fi
  run_cmd "$ROOT/clang-format.sh" || { log "FAIL: clang-format.sh failed"; exit 1; }
}

main "$@"
