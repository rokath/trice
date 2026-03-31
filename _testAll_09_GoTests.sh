#!/usr/bin/env bash
#
# Step 09: Runs the normal Go tests.
#
# Direct invocation:
# - ./_testAll_09_GoTests.sh
#
# Log file:
# - ./temp/testAll/_testAll_09_GoTests.log

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
  run_cmd go clean -cache -testcache || fail "go clean failed"
  run_cmd go test ./... || fail "go test ./... failed"
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$TESTALL_STEP_LOG"; then
    fail "go test log contains FAIL markers" 2
  fi
}

main "$@"
