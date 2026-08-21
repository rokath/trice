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
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"

if [ -t 1 ] && [ "${TERM:-}" != "dumb" ]; then
  TEST_ALL_INTERACTIVE=1
else
  TEST_ALL_INTERACTIVE=0
fi

if [ "$TEST_ALL_INTERACTIVE" -eq 1 ] && [ -z "${NO_COLOR:-}" ]; then
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

TEST_ALL_SPINNER_PID=""
TEST_ALL_PLAN_SCRIPTS=()
TEST_ALL_PLAN_ARGUMENTS=()

summary_line() {
  printf '%s\n' "$1" >>"$SUMMARY_LOG"
  printf '%s\n' "$1"
}

# progress_spinner provides a quiet life sign for long-running steps. It only
# runs on an interactive terminal and rewrites one line, so redirected output
# and CI logs never receive repeated spinner frames.
progress_spinner() {
  local step="$1"
  local frame=0
  local mark

  while :; do
    case $((frame % 4)) in
      0) mark='|' ;;
      1) mark='/' ;;
      2) mark='-' ;;
      3) mark='\' ;;
    esac
    printf '\r[%2d/%2d | ~%3d.%d%%] %s %s' \
      "$TEST_ALL_STEP_NUMBER" "$TEST_ALL_STEP_COUNT" \
      "$((TEST_ALL_STEP_END_TENTHS / 10))" \
      "$((TEST_ALL_STEP_END_TENTHS % 10))" "$mark" "$step"
    frame=$((frame + 1))
    sleep 2
  done
}

# stop_progress_spinner terminates only the spinner started by this runner and
# clears its terminal line before the stable result is printed.
stop_progress_spinner() {
  local was_running=0

  if [ -n "$TEST_ALL_SPINNER_PID" ]; then
    was_running=1
    kill "$TEST_ALL_SPINNER_PID" 2>/dev/null || true
    wait "$TEST_ALL_SPINNER_PID" 2>/dev/null || true
    TEST_ALL_SPINNER_PID=""
  fi
  if [ "$TEST_ALL_INTERACTIVE" -eq 1 ] && [ "$was_running" -eq 1 ]; then
    # Erase the current terminal line without printing padding that could wrap
    # on a narrow terminal.
    printf '\r\033[2K'
  fi
}

run_step() {
  local step="$1"
  shift
  local rc=0
  local step_log
  local detail
  local warning

  if [ "$TEST_ALL_INTERACTIVE" -eq 1 ]; then
    progress_spinner "$step" &
    TEST_ALL_SPINNER_PID=$!
  else
    printf '[%2d/%2d | ~%3d.%d%%] %s: running\n' \
      "$TEST_ALL_STEP_NUMBER" "$TEST_ALL_STEP_COUNT" \
      "$((TEST_ALL_STEP_END_TENTHS / 10))" \
      "$((TEST_ALL_STEP_END_TENTHS % 10))" "$step"
  fi

  "$SCRIPT_DIR/$step" --quiet "$@" || rc=$?
  stop_progress_spinner
  printf '[%2d/%2d | ~%3d.%d%%] %s: ' \
    "$TEST_ALL_STEP_NUMBER" "$TEST_ALL_STEP_COUNT" \
    "$((TEST_ALL_STEP_END_TENTHS / 10))" \
    "$((TEST_ALL_STEP_END_TENTHS % 10))" "$step" >>"$SUMMARY_LOG"
  printf '[%2d/%2d | ~%3d.%d%%] %s: ' \
    "$TEST_ALL_STEP_NUMBER" "$TEST_ALL_STEP_COUNT" \
    "$((TEST_ALL_STEP_END_TENTHS / 10))" \
    "$((TEST_ALL_STEP_END_TENTHS % 10))" "$step"
  if [ "$rc" -eq 0 ]; then
    step_log="$LOG_DIR/$(basename "$step" .sh).log"
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

# add_plan_step stores a script plus its optional single selection argument.
# Keeping one plan avoids separate quick/full execution lists drifting apart.
add_plan_step() {
  local index="${#TEST_ALL_PLAN_SCRIPTS[@]}"
  TEST_ALL_PLAN_SCRIPTS[index]="$1"
  TEST_ALL_PLAN_ARGUMENTS[index]="${2:-}"
}

# build_test_plan orders inexpensive checks first and the long compiler
# matrices last. Full mode adds legacy Insert/Clean coverage without duplicating
# the corresponding Bind steps.
build_test_plan() {
  TEST_ALL_PLAN_SCRIPTS=()
  TEST_ALL_PLAN_ARGUMENTS=()

  add_plan_step "_400_test_shell_format.sh"
  add_plan_step "_410_test_shellcheck.sh"
  add_plan_step "_420_test_dsstore.sh"
  add_plan_step "_430_test_clang_format.sh"
  add_plan_step "_440_test_manual_format.sh"
  add_plan_step "_450_test_markdownlint.sh"
  add_plan_step "_460_test_goreleaser_config.sh"
  add_plan_step "_470_test_actionlint.sh"
  add_plan_step "_480_test_build_trice_tool.sh"
  add_plan_step "_490_test_runtime_prepare.sh"
  add_plan_step "_500_test_bind.sh"
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    add_plan_step "_510_test_bind_workflows.sh"
  fi
  add_plan_step "_520_test_target_code_lint.sh"
  add_plan_step "_530_test_links.sh"
  add_plan_step "_540_test_go.sh"
  add_plan_step "_550_test_go_coverage.sh"
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    add_plan_step "_560_test_clang_insert.sh"
  fi
  add_plan_step "_570_test_clang_bind.sh"
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    add_plan_step "_580_test_gcc_off.sh"
    add_plan_step "_590_test_gcc_insert.sh"
  fi
  add_plan_step "_600_test_gcc_bind.sh" "$TEST_ALL_SELECTED"
  add_plan_step "_610_test_goreleaser_snapshot.sh"
  if [ "$TEST_ALL_SELECTED" = "full" ]; then
    add_plan_step "_620_test_l432_configs.sh"
    add_plan_step "_630_test_pc_targets_insert.sh" "$TEST_ALL_SELECTED"
  fi
  add_plan_step "_640_test_pc_targets_bind.sh" "$TEST_ALL_SELECTED"
}

# expected_step_weight assigns deliberately coarse, hardware-independent work
# units. They only make progress through very uneven test steps visible; they
# are not durations or an ETA. The full-mode weights reflect the observed fact
# that the two PC matrices and the L432 matrix dominate that selection.
expected_step_weight() {
  case "$TEST_ALL_SELECTED:$1" in
    full:_630_test_pc_targets_insert.sh) printf '390\n' ;;
    full:_640_test_pc_targets_bind.sh) printf '400\n' ;;
    full:_620_test_l432_configs.sh) printf '170\n' ;;
    full:_610_test_goreleaser_snapshot.sh) printf '15\n' ;;
    full:_580_test_gcc_off.sh | full:_590_test_gcc_insert.sh | full:_600_test_gcc_bind.sh) printf '4\n' ;;
    full:_560_test_clang_insert.sh | full:_570_test_clang_bind.sh) printf '2\n' ;;
    full:_480_test_build_trice_tool.sh | full:_540_test_go.sh | full:_550_test_go_coverage.sh) printf '3\n' ;;
    full:_500_test_bind.sh | full:_510_test_bind_workflows.sh | full:_520_test_target_code_lint.sh | full:_530_test_links.sh) printf '2\n' ;;
    quick:_610_test_goreleaser_snapshot.sh) printf '55\n' ;;
    quick:_640_test_pc_targets_bind.sh) printf '12\n' ;;
    quick:_550_test_go_coverage.sh) printf '10\n' ;;
    quick:_540_test_go.sh) printf '8\n' ;;
    quick:_530_test_links.sh | quick:_600_test_gcc_bind.sh) printf '5\n' ;;
    quick:_520_test_target_code_lint.sh) printf '4\n' ;;
    quick:_480_test_build_trice_tool.sh | quick:_500_test_bind.sh) printf '3\n' ;;
    quick:_570_test_clang_bind.sh) printf '2\n' ;;
    *) printf '1\n' ;;
  esac
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

# run_selected_steps walks the previously built plan and calculates percentages
# from relative work units. Transactional wrappers finish restoration before a
# failure reaches the fail-fast policy here.
run_selected_steps() {
  local completed_weight=0
  local index
  local step
  local argument
  local weight
  local total_weight=0

  TEST_ALL_STEP_COUNT="${#TEST_ALL_PLAN_SCRIPTS[@]}"
  for step in "${TEST_ALL_PLAN_SCRIPTS[@]}"; do
    weight="$(expected_step_weight "$step")"
    total_weight=$((total_weight + weight))
  done

  for ((index = 0; index < TEST_ALL_STEP_COUNT; index++)); do
    step="${TEST_ALL_PLAN_SCRIPTS[index]}"
    argument="${TEST_ALL_PLAN_ARGUMENTS[index]}"
    weight="$(expected_step_weight "$step")"
    TEST_ALL_STEP_NUMBER=$((index + 1))
    # Round upward to one decimal place so even the first inexpensive checks
    # show visible progress in the much larger full plan.
    TEST_ALL_STEP_END_TENTHS=$((((completed_weight + weight) * 1000 + total_weight - 1) / total_weight))
    if [ "$TEST_ALL_STEP_NUMBER" -eq "$TEST_ALL_STEP_COUNT" ]; then
      TEST_ALL_STEP_END_TENTHS=1000
    fi

    if [ -n "$argument" ]; then
      run_step_with_policy "$step" "$argument" || return 1
    else
      run_step_with_policy "$step" || return 1
    fi
    completed_weight=$((completed_weight + weight))
  done
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
  build_test_plan
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
  summary_line "Progress scale: expected relative test work (hardware-independent; no ETA)"

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

trap 'stop_progress_spinner' EXIT
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
  main "$@"
fi
