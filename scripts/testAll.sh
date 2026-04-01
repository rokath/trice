#!/usr/bin/env bash
#
# Public entry point for test orchestration.
#
# The historical interface intentionally remains unchanged:
# - ./scripts/testAll.sh
# - ./scripts/testAll.sh quick
# - ./scripts/testAll.sh full
#
# Selection:
# - quick
#   Standard run with the normal steps.
# - full
#   Runs the additional full PC target tests and the L432 configuration loop.
#
# Structure:
# - scripts/testAll.sh resolves the CLI mode and only calls the steps needed for that mode.
# - Individual step scripts only accept arguments when they actually need them.
# - Each step has its own name, its own log file, and can be run separately,
#   for example:
#   - ./scripts/_testAll_05_MarkdownLint.sh
#   - ./scripts/_testAll_06_LinkCheck.sh
#   - ./scripts/_testAll_10_PcTargetTests.sh full
#
# Logging:
# - Each step writes to ./temp/log/<scriptname>.log
# - The runner writes a compact summary to ./temp/log/testAll_summary.log.
#
# Sleep prevention:
# - On Linux, systemd-inhibit is used when available.
# - On macOS, caffeinate is used when available.
# - Otherwise the internal runner is called directly.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
run_with_inhibit_linux() {
  local rc
  if command -v systemd-inhibit >/dev/null 2>&1; then
    systemd-inhibit --what=sleep:shutdown --who="$(whoami)" --why="Running testAll.sh" --mode=block \
      bash "$SCRIPT_DIR/_testAll_run.sh" "$@"
    rc=$?
  else
    bash "$SCRIPT_DIR/_testAll_run.sh" "$@"
    rc=$?
  fi
  return "$rc"
}

run_with_inhibit_macos() {
  local rc
  if command -v caffeinate >/dev/null 2>&1; then
    caffeinate -dimsu bash "$SCRIPT_DIR/_testAll_run.sh" "$@"
    rc=$?
  else
    bash "$SCRIPT_DIR/_testAll_run.sh" "$@"
    rc=$?
  fi
  return "$rc"
}

run_without_inhibit() {
  local rc
  bash "$SCRIPT_DIR/_testAll_run.sh" "$@"
  rc=$?
  return "$rc"
}

main() {
  case "$(uname -s)" in
    Linux*) run_with_inhibit_linux "$@" ;;
    Darwin*) run_with_inhibit_macos "$@" ;;
    # A Windows-specific inhibit function could use PowerShell plus the Win32
    # API SetThreadExecutionState to keep the system awake while
    # ./_testAll_run.sh is running. That is possible, but more environment-
    # specific and therefore currently not enabled here.
    MINGW* | MSYS* | CYGWIN*) run_without_inhibit "$@" ;;
    *) run_without_inhibit "$@" ;;
  esac
}

main "$@"
