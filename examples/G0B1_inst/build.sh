#!/usr/bin/env bash
#
# Simple and CI-safe build script (pattern-preserving version).
#
# Main idea:
# - Always run relative to the script location (NOT the caller's working directory).
# - Use strict error handling, so CI fails immediately and clearly.
# - Run `trice clean` automatically after a successful `trice insert`, even if
#   the build fails or the user aborts the script with Ctrl-C.
#
# Cleanup / Ctrl-C behavior:
# - This script may modify source files indirectly by calling:
#
#       trice_insertIDs_in_examples_and_test_folder.sh
#
# - After that point, the repository should be cleaned again by calling:
#
#       trice_cleanIDs_in_examples_and_test_folder.sh
#
# - The cleanup is intentionally handled in this outer script, because this is
#   the level that knows the complete sequence:
#
#       pre-clean -> optional insert -> gcc build -> final clean
#                 -> optional TRICE_OFF verification build without IDs
#
# TRICE_OFF special case:
# - With TRICE_OFF=1, this script intentionally skips `trice insert`.
# - Therefore the first gcc build and the additional verification build both run
#   without inserted IDs, as before.

set -euo pipefail

# ------------------------------------------------------------------------------
# 1) Resolve stable directories
# ------------------------------------------------------------------------------

# Absolute path of the directory containing this build script.
# This works regardless of where the script is called from.
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Repository root, relative to this example directory.
#
# Expected layout:
#   <repo>/examples/<target>/build.sh
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

run_trice_clean_if_needed() {
  # This helper performs the actual cleanup work but does not exit the script.
  #
  # Return value:
  # - 0 if no cleanup was needed or cleanup succeeded.
  # - non-zero if cleanup was needed but failed.
  #
  # Keeping this separate from cleanup_and_exit allows the normal path to clean
  # after the first build and then continue with the additional TRICE_OFF check.

  local clean_status=0

  # Run the Trice helper from the repository root, but do not leave the caller
  # there afterwards.
  #
  # This is important because the normal script path continues after this helper
  # in the TRICE_OFF=1 case. The following verification build must still run in
  # SCRIPT_DIR, not in ROOT.
  #
  # Use a subshell for the directory change:
  #
  #   ( cd "${ROOT}" && command )
  #
  # The parent shell keeps its current working directory unchanged.
  #
  # Only run final clean after insert completed successfully.
  #
  # This avoids running clean after a failed or interrupted insert where this
  # wrapper script did not observe a completed insert step.
  #
  # It also avoids an unnecessary second clean in the TRICE_OFF=1 case, because
  # that case intentionally skips insert.
  #
  # Important limitation:
  # This Bash cleanup cannot protect against Ctrl-C exactly inside the trice
  # process while it is writing a file. That needs to be fixed inside trice
  # itself with atomic writes. This cleanup only tries to leave the repository
  # cleaned after the wrapper observed a successful insert.
  if [ "${ids_inserted}" -eq 1 ]; then
    echo "cleanup: running trice clean"

    # Temporarily disable `set -e` so we can capture and report the real exit
    # code of the clean helper ourselves.
    #
    # Do not write this as:
    #
    #   if ! command; then clean_status=$?; fi
    #
    # because inside the `then` branch `$?` would be the exit status of the
    # logical negation (`!`), not the original command status.
    set +e
    (
      cd "${ROOT}" || exit 1
      bash "${ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh"
    )
    clean_status=$?
    set -e

    if [ "${clean_status}" -ne 0 ]; then
      echo "warning: cleanup: trice clean failed with exit code ${clean_status}" >&2
      return "${clean_status}"
    fi

    ids_inserted=0
  fi

  return 0
}

cleanup_and_exit() {
  # Preserve the exit status that led to cleanup.
  #
  # Examples:
  # - 0   normal successful path
  # - 1   build or script error
  # - 130 Ctrl-C / SIGINT
  # - 143 SIGTERM
  local status="${1:-$?}"

  # Disable traps immediately.
  #
  # This prevents recursive cleanup if:
  # - cleanup itself causes EXIT,
  # - the user presses Ctrl-C again while cleanup is already running,
  # - the clean helper exits with an error.
  trap - INT TERM EXIT

  if ! run_trice_clean_if_needed; then
    # If the script was otherwise successful, a cleanup failure should make the
    # whole script fail. If the script was already failing, keep the original
    # status so the root cause is not hidden by the cleanup failure.
    if [ "${status}" -eq 0 ]; then
      status=1
    fi
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
trap 'cleanup_and_exit $?' EXIT
trap 'cleanup_and_exit 130' INT
trap 'cleanup_and_exit 143' TERM

# ------------------------------------------------------------------------------
# 3a) Optional TRICE_OFF / typeX0 compilation matrix
# ------------------------------------------------------------------------------

if [ "$#" -eq 1 ] && [ "$1" = "--x0-matrix" ]; then
  # The matrix deliberately reuses this script's normal one-build path. That
  # keeps ID insertion, cleanup, build environment loading and TRICE_OFF
  # verification behavior identical to the ordinary example build.
  for off_value in 0 1; do
    for x0_value in 0 1; do
      echo "x0-matrix: TRICE_OFF=${off_value} TRICE_TX_X0_COUNTED_BUFFER_SUPPORT=${x0_value}"
      "${SCRIPT_DIR}/build.sh" "TRICE_OFF=${off_value}" "TRICE_TX_X0_COUNTED_BUFFER_SUPPORT=${x0_value}"
    done
  done

  trap - INT TERM EXIT
  exit 0
fi

for arg in "$@"; do
  if [ "$arg" = "--x0-matrix" ]; then
    echo "usage: ./build.sh --x0-matrix" >&2
    echo "       or pass compiler defines without --x0-matrix" >&2
    exit 2
  fi
done

# ------------------------------------------------------------------------------
# 3) Build TRICE_FLAGS and detect TRICE_OFF
# ------------------------------------------------------------------------------

# Initialize an empty string for the compiler defines passed to make.
flags=""

# Track TRICE_OFF=1 explicitly. This controls whether IDs are inserted before
# the first build and whether the additional no-ID verification build runs.
triceOFF="0"

# Loop through all arguments.
#
# Keep the original pattern-preserving behavior:
#   TRICE_OFF=1
# becomes:
#   -DTRICE_OFF=1
#
# and arbitrary other arguments become -D... as before.
for arg in "$@"; do
  if [ "${arg}" = "TRICE_OFF=1" ]; then
    triceOFF="1"

    # Keep compatibility with existing scripts/env if anything downstream
    # expects this exported shell variable.
    export triceOFF="1"
  fi

  flags="${flags}-D${arg} "
done

# ------------------------------------------------------------------------------
# 4) Run TRICE scripts
# ------------------------------------------------------------------------------

# Always start from a cleaned state.
#
# This pre-clean also helps editors show the Trice IDs cleaned state before the
# build starts.
#
# This pre-clean is intentionally not guarded by ids_inserted. If it fails, the
# script should fail immediately because the starting state is not trustworthy.
cd "${ROOT}"
bash "${ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh"

if [ "${triceOFF}" != "1" ]; then
  # Insert IDs before the normal gcc build.
  #
  # Trice is called here and not within make, to guarantee it is finished before
  # any make job starts.
  #
  # custom aliases should be excluded or without IDs, when translating with
  # TRICE_OFF=1. Therefore insert is skipped in the TRICE_OFF=1 case.
  #
  # After this command succeeds, cleanup must run trice clean on every exit path.
  bash "${ROOT}/trice_insertIDs_in_examples_and_test_folder.sh"
  ids_inserted=1
fi

# ------------------------------------------------------------------------------
# 5) Load build environment and build
# ------------------------------------------------------------------------------

# shellcheck disable=SC1091
source "${ROOT}/scripts/_setup_build_environment.sh"

# Provide a default if _setup_build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

# Ensure we build from the example directory, where the Makefile is expected.
cd "${SCRIPT_DIR}"

# We translate here with inserted IDs in the normal case, and without inserted IDs
# in the TRICE_OFF=1 case. In both cases, we expect no warnings.
make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc

# ------------------------------------------------------------------------------
# 6) Final clean after the first build
# ------------------------------------------------------------------------------

# Use the same cleanup helper as the traps, but do not exit here because the
# TRICE_OFF=1 case still has an additional verification build below.
#
# If cleanup fails after an otherwise successful first build, fail immediately.
run_trice_clean_if_needed

# ------------------------------------------------------------------------------
# 7) Additional TRICE_OFF verification build without IDs
# ------------------------------------------------------------------------------

# Additionally translate the TRICE_OFF case without IDs and expect no warnings.
#
# In the TRICE_OFF=1 case no insert was performed above, so this build runs from
# the cleaned state. In the normal case this section is skipped.
if [ "${triceOFF}" = "1" ]; then
  # Be explicit: the cleanup helper above must not affect this directory, but
  # changing back here makes the intended location obvious and protects future
  # edits.
  cd "${SCRIPT_DIR}"

  make clean
  make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc
fi

# Normal successful end.
#
# Disable traps before exiting because cleanup has already been completed.
trap - INT TERM EXIT
exit 0
