#!/usr/bin/env bash
#
# Step 02: Formats the C sources.
#
# Direct invocation:
# - ./_testAll_02_ClangFormat.sh
#
# Log file:
# - ./temp/testAll/_testAll_02_ClangFormat.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  ensure_testall_dirs
  prepare_shared_env quick
  init_step_log "${BASH_SOURCE[0]}"
  log "Starting $(step_name_from_path "${BASH_SOURCE[0]}") at $(date)"
  run_cmd "$TESTALL_ROOT/clang-format.sh" || fail "clang-format.sh failed"
}

main "$@"
