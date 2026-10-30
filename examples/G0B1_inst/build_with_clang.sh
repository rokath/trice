#!/usr/bin/env bash
#
# Usage: ./build_with_clang.sh [0|98]. Build with Clang; do not flash the board.
# Requires Bash, make, Clang, Trice and Arm GNU libraries in PATH.
# A standalone run prepares Bind headers; managed tests may use Insert/Clean.
# Repository tests may already own that preparation. Cleanup preserves failures
# and also runs on Ctrl-C; the helper functions implement these safeguards.

set -euo pipefail

# Match build.sh: no argument keeps configuration 0, while explicit profiles and
# existing compiler defines are passed through TRICE_FLAGS. Parse before Bind so
# a typo cannot change source state. Installation help stays in the RM.
if [ "$#" -eq 1 ] && { [ "$1" = --help ] || [ "$1" = -h ]; }; then
  printf 'Usage: ./build_with_clang.sh [0|98|CONFIGURATION=0|CONFIGURATION=98] [compiler defines...]\n'
  printf 'See docs/rm/examples.md#g0b1-clock-benchmark (from the repository root).\n'
  exit 0
fi
flags=""
for arg in "$@"; do
  case "$arg" in
    0 | 98) arg="CONFIGURATION=$arg" ;;
    CONFIGURATION=0 | CONFIGURATION=98) ;;
    CONFIGURATION=* | [0-9]*)
      printf 'error: G0B1_inst supports configuration 0 or 98; received: %s\n' "$arg" >&2
      printf 'See docs/rm/examples.md#g0b1-clock-benchmark (from the repository root).\n' >&2
      exit 2
      ;;
  esac
  flags="${flags}-D${arg} "
done

# ------------------------------------------------------------------------------
# 1) Resolve stable directories
# ------------------------------------------------------------------------------

# Absolute path of the directory containing this script.
# This works regardless of where the script is called from.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Repository root, relative to this example directory.
#
# Expected layout:
#   <repo>/examples/<target>/build_with_clang.sh
#
# Therefore the repository root is two levels above this script.
ROOT="$(cd -- "${SCRIPT_DIR}/../.." && pwd)"

# ------------------------------------------------------------------------------
# 2) State used by cleanup
# ------------------------------------------------------------------------------

# ids_inserted:
# - 0: Do not run trice clean in cleanup.
# - 1: trice insert completed successfully; cleanup should run trice clean.
ids_inserted=0

cleanup() {
  # Preserve the exit status that led to cleanup.
  #
  # Examples:
  # - 0   normal successful path
  # - 1   build or script error
  # - 130 Ctrl-C / SIGINT
  # - 143 SIGTERM
  local status="${1:-$?}"
  local clean_status=0

  # Disable traps immediately.
  #
  # This prevents recursive cleanup if:
  # - cleanup itself causes EXIT,
  # - the user presses Ctrl-C again while cleanup is already running,
  # - the clean helper exits with an error.
  trap - INT TERM EXIT

  # Always run helper scripts from the repository root. This makes the behavior
  # independent of the current working directory at the time cleanup is triggered.
  cd "${ROOT}" >/dev/null 2>&1 || true

  # Only run final clean after insert completed successfully.
  #
  # This avoids running clean after a failed or interrupted insert where this
  # wrapper script did not observe a completed insert step.
  #
  # Important limitation:
  # This Bash cleanup cannot protect against Ctrl-C exactly inside the trice
  # process while it is writing a file. That needs to be fixed inside trice
  # itself with atomic writes. This cleanup only tries to leave the repository
  # cleaned after the wrapper observed a successful insert.
  if [ "${ids_inserted}" -eq 1 ]; then
    echo "cleanup: running trice clean"

    bash "${ROOT}/scripts/_240_legacy_clean_ids.sh" || clean_status=$?
    if [ "${clean_status}" -ne 0 ]; then
      echo "warning: cleanup: trice clean failed with exit code ${clean_status}" >&2

      # If the script was otherwise successful, a cleanup failure should make
      # the whole script fail.
      #
      # If the script was already failing, keep the original status so the root
      # cause is not hidden by the cleanup failure.
      if [ "${status}" -eq 0 ]; then
        status="${clean_status}"
      fi
    fi

    ids_inserted=0
  fi

  exit "${status}"
}

# One central cleanup owner:
#
# - EXIT handles ordinary errors, for example a failed make command.
# - INT handles Ctrl-C.
# - TERM handles external termination, for example from CI or kill.
#
# Signal exit codes follow the common shell convention:
# - 130 = interrupted by SIGINT
# - 143 = terminated by SIGTERM
trap 'cleanup $?' EXIT
trap 'cleanup 130' INT
trap 'cleanup 143' TERM

# ------------------------------------------------------------------------------
# 3) Run TRICE scripts
# ------------------------------------------------------------------------------

# Match the GCC entry point: standalone builds refresh Bind headers, while an
# outer Insert/Bind workflow retains ownership of its prepared source state.
# shellcheck source=../prepareTriceBind.sh
source "${ROOT}/examples/prepareTriceBind.sh"
prepare_trice_bind_build "${ROOT}"
if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" = "1" ]; then
  echo "Trice ID workflow owned by outer wrapper: ${TRICE_ID_WORKFLOW:-unknown}"
else
  cd "${ROOT}"
  bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
  bash "${ROOT}/scripts/_230_legacy_insert_ids.sh"
  ids_inserted=1
fi

# ------------------------------------------------------------------------------
# 4) Load build environment and build
# ------------------------------------------------------------------------------

# shellcheck disable=SC1091
source "${ROOT}/scripts/_150_setup_build_environment.sh"

# Provide a default if _150_setup_build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

# Ensure we build from the example directory, where the Makefile is expected.
cd "${SCRIPT_DIR}"

# Command-line defines passed through TRICE_FLAGS change the preprocessor result.
# Make does not automatically know that all affected objects must be rebuilt
# when only TRICE_FLAGS changes. Avoid linking stale objects from a previous
# matrix entry.
make clean

make ${MAKE_JOBS} TRICE_FLAGS="${flags}" clang

# ------------------------------------------------------------------------------
# 5) Post-build cleanup
# ------------------------------------------------------------------------------

# Use the same cleanup path as the traps. This avoids duplicate clean logic and
# keeps normal success, build failure and Ctrl-C behavior consistent.
#
# Standalone sources stay bound; an outer wrapper restores its own initial state.
cleanup 0
