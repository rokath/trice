#!/usr/bin/env bash
#
# Test 530: Runs lychee for link checking.
#
# Direct invocation:
# - ./scripts/_530_test_links.sh
# If lychee is not installed locally, the step is marked as SKIP
# instead of failing.
#
# Log file:
# - ./temp/log/_530_test_links.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  init_logfile
  if ! has_command lychee; then
    log "MISSING TOOL: lychee"
    log "SKIP: lychee not installed"
    exit 0
  fi
  run_cmd lychee --config "$ROOT/lychee.toml" . || {
    log "FAIL: lychee failed"
    exit 1
  }
}

main "$@"
