#!/usr/bin/env bash
#
# Step 07: Runs Go coverage and go install.
#
# Direct invocation:
# - ./_testAll_07_GoCoverage.sh
# If Go is not installed locally, the step is marked as SKIP.
#
# Log file:
# - ./temp/testAll/_testAll_07_GoCoverage.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  if ! has_command go; then
    skip "Go not installed"
    exit 0
  fi
  run_cmd go test ./... -covermode=atomic -coverprofile=coverage.out -coverpkg=./... || fail "go coverage test failed"
  run_cmd go tool cover -func=coverage.out || fail "go tool cover -func failed"
  run_cmd go install ./... || fail "go install ./... failed"
}

main "$@"
