#!/usr/bin/env bash
#
# Test 500: Checks the current Bind generator and generated C/C++ target code.
#
# Direct invocation:
# - ./scripts/_500_test_bind.sh
#
# Log file:
# - ./temp/log/_500_test_bind.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"

# has_bind_compilers reports whether both required language frontends are available.
has_bind_compilers() {
  local c_found=0
  local cpp_found=0
  local compiler
  for compiler in cc gcc clang; do
    if has_command "$compiler"; then
      c_found=1
      break
    fi
  done
  for compiler in c++ g++ clang++; do
    if has_command "$compiler"; then
      cpp_found=1
      break
    fi
  done
  [ "$c_found" -eq 1 ] && [ "$cpp_found" -eq 1 ]
}

main() {
  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Trice bind tests require Go"
    exit 0
  fi
  if ! has_bind_compilers; then
    log "MISSING TOOL: compatible C and C++ compilers"
    log "SKIP: Trice bind target integration requires GCC- or Clang-compatible frontends"
    exit 0
  fi

  run_cmd env TRICE_BIND_INTEGRATION=1 go test ./internal/id \
    -run '^TestBind(GeneratedTargetCompilesCAndCPP|CanonicalTriceCheckGeneratesCompleteSidecar|MVP2RebaseCompilesCAndCPP|MVP2CounterGuardsAndGeneratedInvariants|MVP2RebaseEmitsStableRuntimeIDs)$' \
    -count=1 || {
    log "FAIL: Trice bind integration failed; inspect the Go test diagnostics above"
    exit 1
  }

  log "PASS: Bind generation, C/C++ compilation, counter guards and runtime IDs"
}

main "$@"
