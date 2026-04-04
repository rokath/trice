#!/usr/bin/env bash
#
# Step 02: Formats the C sources.
#
# Direct invocation:
# - ./_testAll_02_ClangFormat.sh
#
# Log file:
# - ./temp/log/_testAll_02_ClangFormat.log

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
    log "SKIP: Go not installed (required by _format_c_code.sh)"
    exit 0
  fi
  run_cmd "$SCRIPTS_DIR/_format_c_code.sh" || {
    log "FAIL: _format_c_code.sh failed"
    exit 1
  }
}

main "$@"
