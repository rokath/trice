#!/usr/bin/env bash
#
# Test 460: Checks the local GoReleaser configuration when goreleaser is installed.
#
# Direct invocation:
# - ./scripts/_460_test_goreleaser_config.sh
#
# Log file:
# - ./temp/log/_460_test_goreleaser_config.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  init_logfile

  has_goreleaser_config_support || return 0

  run_cmd goreleaser check || {
    log "FAIL: goreleaser check failed"
    exit 1
  }
}

main "$@"
