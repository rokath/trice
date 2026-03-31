#!/usr/bin/env bash
#
# Internal runner for testAll.sh.
#
# Direct invocation:
# - Not intended. Use ./testAll.sh instead.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

run_step() {
  local s="$1"
  shift
  local rc=0
  printf '%s: ' "$s" >>"$SUMMARY_LOG"
  "./$s" "$@" || rc=$?
  if [ "$rc" -eq 0 ]; then
    printf 'PASS\n' >>"$SUMMARY_LOG"
  else
    printf 'FAIL\n' >>"$SUMMARY_LOG"
  fi
  return "$rc"
}

main() {
  local selected
  local started_at
  local finished_at
  local duration
  local failed=0
  selected="$(get_mode "${1:-quick}")"
  export SELECTED="$selected"
  export SUMMARY_LOG="$LOG_DIR/testAll_summary.log"
  started_at=$(date +%s)

  : >"$SUMMARY_LOG"
  printf 'Starting testAll at %s\n' "$(date)" >>"$SUMMARY_LOG"
  printf 'Selection: %s\n' "$selected" >>"$SUMMARY_LOG"

  run_step "_testAll_01_CleanDsStore.sh" || failed=1
  run_step "_testAll_02_ClangFormat.sh" || failed=1
  run_step "_testAll_03_BuildTriceTool.sh" || failed=1
  run_step "_testAll_04_FormatManual.sh" || failed=1
  run_step "_testAll_05_MarkdownLint.sh" || failed=1
  run_step "_testAll_06_LinkCheck.sh" || failed=1
  run_step "_testAll_07_GoCoverage.sh" || failed=1
  run_step "_testAll_08_RuntimePrepare.sh" || failed=1
  run_step "_testAll_09_GoTests.sh" || failed=1
  run_step "_testAll_10_PcTargetTests.sh" "$selected" || failed=1
  run_step "_testAll_11_ClangTranslation.sh" || failed=1
  run_step "_testAll_12_GccExampleBuilds.sh" || failed=1
  if [ "$selected" = "full" ]; then
    run_step "_testAll_13_L432Configs.sh" || failed=1
  fi

  finished_at=$(date +%s)
  duration=$((finished_at - started_at))
  if [ "$failed" -eq 0 ]; then
    printf 'Result: PASS\n' >>"$SUMMARY_LOG"
  else
    printf 'Result: FAIL\n' >>"$SUMMARY_LOG"
  fi
  printf 'Duration: %ss\n' "$duration" >>"$SUMMARY_LOG"
  printf 'Finished at %s\n' "$(date)" >>"$SUMMARY_LOG"

  exit "$failed"
}

main "$@"
