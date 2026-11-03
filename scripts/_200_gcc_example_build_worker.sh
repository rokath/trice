#!/usr/bin/env bash
#
# Shared GCC Example worker. The caller owns the ID state and restoration.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# compiler_warnings recognizes source locations and compiler/linker drivers,
# including MSVC locations. Strip ANSI colors before matching; application
# messages such as WARNING:11+i:red are not compiler diagnostics.
compiler_warnings() {
  LC_ALL=C sed -E $'s/\033\\[[0-9;]*[[:alpha:]]//g' "$1" |
    LC_ALL=C grep -E ':[0-9]+(:[0-9]+)?:[[:space:]]+warning:|\([0-9]+(,[0-9]+)?\)[[:space:]]*:[[:space:]]*warning[[:space:]]+[A-Z]+[0-9]+:|(^|.*[/\\])([^/\\[:space:]]*(gcc|g\+\+|clang|clang\+\+|cc1|cc1plus|collect2|ld|ld64)|cc|c\+\+)(-[0-9.]+)?(\.exe)?:[[:space:]]+warning:'
}

# run_compiler_stage keeps each build's evidence separate from earlier output.
# pipefail preserves build/cancellation and tee failures while output stays live.
run_compiler_stage() {
  local stage="$1" key="$2" directory="$3"
  local stage_log="$LOGFILE.build.$key"
  local status
  shift 3
  if (cd "$directory" && "$@") 2>&1 | tee "$stage_log"; then
    :
  else
    status=$?
    return "$status"
  fi
  if compiler_warnings "$stage_log" >/dev/null; then
    printf 'Hint: %s completed with compiler warnings; see the diagnostics above.\n' "$stage"
  fi
}

# clean_all_targets removes stale objects whose preprocessor result came from a
# different ID workflow or target configuration.
clean_all_targets() {
  printf '+ %s/examples/cleanAllTargets.sh\n' "$ROOT"
  "$ROOT/examples/cleanAllTargets.sh"
}

# has_host_build_tools checks the desktop subset separately from the ARM
# toolchain. A partial installation must not turn a missing optional build into
# a compiler failure. Actual CMake configure/build errors remain fatal.
has_host_build_tools() {
  if ! command -v cmake >/dev/null 2>&1; then
    printf 'SKIP: desktop example builds require cmake, which is not installed.\n'
    return 1
  fi
  if ! command -v cc >/dev/null 2>&1 && ! command -v gcc >/dev/null 2>&1 && ! command -v cl >/dev/null 2>&1; then
    printf 'SKIP: desktop example builds require a host C compiler (cc, gcc or cl), which is not installed.\n'
    return 1
  fi
}

# run_standalone_bind_examples validates every public example entry point whose
# checked-in sources include generated Bind headers. Scrubbing the outer
# workflow variables reproduces a direct invocation from a fresh shell.
run_standalone_bind_examples() {
  local entry
  local entry_log
  local failed=0
  local status
  local entries=(
    "DemoData_Trice/build.sh"
    "PC_log/build_and_run.sh"
    "G0B1_log/build.sh"
    "F030_inst/build.sh"
    "F030_inst_rtt/build.sh"
    "F030_inst_uart/build.sh"
    "G0B1_inst/build.sh"
    "G0B1_inst/build_with_clang.sh"
    "L432_inst/build.sh"
    "L432_inst/build_with_clang.sh"
  )

  for entry in "${entries[@]}"; do
    printf '+ standalone examples/%s\n' "$entry"
    entry_log="$LOGFILE.standalone.${entry%/*}.${entry##*/}"
    # Check each entry's own dependencies instead of assuming an ARM compiler
    # also supplies the host compiler and CMake used by the desktop examples.
    case "$entry" in
      */build_with_clang.sh)
        if ! command -v clang >/dev/null 2>&1; then
          printf 'SKIP: standalone %s: clang not installed\n' "$entry"
          continue
        fi
        ;;
      DemoData_Trice/*)
        has_host_build_tools || continue
        ;;
    esac
    case "$entry" in
      PC_log/*)
        if ! command -v cc >/dev/null 2>&1 && ! command -v gcc >/dev/null 2>&1; then
          printf 'SKIP: standalone %s: host C compiler (cc or gcc) not installed\n' "$entry"
          continue
        fi
        ;;
    esac
    if (
      unset C_INCLUDE_PATH CPLUS_INCLUDE_PATH CPATH OBJC_INCLUDE_PATH
      unset TRICE_BIND_INCLUDE_DIR TRICE_ID_WORKFLOW_OWNER TRICE_ID_WORKFLOW
      cd "$ROOT/examples/${entry%/*}" || exit 1
      "./${entry##*/}"
    ) >"$entry_log" 2>&1; then
      # Successful standalone runs still need to expose dependency information
      # in the parent log, particularly why PC_log does not need SEGGER tools.
      grep -E '^(INFO:|SKIP:|Hint:)' "$entry_log" || true
      # Standalone compiler output lives in a separate file. Forward warnings
      # too so a successful nested build cannot hide them from testAll's summary.
      if compiler_warnings "$entry_log" >/dev/null; then
        printf 'Hint: standalone examples/%s completed with compiler warnings; see %s\n' "$entry" "$entry_log"
        compiler_warnings "$entry_log" || true
      fi
      continue
    else
      status=$?
    fi
    cat "$entry_log"
    # Cancellation stops the complete matrix, even with continue-on-failure.
    case "$status" in
      130 | 143) return "$status" ;;
    esac
    printf 'FAIL: standalone examples/%s exited with status %d\n' "$entry" "$status" >&2
    failed=1
  done

  if [ "$failed" -ne 0 ]; then
    return 2
  fi
}

# run_on_matrix executes one representative Bound GCC build in quick mode. Full
# mode retains the historical G0B1 X0 matrix and complete TRICE_ON target list.
# ID preparation remains owned by the calling workflow wrapper in both modes.
run_on_matrix() {
  local selected="$1"

  clean_all_targets
  if [ "$selected" = "quick" ]; then
    printf '+ examples/G0B1_inst/build.sh\n'
    run_compiler_stage "G0B1_inst quick build" quick "$ROOT/examples/G0B1_inst" ./build.sh
    clean_all_targets
    return 0
  fi

  printf '+ examples/G0B1_inst/build.sh --x0-matrix\n'
  run_compiler_stage "G0B1_inst X0 matrix" x0 "$ROOT/examples/G0B1_inst" ./build.sh --x0-matrix

  clean_all_targets
  has_host_build_tools || return 0
  printf '+ examples/buildAllTargets_TRICE_ON.sh\n'
  run_compiler_stage "TRICE_ON builds" on "$ROOT/examples" ./buildAllTargets_TRICE_ON.sh
  clean_all_targets
}

# run_off_matrix retains the existing workflow-independent TRICE_OFF target
# coverage exactly once in the aggregate test suite.
run_off_matrix() {
  has_host_build_tools || return 0
  clean_all_targets
  printf '+ examples/buildAllTargets_TRICE_OFF.sh\n'
  run_compiler_stage "TRICE_OFF builds" off "$ROOT/examples" ./buildAllTargets_TRICE_OFF.sh
  clean_all_targets
}

main() {
  local mode="${1:-}"
  local selected="${2:-full}"
  local status

  : "${LOGFILE:?LOGFILE must be exported by the GCC workflow wrapper}"
  case "$selected" in
    quick | full) ;;
    *)
      printf 'Unsupported GCC Example selection: %s\n' "$selected" >&2
      return 2
      ;;
  esac
  # Standalone entry points deliberately regenerate Bind headers. They cannot
  # run while the outer transaction owns Inserted sources with numeric IDs.
  if [ "$mode" = "on" ] && [ "$selected" = "full" ] && [ "${TRICE_ID_WORKFLOW:-}" = "bind" ]; then
    clean_all_targets
    if run_standalone_bind_examples; then
      :
    else
      status=$?
      clean_all_targets
      return "$status"
    fi
    clean_all_targets
  fi

  # shellcheck source=./_150_setup_build_environment.sh
  source "$SCRIPT_DIR/_150_setup_build_environment.sh"
  printf '+ arm-none-eabi-gcc --version\n'
  arm-none-eabi-gcc --version
  printf '+ command -v arm-none-eabi-gcc\n'
  command -v arm-none-eabi-gcc

  case "$mode" in
    on) run_on_matrix "$selected" ;;
    off) run_off_matrix ;;
    *)
      printf 'Unsupported GCC Example worker mode: %s\n' "$mode" >&2
      return 2
      ;;
  esac
}

main "$@"
