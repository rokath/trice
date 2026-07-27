#!/usr/bin/env bash
#
# Run ShellCheck with the same warning threshold as the GitHub Actions check.
#
# The tracked-file list keeps local results aligned with the files that can be
# part of a pull request. A missing ShellCheck installation is reported as a
# warning and skips this optional check without failing the complete test run.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local shell_files=()
  local file

  init_logfile

  if ! has_command shellcheck; then
    log "MISSING TOOL: shellcheck"
    log "SKIP: shellcheck not installed; install it to run shell linting"
    return 0
  fi

  # Use Git's tracked file list so generated and temporary scripts cannot
  # influence the result of a pre-PR check.
  while IFS= read -r file; do
    if [ -n "$file" ] && [ -f "$file" ]; then
      shell_files+=("$file")
    fi
  done < <(git ls-files '*.sh')

  if [ "${#shell_files[@]}" -eq 0 ]; then
    log "No tracked shell scripts found."
    return 0
  fi

  run_cmd shellcheck --severity=warning "${shell_files[@]}" || {
    log "FAIL: shellcheck reported warnings or errors"
    return 1
  }
}

main "$@"
