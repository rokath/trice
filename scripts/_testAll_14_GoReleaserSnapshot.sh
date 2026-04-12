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
ROOT_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"

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

  if [ -f "$ROOT_DIR/temp/release/TriceUserManual.pdf" ]; then
    run_cmd mkdir -p "$ROOT_DIR/dist" || {
      log "FAIL: could not ensure ./dist/ exists for the local manual PDF copy"
      exit 1
    }
    run_cmd cp -f "$ROOT_DIR/temp/release/TriceUserManual.pdf" "$ROOT_DIR/dist/TriceUserManual.pdf" || {
      log "FAIL: could not copy the generated manual PDF into ./dist/"
      exit 1
    }
  else
    log "WARN: temp/release/TriceUserManual.pdf was not generated; dist/ will not contain the manual PDF"
  fi

  log "OK: local release artifacts generated under ./dist/"
}

main "$@"
