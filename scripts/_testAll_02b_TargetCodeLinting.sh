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
  local cppcheck_bin
  local cppcheck_help
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

  # Resolve the same cppcheck executable shape that _lint_c_code.sh accepts so
  # this step can print the compatibility hint once before spawning profiles.
  if has_command cppcheck; then
    cppcheck_bin="$(command -v cppcheck)"
  else
    cppcheck_bin="/c/Program Files/Cppcheck/cppcheck.exe"
  fi

  # Older cppcheck releases lack --check-level. The helper keeps running without
  # it; this exported flag prevents one duplicate hint per lint profile. Avoid
  # a short-circuiting grep pipeline so an inherited pipefail cannot invert the
  # feature result on Linux, macOS, or Windows Bash environments.
  cppcheck_help="$("$cppcheck_bin" --help 2>&1 || true)"
  case "$cppcheck_help" in
    *--check-level*) ;;
    *)
      log "Hint: cppcheck does not support --check-level; running without exhaustive checking."
      export TRICE_CPPCHECK_CHECK_LEVEL_HINT_SHOWN=1
      ;;
  esac

  run_profile() {
    if [ "$verbose" -eq 1 ]; then
      log "$(printf 'Lint profile: buffer=%-6s output=%-8s endian=%-6s builtin=%s xtea=%s' "$2" "$4" "$6" "$8" "${10}")"
    fi
    "$SCRIPTS_DIR/_lint_c_code.sh" cppcheck "$@" >>"$LOGFILE" 2>&1 || {
      log "FAIL: _lint_c_code.sh cppcheck failed for profile: $*"
      exit 1
    }
  }

  # The default matrix keeps broad coverage of buffer/output/endian combinations
  # without exploding into a full Cartesian product. XTEA is added only for two
  # representative cases:
  # - deferred + xtea=1 proves the deferred encryption path still parses cleanly
  # - direct   + xtea=1 triggers the extra compiler check in _lint_c_code.sh,
  #   which protects the in-place direct XTEA writer against accidental const
  #   changes that cppcheck alone would miss
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
