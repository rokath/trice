#!/usr/bin/env bash
#
# Shared G0B1 Clang translation worker. The caller owns the ID workflow.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

main() {
  : "${LOGFILE:?LOGFILE must be exported by the Clang workflow wrapper}"
  # shellcheck source=./_150_setup_build_environment.sh
  source "$SCRIPT_DIR/_150_setup_build_environment.sh"
  cd "$ROOT/examples/G0B1_inst" || return 1
  printf '+ clang --version\n'
  clang --version
  printf '+ make clean\n'
  # These builds never read input. Do not pass a Git Bash terminal through native
  # Make to MSYS recipe shells: that combination can leave shell children stuck.
  # Keep the redirection local so outer tool-installation prompts still work.
  make clean </dev/null
  printf '+ ./build_with_clang.sh\n'
  ./build_with_clang.sh </dev/null

  # Warning sets vary by compiler release. Keep their text visible but use the
  # build command's failure status, not a search for words in its output.
  if grep -Eiq 'warning:' "$LOGFILE"; then
    printf 'Hint: clang translation completed with compiler warnings; see the diagnostics above.\n'
  fi
}

main "$@"
