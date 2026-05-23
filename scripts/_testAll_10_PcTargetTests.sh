#!/usr/bin/env bash
#
# Step 10: Runs the host-side CGO/PC target tests.
#
# Direct invocation:
# - ./_testAll_10_PcTargetTests.sh
# - ./_testAll_10_PcTargetTests.sh full
#
# Selection:
# - direct invocation default: full test set under ./_test/...
# - quick: only the smaller subset ./be_dblB_de_tcobs_ua/...
# - full: the full test set under ./_test/...
#
# Log file:
# - ./temp/log/_testAll_10_PcTargetTests.log
#
# Cleanup / Ctrl-C behavior:
# - This script calls a helper script that runs `trice insert`.
# - After the tests, this script calls a helper script that runs `trice clean`.
# - If the user presses Ctrl-C after a successful insert, or if the tests fail,
#   this script should still try to run the clean helper.
# - The cleanup is intentionally handled here, in this outer workflow script,
#   because this is the level that knows the complete sequence:
#
#       insert -> test -> clean
#
# - The insert/clean helper scripts should stay simple. They should not contain
#   their own "clean after insert" trap logic, otherwise cleanup can happen too
#   early or twice.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

select_pc_test_json_paths() {
  # The CGO tests read demoTIL.json and demoLI.json from the repository root.
  # Therefore the insert/clean helper scripts in this step must target the same
  # files by default, while still honoring explicit outer overrides.
  #
  if [ "${TRICE_TIL_JSON+x}" = "x" ]; then
    pc_trice_til_json="$TRICE_TIL_JSON"
  else
    pc_trice_til_json="./demoTIL.json"
  fi

  if [ "${TRICE_LI_JSON+x}" = "x" ]; then
    pc_trice_li_json="$TRICE_LI_JSON"
  else
    pc_trice_li_json="./demoLI.json"
  fi
}

cleanup_step() {
  # cleanup_step is used for both paths:
  #
  # 1. Normal successful execution:
  #      insert -> tests -> cleanup_step 0
  #
  # 2. Abnormal execution:
  #      insert -> test failure / Ctrl-C / SIGTERM -> cleanup_step <status>
  #
  # The function always exits the script with the status passed as $1.
  # If no status was passed, it uses the previous command status ($?).
  #
  # This makes the function suitable for EXIT, INT and TERM traps, and also
  # allows calling it explicitly at the end of the normal test path.

  local status="${1:-$?}"
  local clean_status=0

  # Disable traps immediately.
  #
  # This prevents recursive cleanup if:
  # - cleanup itself triggers EXIT,
  # - the user presses Ctrl-C again while cleanup is already running,
  # - the clean helper exits with an error.
  trap - INT TERM EXIT

  # Run from repository root if possible. If this fails, continue cleanup anyway.
  # The "|| true" is intentional here: cleanup should make a best effort.
  cd "$ROOT" >/dev/null 2>&1 || true

  # Only run trice clean if trice insert has completed successfully before.
  #
  # This avoids running clean after an insert failure where the state is unclear.
  # Note: this cannot fully protect against Ctrl-C *inside* trice insert itself.
  # It only prevents unnecessary cleanup before a successful insert was observed
  # by this wrapper script.
  if [ "${ids_inserted:-0}" -eq 1 ]; then
    log "cleanup: running trice clean helper"

    TRICE_TIL_JSON="$pc_trice_til_json" \
      TRICE_LI_JSON="$pc_trice_li_json" \
      "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" >>"$LOGFILE" 2>&1
    clean_status=$?

    if [ "$clean_status" -ne 0 ]; then
      log "WARN: cleanup: trice clean helper failed with exit code $clean_status"

      # If the test script was otherwise successful, a cleanup failure should
      # make the whole script fail.
      #
      # If the script was already failing, keep the original failure status so
      # the first/root cause is not hidden by the cleanup failure.
      if [ "$status" -eq 0 ]; then
        status="$clean_status"
      fi
    fi

    # Mark sources as no longer intentionally inserted from this script's point
    # of view. If clean failed, the warning above remains in the log.
    ids_inserted=0
  fi

  # Restore C_INCLUDE_PATH if this script changed it.
  #
  # During CGO tests this script may temporarily clear C_INCLUDE_PATH because
  # external include paths can influence host-side compilation. If it was
  # changed, restore the exact previous value here.
  if [ "${saved_c_include_path+x}" = "x" ]; then
    export C_INCLUDE_PATH="$saved_c_include_path"
  else
    # If this script did not save a previous value, make sure no artificial
    # empty value remains from this script.
    unset C_INCLUDE_PATH || true
  fi

  exit "$status"
}

main() {
  local selected

  selected="$(get_mode "${1:-full}")"
  export SELECTED="$selected"

  select_pc_test_json_paths
  init_logfile

  # One central cleanup owner:
  #
  # - EXIT handles ordinary errors, for example a failed go test.
  # - INT handles Ctrl-C.
  # - TERM handles external termination, for example from CI or kill.
  #
  # Signal exit codes follow the common shell convention:
  # - 130 = interrupted by SIGINT
  # - 143 = terminated by SIGTERM
  trap 'cleanup_step $?' EXIT
  trap 'cleanup_step 130' INT
  trap 'cleanup_step 143' TERM

  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Go not installed"
    exit 0
  fi

  # Insert Trice IDs before running the host-side tests.
  #
  # The helper script is intentionally kept outside this file. This outer script
  # only tracks whether the helper completed successfully.
  run_cmd env \
    TRICE_TIL_JSON="$pc_trice_til_json" \
    TRICE_LI_JSON="$pc_trice_li_json" \
    "$ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || {
    log "FAIL: insert IDs failed"
    exit 1
  }

  # From this point on, cleanup should try to run the clean helper.
  ids_inserted=1

  # Some environments export C_INCLUDE_PATH globally. That can unintentionally
  # affect CGO compilation. Clear it temporarily, but remember the old value so
  # cleanup_step can restore it on every exit path.
  if [ "${C_INCLUDE_PATH:-}" != "" ]; then
    saved_c_include_path="$C_INCLUDE_PATH"
    log "WARN: Clearing C_INCLUDE_PATH temporarily for CGO tests"
    export C_INCLUDE_PATH=""
  fi

  cd "$ROOT/_test" || {
    log "FAIL: cannot enter _test directory"
    exit 1
  }

  if [ "$selected" = "quick" ]; then
    run_cmd go test ./be_dblB_de_tcobs_ua/... || {
      log "FAIL: quick PC target tests failed"
      exit 1
    }
  else
    run_cmd go test ./... || {
      log "FAIL: full PC target tests failed"
      exit 1
    }
  fi

  if grep_log '(^|[[:space:]])FAIL([[:space:]:]|$)' "$LOGFILE"; then
    log "FAIL: PC target test log contains FAIL markers"
    exit 2
  fi

  # Normal successful path:
  #
  # Use the same cleanup function as the traps. This avoids duplicated clean
  # logic and makes the normal path behave like the error/Ctrl-C paths.
  cleanup_step 0
}

# State variables used by cleanup_step.
#
# ids_inserted:
# - 0: Do not run trice clean.
# - 1: trice insert completed successfully; cleanup should run trice clean.
#
# saved_c_include_path:
# - intentionally not initialized here.
# - if it exists, cleanup_step restores it.
# - if it does not exist, cleanup_step unsets C_INCLUDE_PATH.
ids_inserted=0

main "$@"
