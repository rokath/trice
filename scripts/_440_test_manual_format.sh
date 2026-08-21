#!/usr/bin/env bash
#
# Test 440: Checks the Trice User Manual formatting when mdtoc is installed.
#
# Direct invocation:
# - ./scripts/_440_test_manual_format.sh
#
# Log file:
# - ./temp/log/_440_test_manual_format.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  init_logfile
  if ! has_command mdtoc; then
    log "MISSING TOOL: mdtoc"
    log "SKIP: mdtoc not installed"
    exit 0
  fi
  run_cmd "$SCRIPTS_DIR/_310_refresh_trice_user_manual.sh" check || {
    log "FAIL: _310_refresh_trice_user_manual.sh check failed"
    exit 1
  }
}

main "$@"
