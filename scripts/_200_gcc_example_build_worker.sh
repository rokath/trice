#!/usr/bin/env bash
#
# Shared GCC Example worker. The caller owns the ID state and restoration.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# fail_on_compiler_diagnostics retains diagnostics without making a successful
# build fail because another compiler release adds a warning. Build failures
# propagate through the command status; arbitrary words in log text are not a
# reliable compiler error detector.
fail_on_compiler_diagnostics() {
  local stage="$1"

  if grep -Eiq 'warning:' "$LOGFILE"; then
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
    "G0B1_inst/build.sh"
    "L432_inst/build.sh"
  )

  for entry in "${entries[@]}"; do
    printf '+ standalone examples/%s\n' "$entry"
    entry_log="$LOGFILE.standalone.${entry%/*}"
    # Check each entry's own dependencies instead of assuming an ARM compiler
    # also supplies the host compiler and CMake used by the desktop examples.
    case "$entry" in
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
      continue
    else
      status=$?
    fi
    cat "$entry_log"
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
    (
      cd "$ROOT/examples/G0B1_inst" || exit 1
      ./build.sh
    )
    fail_on_compiler_diagnostics "G0B1_inst quick build"
    clean_all_targets
    return 0
  fi

  printf '+ examples/G0B1_inst/build.sh --x0-matrix\n'
  (
    cd "$ROOT/examples/G0B1_inst" || exit 1
    ./build.sh --x0-matrix
  )
  fail_on_compiler_diagnostics "G0B1_inst X0 matrix"

  clean_all_targets
  has_host_build_tools || return 0
  printf '+ examples/buildAllTargets_TRICE_ON.sh\n'
  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_ON.sh
  )
  fail_on_compiler_diagnostics "TRICE_ON builds"
  clean_all_targets
}

# run_off_matrix retains the existing workflow-independent TRICE_OFF target
# coverage exactly once in the aggregate test suite.
run_off_matrix() {
  has_host_build_tools || return 0
  clean_all_targets
  printf '+ examples/buildAllTargets_TRICE_OFF.sh\n'
  (
    cd "$ROOT/examples" || exit 1
    ./buildAllTargets_TRICE_OFF.sh
  )
  fail_on_compiler_diagnostics "TRICE_OFF builds"
  clean_all_targets
}

main() {
  local mode="${1:-}"
  local selected="${2:-full}"

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
    if ! run_standalone_bind_examples; then
      clean_all_targets
      return 2
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
