#!/usr/bin/env bash
#
# Test 470: Runs actionlint for GitHub workflow files.
#
# Direct invocation:
# - ./scripts/_470_test_actionlint.sh
#
# Log file:
# - ./temp/log/_470_test_actionlint.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  init_logfile

  if has_command actionlint; then
    run_cmd actionlint "$ROOT/.github/workflows/"*.yml || {
      log "FAIL: actionlint failed"
      exit 1
    }
    return 0
  fi

  log "SKIP: actionlint not installed locally; install it to run workflow lint"
  exit 0
}

main "$@"
