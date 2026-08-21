#!/usr/bin/env bash
#
# Test 540: Checks Go formatting and runs the normal Go tests.
#
# Direct invocation:
# - ./scripts/_540_test_go.sh
#
# Log file:
# - ./temp/log/_540_test_go.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  local package
  local package_list
  local packages=()

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
  run_cmd "$SCRIPTS_DIR/_270_format_go_code.sh" check || {
    log "FAIL: _270_format_go_code.sh check failed"
    exit 1
  }
  run_cmd go clean -cache -testcache || {
    log "FAIL: go clean failed"
    exit 1
  }

  # The CGO target matrix under ./_test has its own transactional workflow in
  # step 10. Excluding it here prevents every configuration from being executed
  # once with its historical per-folder mode before the ordered Bulk/Line pass.
  log "+ go list ./..."
  package_list="$(go list ./... 2>>"$LOGFILE")" || {
    log "FAIL: go list ./... failed"
    exit 1
  }
  for package in $package_list; do
    case "$package" in
      */_test/*) continue ;;
      *) packages+=("$package") ;;
    esac
  done
  if [ "${#packages[@]}" -eq 0 ]; then
    log "FAIL: go list returned no non-PC-test packages"
    exit 1
  fi
  run_cmd go test "${packages[@]}" || {
    log "FAIL: normal Go package tests failed"
    exit 1
  }
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$LOGFILE"; then
    log "FAIL: go test log contains FAIL markers"
    exit 2
  fi
}

main "$@"
