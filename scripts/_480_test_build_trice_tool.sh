#!/usr/bin/env bash
#
# Test 480: Builds the Trice and tlog tools.
#
# Direct invocation:
# - ./scripts/_480_test_build_trice_tool.sh
#
# Notes:
# - This QA step installs into the temp-local TRICE_BIN_DIR prepared by the
#   shared test harness.
# - The build helper now protects existing installations by default and may ask
#   interactively before overwriting.
# - For automated test runs we therefore force a non-interactive overwrite and
#   disable backup creation. Backups are useful for real user installations,
#   but they would only create churn inside the disposable temp test bin folder.
#
# Log file:
# - ./temp/log/_480_test_build_trice_tool.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  local goexe
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi
  goexe="$(go env GOEXE)"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  # The logfile is an existing regular file, so it is a deterministic invalid
  # GOROOT. This verifies that the standalone build helper ignores stale
  # overrides instead of depending on the test harness to sanitize them.
  run_cmd env GOROOT="$LOGFILE" "$SCRIPTS_DIR/buildTriceTool.sh" --target-file "$TRICE_BIN_DIR/trice${goexe}" --force --no-backup --silent || {
    log "FAIL: buildTriceTool.sh failed"
    exit 1
  }
  if [ ! -x "$TRICE_BIN_DIR/tlog${goexe}" ]; then
    log "FAIL: expected tlog binary was not built: $TRICE_BIN_DIR/tlog${goexe}"
    exit 1
  fi
}

main "$@"
