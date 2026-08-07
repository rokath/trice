#!/usr/bin/env bash
#
# Step 11a: Checks the G0B1 Clang translation in the Inserted state.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"
# shellcheck source=./_trice_test_state.sh
source "$SCRIPT_DIR/_trice_test_state.sh"

main() {
  init_logfile
  export LOGFILE
  if ! has_command clang; then
    log "MISSING TOOL: clang"
    log "SKIP: clang not installed"
    return 0
  fi
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed; ARM GCC libraries are required"
    return 0
  fi
  trice_test_run_managed_workflow insert example "$SCRIPT_DIR/_testAll_11_ClangTranslation_common.sh"
}

main "$@"
