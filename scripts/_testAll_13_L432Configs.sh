#!/usr/bin/env bash
#
# Step 13: Builds all L432 configurations.
#
# Direct invocation:
# - ./_testAll_13_L432Configs.sh
#
# Log file:
# - ./temp/log/_testAll_13_L432Configs.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local rc
  init_logfile
  source "$SCRIPTS_DIR/_setup_build_environment.sh" >>"$LOGFILE" 2>&1 || { log "FAIL: _setup_build_environment.sh failed"; exit 1; }
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    exit 0
  fi
  (
    cd "$ROOT/examples/L432_inst" || exit 1
    ./all_configs_build.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "FAIL: L432 configuration builds failed"
    exit "$rc"
  fi
  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: L432 configuration builds reported warnings or errors"
    exit 2
  fi
}

main "$@"
