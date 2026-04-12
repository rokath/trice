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
  RESET_COLOR=$'\033[0m'
else
  PASS_COLOR=''
  FAIL_COLOR=''
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
  printf '%s: ' "$s" >>"$SUMMARY_LOG"
  printf '%s: ' "$s"
  "$SCRIPT_DIR/$s" --quiet "$@" || rc=$?
  if [ "$rc" -eq 0 ]; then
    printf 'PASS\n' >>"$SUMMARY_LOG"
    printf '%sPASS%s\n' "$PASS_COLOR" "$RESET_COLOR"
  else
    printf 'FAIL\n' >>"$SUMMARY_LOG"
    printf '%sFAIL%s\n' "$FAIL_COLOR" "$RESET_COLOR"
  fi
  return "$rc"
}

main() {
  local selected
  local started_at
  local finished_at
  local duration
  local failed=0
  # local initial_tracked_status
  # local final_tracked_status
  selected="$(get_mode "${1:-quick}")"
  export SELECTED="$selected"
  export SUMMARY_LOG="$LOG_DIR/testAll_summary.log"
  started_at=$(date +%s)
  # initial_tracked_status="$(tracked_worktree_status)"

  : >"$SUMMARY_LOG"
  summary_line "Starting testAll at $(date)"
  summary_line "Selection: $selected"

  run_step "_testAll_01_CleanDsStore.sh" || failed=1
  run_step "_testAll_02_ClangFormat.sh" || failed=1
  run_step "_testAll_03_BuildTriceTool.sh" || failed=1
  run_step "_testAll_04_FormatManual.sh" || failed=1
  run_step "_testAll_05_MarkdownLint.sh" || failed=1
  run_step "_testAll_06_LinkCheck.sh" || failed=1
  run_step "_testAll_06a_GoReleaser.sh" || failed=1
  run_step "_testAll_07_GoCoverage.sh" || failed=1
  run_step "_testAll_08_RuntimePrepare.sh" || failed=1
  run_step "_testAll_09_GoTests.sh" || failed=1
  run_step "_testAll_10_PcTargetTests.sh" "$selected" || failed=1
  run_step "_testAll_11_ClangTranslation.sh" || failed=1
  run_step "_testAll_12_GccExampleBuilds.sh" || failed=1
  if [ "$selected" = "full" ]; then
    run_step "_testAll_13_L432Configs.sh" || failed=1
    run_step "_testAll_14_GoReleaserSnapshot.sh" || failed=1
  fi

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
  if [ "$failed" -eq 0 ]; then
    summary_line "Result: PASS"
  else
    summary_line "Result: FAIL"
  fi
  summary_line "Duration: ${duration}s"
  summary_line "Finished at $(date)"

  exit "$failed"
}

main "$@"
