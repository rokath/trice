#!/usr/bin/env bash
#
# Step 07: Runs Go coverage and a temp-local Trice tool build.
#
# Direct invocation:
# - ./_testAll_07_GoCoverage.sh
# If Go is not installed locally, the step is marked as SKIP.
#
# Notes:
# - The coverage run still uses Go's native test coverage across ./....
# - The follow-up tool build now writes explicitly into the disposable
#   TRICE_BIN_DIR instead of relying on go install destination resolution.
# - This keeps any user-installed trice binary outside the test sandbox
#   untouched, even when the step fails or is interrupted.
#
# Log file:
# - ./temp/log/_testAll_07_GoCoverage.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local coverage_file="./temp/log/coverage.out"
  local goexe
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
  goexe="$(go env GOEXE)"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  run_cmd "$SCRIPTS_DIR/buildTriceTool.sh" --target-file "$TRICE_BIN_DIR/trice${goexe}" --force --no-backup --silent || {
    log "FAIL: temp-local trice build failed"
    exit 1
  }
}

main "$@"
