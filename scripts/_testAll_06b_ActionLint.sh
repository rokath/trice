#!/usr/bin/env bash
#
# Step 06b: Runs actionlint for GitHub workflow files.
#
# Direct invocation:
# - ./_testAll_06b_ActionLint.sh
#
# Log file:
# - ./temp/log/_testAll_06b_ActionLint.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  init_logfile

  if has_command actionlint; then
    run_cmd actionlint "$ROOT/.github/workflows/"*.yml || {
      log "FAIL: actionlint failed"
      exit 1
    }
    return 0
  fi

  log "SKIP: actionlint not installed locally; install it to run workflow lint"
  exit 0
}

main "$@"
