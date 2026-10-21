#!/usr/bin/env bash
#
# Test 520: Runs target code linting for selected Trice C library profiles.
#
# Direct invocation:
# - ./scripts/_520_test_target_code_lint.sh
#
# Log file:
# - ./temp/log/_520_test_target_code_lint.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"
source "$SCRIPT_DIR/_105_setup_test_tools.sh"

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
  if ! ensure_cppcheck_test_tool; then
    log "FAIL: required Cppcheck version 2.22.0 is unavailable"
    exit 1
  fi

  # Resolve the same cppcheck executable shape that _290_lint_c_code.sh accepts so
  # this step can print the compatibility hint once before spawning profiles.
  if has_command cppcheck; then
    cppcheck_bin="$(command -v cppcheck)"
  else
    cppcheck_bin="/c/Program Files/Cppcheck/cppcheck.exe"
  fi

  # Verify exhaustive-check support once, before launching the profile matrix.
  cppcheck_help="$("$cppcheck_bin" --help 2>&1 || true)"
  case "$cppcheck_help" in
    *--check-level*) ;;
    *)
      log "FAIL: Cppcheck 2.22.0 does not advertise --check-level; exhaustive checking cannot run"
      exit 1
      ;;
  esac

  run_profile() {
    if [ "$verbose" -eq 1 ]; then
      log "$(printf 'Lint profile: buffer=%-6s output=%-8s endian=%-6s builtin=%s xtea=%s' "$2" "$4" "$6" "$8" "${10}")"
    fi
    "$SCRIPTS_DIR/_290_lint_c_code.sh" cppcheck "$@" >>"$LOGFILE" 2>&1 || {
      log "FAIL: _290_lint_c_code.sh cppcheck failed for profile: $*"
      exit 1
    }
  }

  # The default matrix keeps broad coverage of buffer/output/endian combinations
  # without exploding into a full Cartesian product. XTEA is added only for two
  # representative cases:
  # - deferred + xtea=1 proves the deferred encryption path still parses cleanly
  # - direct   + xtea=1 triggers the extra compiler check in _290_lint_c_code.sh,
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
