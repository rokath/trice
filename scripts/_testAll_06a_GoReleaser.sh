#!/usr/bin/env bash
#
# Step 06a: Checks the local GoReleaser configuration when goreleaser is installed.
#
# Direct invocation:
# - ./_testAll_06a_GoReleaser.sh
#
# Log file:
# - ./temp/log/_testAll_06a_GoReleaser.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile

  if ! has_command goreleaser; then
    log "SKIP: goreleaser not installed locally; skipping goreleaser check"
    exit 0
  fi

  run_cmd goreleaser check || {
    log "FAIL: goreleaser check failed"
    exit 1
  }
}

main "$@"
