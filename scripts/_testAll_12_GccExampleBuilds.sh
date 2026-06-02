#!/usr/bin/env bash
#
# Step 12: Builds the ARM examples with TRICE_OFF and TRICE_ON.
#
# Direct invocation:
# - ./_testAll_12_GccExampleBuilds.sh
#
# If arm-none-eabi-gcc is missing, the step is marked as SKIP.
#
# Log file:
# - ./temp/log/_testAll_12_GccExampleBuilds.log
#
# Important cleanup ownership rule:
# - This Step-12 script builds all examples by calling:
#
#       examples/buildAllTargets_TRICE_OFF.sh
#       examples/buildAllTargets_TRICE_ON.sh
#
# - Individual example build scripts may now manage their own sequence:
#
#       pre-clean -> optional insert -> build -> final clean
#
# - Therefore this Step-12 script must NOT do one global `trice insert` before
#   the complete buildAllTargets run. Otherwise parent and child scripts both
#   modify the same source tree and cleanup can happen at surprising times.
#
# - Step 12 is only the outer orchestrator:
#
#       final safety clean -> clean build tree -> buildAllTargets_TRICE_OFF
#                          -> clean build tree -> buildAllTargets_TRICE_ON
#                          -> clean build tree -> final safety clean
#
# - The final safety clean is intentionally conservative. It is only a last
#   attempt to leave the repository in cleaned state if a child script failed,
#   was interrupted, or did not get a chance to run its own cleanup.
#
# Limitation:
# - Bash cleanup cannot protect against Ctrl-C exactly while the trice binary is
#   writing a file. That requires atomic write handling inside trice itself.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

cleanup_step() {
  # cleanup_step is used for both paths:
  #
  # 1. Normal successful execution:
  #      builds -> cleanup_step 0
  #
  # 2. Abnormal execution:
  #      build failure / Ctrl-C / SIGTERM -> cleanup_step <status>
  #
  # The function always exits the script with the status passed as $1.
  # If no status was passed, it uses the previous command status ($?).

  local status="${1:-$?}"
  local clean_status=0

  # Disable traps immediately.
  #
  # This prevents recursive cleanup if:
  # - cleanup itself triggers EXIT,
  # - the user presses Ctrl-C again while cleanup is already running,
  # - the clean helper exits with an error.
  trap - INT TERM EXIT

  cd "$ROOT" >/dev/null 2>&1 || true

  # Run a final safety clean after the build phase has started.
  #
  # This script no longer tracks "ids_inserted" because the individual example
  # build scripts own insert/clean now. The safety clean is harmless when the
  # tree is already clean and helpful after interrupted child builds.
  if [ "${build_phase_started:-0}" -eq 1 ]; then
    log "cleanup: running final trice clean safety helper"

    "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$LOGFILE" 2>&1
    clean_status=$?

    if [ "$clean_status" -ne 0 ]; then
      log "WARN: cleanup: final trice clean safety helper failed with exit code $clean_status"

      # If the build script was otherwise successful, a cleanup failure should
      # make the whole script fail. If the script was already failing, keep the
      # original failure status so the first/root cause is not hidden.
      if [ "$status" -eq 0 ]; then
        status="$clean_status"
      fi
    fi
  fi

  exit "$status"
}

main() {
  local rc

  init_logfile

  # One central cleanup owner for this outer orchestration script:
  #
  # - EXIT handles ordinary errors, for example a failed build step.
  # - INT handles Ctrl-C.
  # - TERM handles external termination, for example from CI or kill.
  #
  # Signal exit codes follow the common shell convention:
  # - 130 = interrupted by SIGINT
  # - 143 = terminated by SIGTERM
  trap 'cleanup_step $?' EXIT
  trap 'cleanup_step 130' INT
  trap 'cleanup_step 143' TERM

  source "$SCRIPTS_DIR/_setup_build_environment.sh" >>"$LOGFILE" 2>&1 || {
    log "FAIL: _setup_build_environment.sh failed"
    exit 1
  }

  if ! has_command arm-none-eabi-gcc; then
    log "MISSING TOOL: arm-none-eabi-gcc"
    log "SKIP: arm-none-eabi-gcc not installed"
    exit 0
  fi

  run_cmd arm-none-eabi-gcc --version || true
  run_cmd which arm-none-eabi-gcc || true

  # Start from a cleaned Trice-ID state.
  #
  # This is a pre-clean only. There is intentionally no global insert here,
  # because the individual example build scripts now manage their own insert
  # and clean behavior.
  run_cmd "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" || {
    log "FAIL: initial trice clean failed"
    exit 1
  }

  build_phase_started=1

  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed before G0B1_inst X0 matrix"
    exit 1
  }

  (
    cd "$ROOT/examples/G0B1_inst" || exit 1
    ./build.sh --x0-matrix
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}

  if [ "$rc" -ne 0 ]; then
    log "FAIL: G0B1_inst X0 matrix failed"
    exit "$rc"
  fi

  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: G0B1_inst X0 matrix reported warnings or errors"
    exit 2
  fi

  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed before TRICE_OFF"
    exit 1
  }

  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_OFF.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}

  if [ "$rc" -ne 0 ]; then
    log "FAIL: TRICE_OFF builds failed"
    exit "$rc"
  fi

  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: TRICE_OFF builds reported warnings or errors"
    exit 2
  fi

  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed after TRICE_OFF"
    exit 1
  }

  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_ON.sh
  ) 2>&1 | log_pipe
  rc=${PIPESTATUS[0]}

  if [ "$rc" -ne 0 ]; then
    log "FAIL: TRICE_ON builds failed"
    exit "$rc"
  fi

  if grep_log '(warning|error)' "$LOGFILE"; then
    log "FAIL: TRICE_ON builds reported warnings or errors"
    exit 2
  fi

  run_cmd "$ROOT/examples/cleanAllTargets.sh" || {
    log "FAIL: cleanAllTargets.sh failed after TRICE_ON"
    exit 1
  }

  # Normal successful path:
  #
  # Use the same cleanup function as the traps. This avoids duplicated final
  # safety-clean logic and makes success, failure and Ctrl-C consistent.
  cleanup_step 0
}

# State variable used by cleanup_step.
#
# build_phase_started:
# - 0: Do not run final safety clean yet.
# - 1: Build phase has started; final safety clean is allowed.
build_phase_started=0

main "$@"
