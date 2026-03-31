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
  init_logfile
  if ! has_command markdownlint; then
    log "MISSING TOOL: markdownlint"
    log "SKIP: markdownlint not installed"
    exit 0
  fi
  run_cmd markdownlint . || { log "FAIL: markdownlint failed"; exit 1; }
}

main "$@"
