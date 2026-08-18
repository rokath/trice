#!/usr/bin/env bash
#
# Shared G0B1 Clang translation worker. The caller owns the ID workflow.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

main() {
  : "${LOGFILE:?LOGFILE must be exported by the Clang workflow wrapper}"
  # shellcheck source=./_setup_build_environment.sh
  source "$SCRIPT_DIR/_setup_build_environment.sh"
  cd "$ROOT/examples/G0B1_inst" || return 1
  printf '+ clang --version\n'
  clang --version
  printf '+ make clean\n'
  make clean
  printf '+ ./build_with_clang.sh\n'
  ./build_with_clang.sh

  if grep -Eiq '(warning|error)' "$LOGFILE"; then
    printf 'FAIL: clang translation reported warnings or errors\n' >&2
    return 2
  fi
}

main "$@"
