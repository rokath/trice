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
  local repository_root_uri
  local repository_source_remap

  init_logfile
  if ! has_command lychee; then
    log "MISSING TOOL: lychee"
    log "SKIP: lychee not installed"
    exit 0
  fi
  # Git Bash exposes a Windows drive as /c/... while file URIs require /c:/....
  case "$ROOT" in
    /[[:alpha:]]/*) repository_root_uri="file:///${ROOT:1:1}:${ROOT:2}" ;;
    *) repository_root_uri="file://$ROOT" ;;
  esac
  # Keep the absolute URL in CLI help while validating its repository target locally.
  repository_source_remap="https://github.com/rokath/trice/blob/main/_test/testdata/triceCheck.c $repository_root_uri/_test/testdata/triceCheck.c"
  run_cmd lychee --config "$ROOT/lychee.toml" --remap "$repository_source_remap" . || {
    log "FAIL: lychee failed"
    exit 1
  }
}

main "$@"
