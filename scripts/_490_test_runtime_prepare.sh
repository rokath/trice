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
    if ! has_command "$compiler"; then
      if [ "$SELECTED" = full ]; then
        case "$compiler" in
          gcc | g++ | clang | clang++)
            log "FAIL: full requires the native $compiler frontend; it is missing from PATH"
            failed=1
            ;;
        esac
      fi
      continue
    fi
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
    # On macOS, commands named gcc/g++ may actually be Apple Clang. Full mode
    # requires two real compiler families, not two names for the same frontend.
    if [ "$SELECTED" = full ]; then
      case "$compiler" in
        gcc | g++)
          source='#if !defined(__GNUC__) || defined(__clang__)
#error full requires genuine GCC, not a Clang compatibility alias
#endif
'"$source"
          ;;
        clang | clang++)
          source='#ifndef __clang__
#error full requires genuine Clang
#endif
'"$source"
          ;;
      esac
    fi
    log "INFO: checking host $language compiler: $(command -v "$compiler")"
    run_cmd "$compiler" --version </dev/null || true
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

# Unix package managers often expose genuine GCC only under versioned names.
# Discover those in PATH without assuming a Homebrew prefix or a GCC version;
# aliases stay checkout-local and are activated for child steps only after probes.
select_versioned_host_gcc() {
  local directory candidate companion macros
  local search_dirs=()
  IFS=: read -r -a search_dirs <<<"$PATH"
  for directory in "${search_dirs[@]}"; do
    [ -n "$directory" ] || directory=.
    for candidate in "$directory"/gcc-[0-9]*; do
      [ -x "$candidate" ] || continue
      companion="${candidate%/gcc-*}/g++-${candidate##*/gcc-}"
      [ -x "$companion" ] || continue
      macros="$("$candidate" -dM -E -x c - </dev/null 2>/dev/null)" || continue
      printf '%s\n' "$macros" | grep -q '^#define __GNUC__ ' || continue
      if printf '%s\n' "$macros" | grep -q '^#define __clang__ '; then
        continue
      fi
      directory="$(cd "$directory" && pwd)" || return 1
      mkdir -p ./temp/tools/host-gcc/bin || return 1
      rm -f ./temp/tools/host-gcc/.ready || return 1
      ln -sf "$directory/${candidate##*/}" ./temp/tools/host-gcc/bin/gcc || return 1
      ln -sf "$directory/${companion##*/}" ./temp/tools/host-gcc/bin/g++ || return 1
      PATH="$(pwd)/temp/tools/host-gcc/bin:$PATH"
      export PATH
      hash -r
      log "INFO: checking versioned GCC from $candidate and $companion"
      return 0
    done
  done
  return 1
}

# Preserve working installations. When preparation fails, a supported Windows
# host can use a complete verified bundle instead of mixing compiler runtimes.
# Other platforms retain explicit prerequisite failures; system package managers
# and SDK installers are never run implicitly or with elevated privileges.
prepare_host_compilers() {
  local previous_path="$PATH"
  if check_host_compilers; then
    return 0
  fi
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      select_windows_host_test_package || return 1
      rm -f "$TRICE_HOST_PACKAGE_DIR/.ready" || return 1
      if ! confirm_tool_install "native GCC 16.2.0 and Clang 23.1.3 with MinGW-w64 runtime (about 400 MB download)"; then
        log "FAIL: full compiler coverage is unavailable; local GCC/Clang setup was not approved"
        return 1
      fi
      if ! install_windows_host_test_tools >>"$LOGFILE" 2>&1; then
        log "FAIL: checkout-local GCC/Clang setup failed; inspect the download/extraction diagnostics in $LOGFILE"
        return 1
      fi
      PATH="$(cd "$TRICE_HOST_PACKAGE_BIN" && pwd):$PATH"
      export PATH
      hash -r
      if check_host_compilers; then
        touch "$TRICE_HOST_PACKAGE_DIR/.ready" || return 1
        log "Hint: using checkout-local GCC and Clang from $TRICE_HOST_PACKAGE_BIN; system settings are unchanged."
        return 0
      fi
      ;;
    *)
      if [ "$SELECTED" = full ] && select_versioned_host_gcc && check_host_compilers; then
        touch ./temp/tools/host-gcc/.ready || return 1
        log "Hint: using checkout-local aliases for versioned GCC; system settings are unchanged."
        return 0
      fi
      log "Hint: Linux needs native GCC/g++ and Clang with development headers; install the matching packages for your distribution."
      log "Hint: macOS needs Xcode Command Line Tools/SDK and genuine GCC (for example Homebrew gcc); put its bin directory in PATH."
      ;;
  esac
  PATH="$previous_path"
  export PATH
  hash -r
  log "FAIL: native GCC/Clang preparation is incomplete; no compiler tests were replaced or counted as passed"
  return 1
}

main() {
  init_logfile
  log_runtime_context
  prepare_host_compilers || return 1
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
