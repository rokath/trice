#!/usr/bin/env bash
#
# Step 02b: Runs target code linting for selected Trice C library profiles.
#
# Direct invocation:
# - ./_testAll_02b_TargetCodeLinting.sh
#
# Log file:
# - ./temp/log/_testAll_02b_TargetCodeLinting.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"

main() {
  local verbose=0
  for arg in "$@"; do
    if [ "$arg" = "--verbose" ]; then
      verbose=1
    fi
  done

  init_logfile
  if ! has_command cppcheck && [ ! -x "/c/Program Files/Cppcheck/cppcheck.exe" ]; then
    log "MISSING TOOL: cppcheck"
    log "SKIP: cppcheck not installed"
    exit 0
  fi

  run_profile() {
    if [ "$verbose" -eq 1 ]; then
      log "$(printf 'Lint profile: buffer=%-6s output=%-8s endian=%-6s builtin=%s xtea=%s' "$2" "$4" "$6" "$8" "${10}")"
    fi
    "$SCRIPTS_DIR/_lint_c_code.sh" cppcheck "$@" >>"$LOGFILE" 2>&1 || {
      log "FAIL: _lint_c_code.sh cppcheck failed for profile: $*"
      exit 1
    }
  }

  run_profile --buffer ring --output deferred --endian little --builtin 0 --xtea 0
  run_profile --buffer ring --output deferred --endian big --builtin 0 --xtea 0
  run_profile --buffer double --output deferred --endian little --builtin 0 --xtea 0
  run_profile --buffer ring --output both --endian little --builtin 0 --xtea 0
  run_profile --buffer double --output both --endian big --builtin 0 --xtea 0
  run_profile --buffer stack --output direct --endian little --builtin 0 --xtea 0
  run_profile --buffer static --output direct --endian little --builtin 0 --xtea 0
  run_profile --buffer ring --output deferred --endian little --builtin 1 --xtea 0
  run_profile --buffer ring --output deferred --endian little --builtin 0 --xtea 1
  run_profile --buffer static --output direct --endian little --builtin 0 --xtea 1
}

main "$@"
