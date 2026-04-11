#!/usr/bin/env bash
#
# Step 14: Generates all local GoReleaser snapshot assets into ./dist/.
#
# Direct invocation:
# - ./_testAll_14_GoReleaserSnapshot.sh
#
# Log file:
# - ./temp/log/_testAll_14_GoReleaserSnapshot.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile

  if ! has_command goreleaser; then
    log "SKIP: goreleaser not installed locally; skipping local release-asset build"
    exit 0
  fi

  if ! has_command npx; then
    log "SKIP: npx not installed locally; skipping local release-asset build"
    log "INFO: The local GoReleaser snapshot needs Node.js because the manual PDF is generated headlessly."
    exit 0
  fi

  run_cmd goreleaser release --snapshot --clean || {
    log "FAIL: local GoReleaser snapshot build failed"
    exit 1
  }

  log "OK: local release artifacts generated under ./dist/"
}

main "$@"
