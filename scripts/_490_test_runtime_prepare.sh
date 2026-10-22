#!/usr/bin/env bash
#
# Test 490: Checks host compilers, logs the environment and validates Bind preparation.
#
# Direct invocation:
# - ./scripts/_490_test_runtime_prepare.sh
#
# Log file:
# - ./temp/log/_490_test_runtime_prepare.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_100_test_common.sh"
# shellcheck source=./_140_trice_test_state.sh
source "$SCRIPT_DIR/_140_trice_test_state.sh"

log_runtime_context() {
  # Write the most relevant environment settings into this step log to make
  # later troubleshooting easier.
  log "SELECTED: ${SELECTED:-unknown}"
  log "OSTYPE: ${OSTYPE:-unknown}"
  log "GOCACHE: ${GOCACHE:-unset}"
  log "TRICE_TMP_DIR: ${TRICE_TMP_DIR:-unset}"
  log "TRICE_BIN_DIR: ${TRICE_BIN_DIR:-unset}"
  log "TRICE_TIL_JSON: ${TRICE_TIL_JSON:-unset}"
  log "TRICE_LI_JSON: ${TRICE_LI_JSON:-unset}"
  if has_command uname; then
    run_cmd uname -a || true
  fi
  if has_command go; then
    run_cmd go version || true
  fi
}

# Exercise the standard headers, linker and executable runtime of every visible
# frontend used by the Go integration tests. Presence in PATH alone is insufficient,
# particularly when clang and clang++ select different Windows runtime families.
check_host_compilers() {
  local probe_dir compiler language standard source failed=0
  probe_dir="$(mktemp -d "$LOG_DIR/host-compilers.XXXXXX")" || {
    log "FAIL: cannot create a host-compiler probe directory in $LOG_DIR"
    return 1
  }
  for compiler in cc gcc clang c++ g++ clang++; do
    has_command "$compiler" || continue
    case "$compiler" in
      c++ | g++ | clang++)
        language=c++
        standard=c++11
        source='#include <string.h>
#include <string>
int main() { return std::string("trice").size() == strlen("trice") ? 0 : 1; }'
        ;;
      *)
        language=c
        standard=c11
        source='#include <string.h>
int main(void) { return strlen("trice") == 5 ? 0 : 1; }'
        ;;
    esac
    log "INFO: checking host $language compiler: $(command -v "$compiler")"
    run_cmd "$compiler" --version || true
    if ! printf '%s\n' "$source" | run_cmd "$compiler" "-std=$standard" -O0 -Wall -Wextra -Werror \
      -x "$language" - -o "$probe_dir/host.exe"; then
      log "FAIL: $compiler cannot compile and link a native $language program with standard headers and libraries"
      failed=1
    elif ! run_cmd "$probe_dir/host.exe"; then
      log "FAIL: $compiler produced a host executable that cannot run successfully"
      failed=1
    fi
    # Remove only this probe's output; other diagnostic artifacts can remain.
    rm -f -- "$probe_dir/host.exe"
  done
  rmdir "$probe_dir" 2>/dev/null || true
  if [ "$failed" -ne 0 ]; then
    log "FAIL: repair the host compiler installation before rerunning compiler-dependent tests"
    log "Hint: use a complete native GCC/Clang toolchain with matching C/C++ headers and libraries, and put its bin directory first in PATH."
    log "Hint: on Windows, check both clang and clang++; a GNU target needs a compatible MinGW-w64 runtime, an MSVC target needs Visual Studio C++ Build Tools and a Windows SDK."
    log "Hint: see docs/rm/development_environment_setup.md (Choose the right Windows compiler)."
    return 1
  fi
}

main() {
  init_logfile
  log_runtime_context
  check_host_compilers || return 1
  if has_command trice; then
    run_cmd which trice || true
    run_cmd trice version -v || {
      log "FAIL: trice version failed"
      exit 1
    }
  else
    log "MISSING TOOL: trice"
    log "SKIP: trice not installed or built; canonical Bind preparation requires it."
    return 0
  fi

  # Verify canonical Bind preparation inside the same snapshot used by later
  # workflow steps. Restoring here preserves the caller's original tables and
  # generated files; later steps prepare their own required ID state.
  trice_test_run_managed_workflow bind none true || {
    log "FAIL: preparing canonical Bind state failed"
    exit 1
  }
}

main "$@"
