#!/usr/bin/env bash
#
# Shared GCC Example worker. The caller owns the ID state and restoration.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# fail_on_compiler_diagnostics preserves the existing warning/error policy for
# every workflow while the parent wrapper captures output in LOGFILE.
fail_on_compiler_diagnostics() {
  local stage="$1"

  if grep -Eiq '(warning|error)' "$LOGFILE"; then
    printf 'FAIL: %s reported warnings or errors\n' "$stage" >&2
    return 2
  fi
}

# clean_all_targets removes stale objects whose preprocessor result came from a
# different ID workflow or target configuration.
clean_all_targets() {
  printf '+ %s/examples/cleanAllTargets.sh\n' "$ROOT"
  "$ROOT/examples/cleanAllTargets.sh"
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
  # shellcheck source=./_setup_build_environment.sh
  source "$SCRIPT_DIR/_setup_build_environment.sh"
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
