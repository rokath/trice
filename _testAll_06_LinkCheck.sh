#!/usr/bin/env bash
#
# Step 06: Runs lychee for link checking.
#
# Direct invocation:
# - ./_testAll_06_LinkCheck.sh
# If lychee is not installed locally, the step is marked as SKIP
# instead of failing.
#
# Log file:
# - ./temp/testAll/_testAll_06_LinkCheck.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  if ! has_command lychee; then
    skip "lychee not installed"
    exit 0
  fi
  run_cmd lychee . || fail "lychee failed"
}

main "$@"
