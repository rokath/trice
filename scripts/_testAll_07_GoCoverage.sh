#!/usr/bin/env bash
#
# Step 07: Runs Go coverage and a temp-local Go install.
#
# Direct invocation:
# - ./_testAll_07_GoCoverage.sh
# If Go is not installed locally, the step is marked as SKIP.
#
# Log file:
# - ./temp/log/_testAll_07_GoCoverage.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local coverage_file="./temp/log/coverage.out"
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  run_cmd go test ./... -covermode=atomic -coverprofile="$coverage_file" -coverpkg=./... || {
    log "FAIL: go coverage test failed"
    exit 1
  }
  run_cmd go tool cover -func="$coverage_file" || {
    log "FAIL: go tool cover -func failed"
    exit 1
  }
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  run_cmd env GOBIN="$TRICE_BIN_DIR" go install ./... || {
    log "FAIL: temp-local go install ./... failed"
    exit 1
  }
}

main "$@"
