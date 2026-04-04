#!/usr/bin/env bash
#
# Step 09: Checks Go formatting and runs the normal Go tests.
#
# Direct invocation:
# - ./_testAll_09_GoTests.sh
#
# Log file:
# - ./temp/log/_testAll_09_GoTests.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  if ! has_command gofmt; then
    log "MISSING TOOL: gofmt"
    log "SKIP: gofmt not installed"
    exit 0
  fi
  run_cmd "$SCRIPTS_DIR/_format_go_code.sh" check || {
    log "FAIL: _format_go_code.sh check failed"
    exit 1
  }
  run_cmd go clean -cache -testcache || {
    log "FAIL: go clean failed"
    exit 1
  }
  run_cmd go test ./... || {
    log "FAIL: go test ./... failed"
    exit 1
  }
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$LOGFILE"; then
    log "FAIL: go test log contains FAIL markers"
    exit 2
  fi
}

main "$@"
