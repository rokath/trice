#!/usr/bin/env bash
#
# Test 430: Checks C source formatting.
#
# Direct invocation:
# - ./scripts/_430_test_clang_format.sh
#
# Log file:
# - ./temp/log/_430_test_clang_format.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  init_logfile
  if ! has_command clang-format; then
    log "MISSING TOOL: clang-format"
    log "SKIP: clang-format not installed"
    exit 0
  fi
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed (required by _280_format_c_code.sh)"
    exit 0
  fi
  run_cmd "$SCRIPTS_DIR/_280_format_c_code.sh" || {
    log "FAIL: _280_format_c_code.sh failed"
    exit 1
  }
}

main "$@"
