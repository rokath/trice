#!/usr/bin/env bash

# Run the same real PC/CGO configurations with the second native compiler.
# Separate child logs preserve GCC evidence instead of overwriting step 640.
set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"

main() {
  local selected status=0 bind_status=0
  selected="$(get_mode "${1:-full}")"
  init_logfile
  mkdir -p "$LOG_DIR/clang" || return 2
  if [ "$selected" = full ]; then
    run_cmd env CC=clang CXX=clang++ LOG_DIR="$LOG_DIR/clang" \
      bash "$SCRIPT_DIR/_630_test_pc_targets_insert.sh" full || status=$?
    # Cancellation always ends the group. A direct invocation also keeps the
    # workers' stop-on-first-failure default unless continuation was requested.
    case "$status" in 130 | 143) return "$status" ;; esac
    if [ "$status" -ne 0 ] && [ "${TRICE_TEST_NO_STOP:-0}" -eq 0 ]; then return "$status"; fi
  fi
  run_cmd env CC=clang CXX=clang++ LOG_DIR="$LOG_DIR/clang" \
    bash "$SCRIPT_DIR/_640_test_pc_targets_bind.sh" "$selected" || bind_status=$?
  case "$bind_status" in 130 | 143) return "$bind_status" ;; esac
  # Later failures cannot turn an incomplete first workflow into a complete run.
  if [ "$status" -eq 2 ] || [ "$bind_status" -eq 2 ]; then return 2; fi
  [ "$status" -eq 0 ] || return "$status"
  return "$bind_status"
}

main "$@"
