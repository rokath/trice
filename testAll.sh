#!/usr/bin/env bash
#
# Public entry point for test orchestration.
#
# The historical interface intentionally remains unchanged:
# - ./testAll.sh
# - ./testAll.sh quick
# - ./testAll.sh full
#
# Selection:
# - quick
#   Standard run with the normal steps.
# - full
#   Runs the additional full PC target tests and the L432 configuration loop.
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
# - Each step writes one status line into ./testAll.log.
#
# Sleep prevention:
# - On Linux, systemd-inhibit is used when available.
# - On macOS, caffeinate is used when available.
# - Otherwise the internal runner is called directly.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR" || exit 1

run_with_inhibit_linux() {
  if command -v systemd-inhibit >/dev/null 2>&1; then
    systemd-inhibit --what=sleep:shutdown --who="$(whoami)" --why="Running testAll.sh" --mode=block \
      bash "./_testAll_run.sh" "$@"
  else
    bash "./_testAll_run.sh" "$@"
  fi
}

run_with_inhibit_macos() {
  if command -v caffeinate >/dev/null 2>&1; then
    caffeinate -dimsu bash "./_testAll_run.sh" "$@"
  else
    bash "./_testAll_run.sh" "$@"
  fi
}

run_without_inhibit() {
  bash "./_testAll_run.sh" "$@"
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
