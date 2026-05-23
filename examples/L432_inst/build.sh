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
# - This script modifies source files indirectly by calling:
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
#       pre-clean -> insert -> gcc build -> final clean
#
# - The cleanup function is used for both normal success and abnormal exits.
#
# About the make exit code:
# - The original script wanted to preserve make's exit code exactly.
# - With `set -e`, a failing `make` would normally abort the script immediately,
#   before EXITCODE=$? is reached.
# - Therefore this version temporarily disables `set -e` only around `make`,
#   captures the make exit code explicitly, runs cleanup, and exits with the
#   captured make exit code unless cleanup itself fails after an otherwise
#   successful build.

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
  # Keeping this separate from cleanup_and_exit allows the normal path to:
  # - capture make's exit code,
  # - run cleanup,
  # - then exit with the original make exit code.

  local clean_status=0

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

    if ! bash "${ROOT}/trice_cleanIDs_in_examples_and_test_folder.sh"; then
      clean_status=$?
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
# - EXIT handles ordinary errors, for example errors before or after make.
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
# 3) Build TRICE_FLAGS
# ------------------------------------------------------------------------------

# Initialize an empty string for the compiler defines passed to make.
flags=""

# Loop through all arguments.
#
# Keep the original pattern-preserving behavior:
#   SOME_FLAG
# becomes:
#   -DSOME_FLAG
for arg in "$@"; do
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

# Insert IDs before the gcc build.
#
# Trice is called here and not within make, to guarantee it is finished before
# any make job starts.
#
# After this command succeeds, cleanup must run trice clean on every exit path.
bash "${ROOT}/trice_insertIDs_in_examples_and_test_folder.sh"
ids_inserted=1

# ------------------------------------------------------------------------------
# 5) Load build environment and build
# ------------------------------------------------------------------------------

# shellcheck disable=SC1091
source "${ROOT}/scripts/_setup_build_environment.sh"

# Provide a default if _setup_build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

# Ensure we build from the example directory, where the Makefile is expected.
cd "${SCRIPT_DIR}"

# Preserve make's exit code explicitly.
#
# We temporarily disable `set -e` only around make. Otherwise a failing make
# would abort the script immediately and the next line could not capture `$?`.
set +e
make ${MAKE_JOBS} TRICE_FLAGS="${flags}" gcc
make_status=$?
set -e

# ------------------------------------------------------------------------------
# 6) Post-build cleanup and exit
# ------------------------------------------------------------------------------

# Use the same cleanup helper as the traps. This avoids duplicate clean logic and
# keeps normal success, build failure and Ctrl-C behavior consistent.
#
# If cleanup fails after an otherwise successful build, fail the script.
# If make already failed, keep make's exit code.
if ! run_trice_clean_if_needed; then
  if [ "${make_status}" -eq 0 ]; then
    make_status=1
  fi
fi

# Normal controlled end.
#
# Disable traps before exiting because cleanup has already been completed.
trap - INT TERM EXIT
exit "${make_status}"
