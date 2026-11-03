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
  local go_timeout

  init_logfile
  go_timeout="$(go_package_test_timeout "${TRICE_GO_TEST_TIMEOUT-2h}")" || exit 2
  log "Go test timeout per package: $go_timeout"
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
      # Disposable project copies are not repository test packages. Git ignore
      # rules do not prevent go list from discovering their Go sources.
      */temp/*) continue ;;
      *) packages+=("$package") ;;
    esac
  done
  if [ "${#packages[@]}" -eq 0 ]; then
    log "FAIL: go list returned no non-PC-test packages"
    exit 1
  fi
  # Reuse compiled dependencies, but execute tests on every suite invocation.
  # Clearing the global cache here would also discard the PC worker's builds.
  # Native script fixtures can exceed thirty minutes even while making progress.
  # Keep a generous, overridable package budget for slower hosts and cold caches.
  run_go_tests -count=1 "${packages[@]}" -skip '^TestContextEnrichmentRebasePoC$' "-timeout=$go_timeout" || {
    local status=$?
    log "FAIL: normal Go package tests failed"
    exit "$status"
  }
  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$LOGFILE"; then
    log "FAIL: go test log contains FAIL markers"
    exit 2
  fi
}

main "$@"
