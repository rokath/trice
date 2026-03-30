#!/usr/bin/env bash
#
# Public entry point for test orchestration.
#
# The historical interface intentionally remains unchanged:
# - ./testAll.sh
# - ./testAll.sh quick
# - ./testAll.sh config
# - ./testAll.sh full
#
# Selection:
# - quick
#   Standard run with the normal steps, but without the additional
#   L432 configuration loop.
# - config
#   Skips the Go and PC target tests, but still runs the preparation,
#   build and configuration checks including the L432 configuration step.
# - full
#   Runs all steps.
#
# Structure:
# - testAll.sh resolves the CLI mode and only calls the steps needed for that mode.
# - Individual step scripts only accept arguments when they actually need them.
# - Each step has its own name, its own log file, and can be run separately,
#   for example:
#   - ./_testAll_05_MarkdownLint.sh
#   - ./_testAll_06_LinkCheck.sh
#   - ./_testAll_10_PcTargetTests.sh full
#
# Logging:
# - Each step writes to ./temp/testAll/<scriptname>.log
# - testAll.sh assembles ./testAll.log from those files at the end.
#
# Sleep prevention:
# - On Linux, systemd-inhibit is used when available.
# - On macOS, caffeinate is used when available.
# - Otherwise the script runs directly.
# - --internal-run is only an internal wrapper argument.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
SCRIPT_PATH="$SCRIPT_DIR/$(basename -- "${BASH_SOURCE[0]}")"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

validate_selection() {
  case "${1:-quick}" in
    quick | config | full) return 0 ;;
    *)
      printf 'Unsupported test selection: %s\n' "${1:-}" >&2
      printf 'Usage: ./testAll.sh [quick|config|full]\n' >&2
      return 2
      ;;
  esac
}

run_stage() {
  local step_script="$1"
  shift
  "$SCRIPT_DIR/$step_script" "$@"
}

selected_steps() {
  cat <<'EOF'
_testAll_01_CleanDsStore.sh
_testAll_02_ClangFormat.sh
_testAll_03_BuildTriceTool.sh
_testAll_04_FormatManual.sh
_testAll_05_MarkdownLint.sh
_testAll_06_LinkCheck.sh
_testAll_07_GoCoverage.sh
_testAll_08_RuntimePrepare.sh
EOF
  case "$1" in
    quick)
      cat <<'EOF'
_testAll_09_GoTests.sh
_testAll_10_PcTargetTests.sh quick
EOF
      ;;
    config)
      :
      ;;
    full)
      cat <<'EOF'
_testAll_09_GoTests.sh
_testAll_10_PcTargetTests.sh full
EOF
      ;;
  esac
  cat <<'EOF'
_testAll_11_ClangTranslation.sh
_testAll_12_GccExampleBuilds.sh
EOF
  case "$1" in
    config | full)
      cat <<'EOF'
_testAll_13_L432Configs.sh
EOF
      ;;
  esac
}

my_long_task() {
  local selected="${1:-quick}"
  local t0
  local t1
  local runtime
  local rc=0
  local summary_log
  local step_line
  local step_script
  local step_args=()
  local log_files=()

  validate_selection "$selected" || return $?

  ensure_testall_dirs
  prepare_shared_env "$selected"

  summary_log="$(step_log_path_from_name "testAll_summary")"
  : >"$summary_log"

  t0=$(date +%s)
  printf 'Starting testAll.sh at %s\n' "$(date)" | tee -a "$summary_log"
  printf 'Selection: %s\n' "$selected" | tee -a "$summary_log"

  while IFS= read -r step_line; do
    [ -n "$step_line" ] || continue
    # Split the orchestrator line into script name plus optional script args.
    read -r -a step_args <<<"$step_line"
    step_script="${step_args[0]}"
    log_files+=("$(step_log_path_from_name "$(basename "$step_script" .sh)")")
    run_stage "$step_script" "${step_args[@]:1}" || {
      rc=$?
      break
    }
  done <<EOF
$(selected_steps "$selected")
EOF

  t1=$(date +%s)
  runtime=$((t1 - t0))
  if [ "$rc" -eq 0 ]; then
    printf 'PASS: testAll.sh completed successfully in %s seconds.\n' "$runtime" | tee -a "$summary_log"
  else
    printf 'FAIL: testAll.sh stopped with exit code %s after %s seconds.\n' "$rc" "$runtime" | tee -a "$summary_log"
  fi
  printf 'Finished at %s\n' "$(date)" | tee -a "$summary_log"

  combine_logs "$TESTALL_MAIN_LOG" "$summary_log" "${log_files[@]}"
  return "$rc"
}

run_with_inhibit_linux() {
  if command -v systemd-inhibit >/dev/null 2>&1; then
    systemd-inhibit --what=sleep:shutdown --who="$(whoami)" --why="Running testAll.sh" --mode=block \
      bash "$SCRIPT_PATH" --internal-run "$@"
  else
    my_long_task "$@"
  fi
}

run_with_inhibit_macos() {
  if command -v caffeinate >/dev/null 2>&1; then
    caffeinate -dimsu bash "$SCRIPT_PATH" --internal-run "$@"
  else
    my_long_task "$@"
  fi
}

run_without_inhibit() {
  my_long_task "$@"
}

main() {
  if [ "${1:-}" = "--internal-run" ]; then
    shift
    my_long_task "$@"
    exit $?
  fi

  case "$(uname -s)" in
    Linux*) run_with_inhibit_linux "$@" ;;
    Darwin*) run_with_inhibit_macos "$@" ;;
    *) run_without_inhibit "$@" ;;
  esac
}

main "$@"
