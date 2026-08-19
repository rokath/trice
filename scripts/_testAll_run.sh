#!/usr/bin/env bash
#
# Internal runner for testAll.sh.
#
# Direct invocation:
# - Not intended. Use ./scripts/testAll.sh instead.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$ROOT_DIR" || exit 1
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

if [ -t 1 ] && [ "${TERM:-}" != "dumb" ] && [ -z "${NO_COLOR:-}" ]; then
  PASS_COLOR=$'\033[32m'
  FAIL_COLOR=$'\033[31m'
  WARN_COLOR=$'\033[33m'
  RESET_COLOR=$'\033[0m'
else
  PASS_COLOR=''
  FAIL_COLOR=''
  WARN_COLOR=''
  RESET_COLOR=''
fi

summary_line() {
  printf '%s\n' "$1" >>"$SUMMARY_LOG"
  printf '%s\n' "$1"
}

run_step() {
  local s="$1"
  shift
  local rc=0
  local step_log
  local detail
  local warning
  printf '%s: ' "$s" >>"$SUMMARY_LOG"
  printf '%s: ' "$s"
  "$SCRIPT_DIR/$s" --quiet "$@" || rc=$?
  if [ "$rc" -eq 0 ]; then
    step_log="$LOG_DIR/$(basename "$s" .sh).log"
    if [ -f "$step_log" ] && grep -Eq '^(MISSING TOOL:|SKIP: .*not installed)' "$step_log"; then
      printf 'WARN\n' >>"$SUMMARY_LOG"
      printf '%sWARN%s\n' "$WARN_COLOR" "$RESET_COLOR"
      # Surface tool-related skip details that would otherwise only be visible
      # in the per-step log during a quiet aggregate test run.
      while IFS= read -r warning; do
        summary_line "  $warning"
      done < <(grep -E '^(MISSING TOOL:|SKIP: .*not installed)' "$step_log")
    else
      printf 'PASS\n' >>"$SUMMARY_LOG"
      printf '%sPASS%s\n' "$PASS_COLOR" "$RESET_COLOR"
      # Surface non-fatal compatibility information from quiet step logs so a
      # passing aggregate run still shows degraded local-tool behavior.
      if [ -f "$step_log" ]; then
        while IFS= read -r detail; do
          summary_line "  $detail"
        done < <(grep -E '^Hint:' "$step_log")
      fi
    fi
  else
    printf 'FAIL\n' >>"$SUMMARY_LOG"
    printf '%sFAIL%s\n' "$FAIL_COLOR" "$RESET_COLOR"
  fi
  return "$rc"
}

# parse_test_all_arguments accepts the historical selection plus the optional
# failure policy in either order. The default is deliberately fail-fast.
parse_test_all_arguments() {
  local argument
  local selection_seen=0

  TEST_ALL_SELECTED="quick"
  TEST_ALL_NO_STOP=0
  for argument in "$@"; do
    case "$argument" in
      quick | full)
        if [ "$selection_seen" -eq 1 ]; then
          printf 'Test selection specified more than once: %s\n' "$argument" >&2
          return 2
        fi
        TEST_ALL_SELECTED="$(get_mode "$argument")"
        selection_seen=1
        ;;
      --no-stop) TEST_ALL_NO_STOP=1 ;;
      *)
        printf 'Unsupported testAll argument: %s\n' "$argument" >&2
        printf 'Usage: ./scripts/testAll.sh [quick|full] [--no-stop]\n' >&2
        return 2
        ;;
    esac
  done
}

# run_step_with_policy records every failure. In the default mode it returns a
# failure immediately; --no-stop converts that control result to success so the
# caller can continue while retaining the failed final status.
run_step_with_policy() {
  if run_step "$@"; then
    return 0
  fi
  TEST_ALL_FAILED=1
  if [ "$TEST_ALL_NO_STOP" -eq 1 ]; then
    return 0
  fi
  return 1
}

# run_selected_steps keeps the suite order in one place and returns at the first
# failed step unless --no-stop was selected. Transactional step wrappers finish
# their own restoration before their failure reaches this function.
run_selected_steps() {
  run_step_with_policy "_testAll_00a_FormatShellScripts.sh" || return 1
  run_step_with_policy "_testAll_00b_checkShell.sh" || return 1
  run_step_with_policy "_testAll_01_CleanDsStore.sh" || return 1
  run_step_with_policy "_testAll_02_ClangFormat.sh" || return 1
  run_step_with_policy "_testAll_02b_TargetCodeLinting.sh" || return 1
  run_step_with_policy "_testAll_03_BuildTriceTool.sh" || return 1
  run_step_with_policy "_testAll_04_FormatManual.sh" || return 1
  run_step_with_policy "_testAll_05_MarkdownLint.sh" || return 1
  run_step_with_policy "_testAll_06_LinkCheck.sh" || return 1
  run_step_with_policy "_testAll_06a_GoReleaser.sh" || return 1
  run_step_with_policy "_testAll_06b_ActionLint.sh" || return 1
  run_step_with_policy "_testAll_07_GoCoverage.sh" || return 1
  run_step_with_policy "_testAll_08_RuntimePrepare.sh" || return 1
  run_step_with_policy "_testAll_09_GoTests.sh" || return 1
  run_step_with_policy "_testAll_09b_BindTests.sh" || return 1
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    run_step_with_policy "_testAll_09c_BindWorkflowTests.sh" || return 1
    run_step_with_policy "_testAll_10a_PcTargetTests_insert.sh" "$TEST_ALL_SELECTED" || return 1
  fi
  run_step_with_policy "_testAll_10b_PcTargetTests_bind.sh" "$TEST_ALL_SELECTED" || return 1
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    run_step_with_policy "_testAll_11a_ClangTranslation_insert.sh" || return 1
  fi
  run_step_with_policy "_testAll_11b_ClangTranslation_bind.sh" || return 1
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    run_step_with_policy "_testAll_12a_GccExampleBuilds_insert.sh" || return 1
    run_step_with_policy "_testAll_12c_GccExampleBuilds_off.sh" || return 1
  fi
  run_step_with_policy "_testAll_12b_GccExampleBuilds_bind.sh" "$TEST_ALL_SELECTED" || return 1
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    run_step_with_policy "_testAll_13_L432Configs.sh" || return 1
  fi
  run_step_with_policy "_testAll_14_GoReleaserSnapshot.sh" || return 1
}

main() {
  local selected
  local started_at
  local finished_at
  local duration
  # local initial_tracked_status
  # local final_tracked_status
  parse_test_all_arguments "$@" || exit $?
  selected="$TEST_ALL_SELECTED"
  TEST_ALL_FAILED=0
  export TRICE_TEST_NO_STOP="$TEST_ALL_NO_STOP"
  export SELECTED="$selected"
  export SUMMARY_LOG="$LOG_DIR/testAll_summary.log"
  started_at=$(date +%s)
  # initial_tracked_status="$(tracked_worktree_status)"

  : >"$SUMMARY_LOG"
  summary_line "Starting testAll at $(date)"
  summary_line "Selection: $selected"
  if [ "$selected" = "quick" ]; then
    summary_line "ID workflows: bind"
  else
    summary_line "ID workflows: bind plus legacy insert/clean"
  fi
  if [ "$TEST_ALL_NO_STOP" -eq 1 ]; then
    summary_line "Failure policy: continue after failures (--no-stop)"
  else
    summary_line "Failure policy: stop after the first failure"
  fi

  run_selected_steps || true

  # Temporarily disabled until the remaining testAll steps are fully read-only again.
  # final_tracked_status="$(tracked_worktree_status)"
  # if [ "$final_tracked_status" != "$initial_tracked_status" ]; then
  #   failed=1
  #   summary_line "Result detail: tracked worktree changed during testAll"
  #   if [ -n "$final_tracked_status" ]; then
  #     summary_line "Tracked status after run:"
  #     while IFS= read -r line; do
  #       [ -n "$line" ] && summary_line "  $line"
  #     done <<EOF
  # $final_tracked_status
  # EOF
  #   fi
  # fi

  finished_at=$(date +%s)
  duration=$((finished_at - started_at))
  if [ "$TEST_ALL_FAILED" -eq 0 ]; then
    summary_line "Result: PASS"
  else
    summary_line "Result: FAIL"
  fi
  summary_line "Duration: ${duration}s"
  summary_line "Finished at $(date)"

  exit "$TEST_ALL_FAILED"
}

main "$@"
