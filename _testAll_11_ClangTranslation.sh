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
# - ./temp/testAll/_testAll_11_ClangTranslation.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local rc
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  source "$TESTALL_ROOT/build_environment.sh" >>"$TESTALL_STEP_LOG" 2>&1 || fail "build_environment.sh failed"
  if ! has_command clang; then
    skip "clang not installed"
    exit 0
  fi
  if ! has_command arm-none-eabi-gcc; then
    warn "arm-none-eabi-gcc not installed"
    skip "Skipping clang translation because ARM GCC libraries are needed"
    exit 0
  fi
  (
    cd "$TESTALL_ROOT/examples/G0B1_inst" || exit 1
    printf 'clang --version\n'
    clang --version
    make clean
    ./build_with_clang.sh
  ) 2>&1 | tee -a "$TESTALL_STEP_LOG"
  rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    fail "clang translation failed" "$rc"
  fi

  # At this point the build already completed successfully.
  # We still scan the captured log for warnings or errors because the
  # underlying make/clang calls may report issues without returning a hard
  # non-zero exit code in every case.
  #
  # Intentionally do not filter specific warnings here.
  # The build environment is expected to be configured cleanly enough that the
  # Clang translation runs without known benign warnings.
  if grep_log '(warning|error)' "$TESTALL_STEP_LOG"; then
    fail "clang translation reported warnings or errors" 2
  fi
}

main "$@"
