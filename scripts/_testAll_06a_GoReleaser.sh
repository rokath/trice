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

require_goreleaser_version() {
  local version
  version="$(goreleaser --version 2>/dev/null | awk '/GitVersion:/ { print $2; exit }')"

  case "$version" in
    v2.[1-9][0-9]* | 2.[1-9][0-9]* | v3.* | 3.*) ;;
    *)
      echo "FAIL: GoReleaser >= v2.10 is required because .goreleaser.yaml uses homebrew_casks."
      echo "Found: ${version:-unknown}"
      echo "Binary: $(command -v goreleaser)"
      exit 1
      ;;
  esac
}

main() {
  init_logfile

  if ! has_command goreleaser; then
    log "SKIP: goreleaser not installed locally; skipping goreleaser check"
    exit 0
  fi

  require_goreleaser_version

  run_cmd goreleaser check || {
    log "FAIL: goreleaser check failed"
    exit 1
  }
}

main "$@"
