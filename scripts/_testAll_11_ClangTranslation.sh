#!/usr/bin/env bash
#
# Step 11: Checks the clang translation for G0B1_inst.
#
# Direct invocation:
# - ./_testAll_11_ClangTranslation.sh
# If clang or arm-none-eabi-gcc are missing, the step is marked as SKIP/WARN
# instead of failing immediately due to the environment.
#
# Log file:
# - ./temp/log/_testAll_11_ClangTranslation.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local rc
  init_logfile
  source "$SCRIPTS_DIR/_setup_build_environment.sh" >>"$LOGFILE" 2>&1 || { log "FAIL: _setup_build_environment.sh failed"; exit 1; }
  if ! has_command clang; then
    log "MISSING TOOL: clang"
    log "SKIP: clang not installed"
    exit 0
  fi
  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "WARN: arm-none-eabi-gcc not installed"
    log "SKIP: Skipping clang translation because ARM GCC libraries are needed"
    exit 0
  fi
  (
    cd "$ROOT/examples/G0B1_inst" || exit 1
    printf 'clang --version\n'
    clang --version
    make clean
    ./build_with_clang.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "FAIL: clang translation failed"
    exit "$rc"
  fi

  # At this point the build already completed successfully.
  # We still scan the captured log for warnings or errors because the
  # underlying make/clang calls may report issues without returning a hard
  # non-zero exit code in every case.
  #
  # Intentionally do not filter specific warnings here.
  # The build environment is expected to be configured cleanly enough that the
  # Clang translation runs without known benign warnings.
  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: clang translation reported warnings or errors"
    exit 2
  fi
}

main "$@"
