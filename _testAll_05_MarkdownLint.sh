#!/usr/bin/env bash
#
# Step 05: Runs markdownlint.
#
# Direct invocation:
# - ./_testAll_05_MarkdownLint.sh
# If markdownlint is not installed locally, the step is marked as SKIP
# instead of failing.
#
# Log file:
# - ./temp/testAll/_testAll_05_MarkdownLint.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  if ! has_command markdownlint; then
    skip "markdownlint not installed"
    exit 0
  fi
  run_cmd markdownlint . || fail "markdownlint failed"
}

main "$@"
