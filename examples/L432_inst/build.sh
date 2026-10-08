#!/usr/bin/env bash
#
# Usage: ./build.sh                    (normal firmware build)
#        ./build.sh TRICE_OFF=1        (compile without Trice logging)
# Build the firmware; this script does not flash the board.
# Requires Bash, make, Trice and the Arm GNU toolchain in PATH.
# Library/image details: ../../docs/TriceReferenceManual.md#trice-project-image-size-optimization
# Normal standalone builds prepare Bind headers before compiling.
# Repository tests can supply an Insert workflow instead. Cleanup then removes
# the temporary IDs on success, failure or Ctrl-C; keep those safeguards intact.
# The helpers below handle that cleanup. The actual build commands follow them.

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

    bash "${ROOT}/scripts/_240_legacy_clean_ids.sh" || clean_status=$?
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
  local clean_status=0

  # Disable traps immediately.
  #
  # This prevents recursive cleanup if:
  # - cleanup itself causes EXIT,
  # - the user presses Ctrl-C again while cleanup is already running,
  # - the clean helper exits with an error.
  trap - INT TERM EXIT

  run_trice_clean_if_needed || clean_status=$?
  if [ "${clean_status}" -ne 0 ]; then
    # If the script was otherwise successful, a cleanup failure should make the
    # whole script fail. If the script was already failing, keep the original
    # status so the root cause is not hidden by the cleanup failure.
    if [ "${status}" -eq 0 ]; then
      status="${clean_status}"
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

# Bound sources need generated sidecar headers and their include path even when
# this script is called directly instead of through the aggregate test wrapper.
# shellcheck source=../prepareTriceBind.sh
source "${ROOT}/examples/prepareTriceBind.sh"
prepare_trice_bind_build "${ROOT}"

# A managed test wrapper or the direct Bind preparation owns the source state.
# Only the legacy fallback needs to modify and later restore the sources.
if [ "${TRICE_ID_WORKFLOW_OWNER:-0}" != "1" ]; then
  cd "${ROOT}"
  bash "${ROOT}/scripts/_240_legacy_clean_ids.sh"
  bash "${ROOT}/scripts/_230_legacy_insert_ids.sh"
  ids_inserted=1
fi

# ------------------------------------------------------------------------------
# 5) Load build environment and build
# ------------------------------------------------------------------------------

# shellcheck disable=SC1091
source "${ROOT}/scripts/_150_setup_build_environment.sh"

# Provide a default if _150_setup_build_environment.sh does not set MAKE_JOBS.
: "${MAKE_JOBS:=-j2}"

# Ensure we build from the example directory, where the Makefile is expected.
cd "${SCRIPT_DIR}"

# Make cannot detect command-line define or ID-workflow changes from object
# dependencies. Record both inputs beside the objects and clean only when that
# build signature changes. A missing signature beside existing objects is
# treated as unknown state and therefore triggers one compatibility clean.
build_state_dir="${SCRIPT_DIR}/out.gcc"
build_signature_file="${build_state_dir}/.trice-build-signature"
build_signature="workflow=${TRICE_ID_WORKFLOW:-legacy};flags=${flags}"
previous_build_signature=""
if [ -f "${build_signature_file}" ]; then
  IFS= read -r previous_build_signature <"${build_signature_file}" || true
fi
if [ -d "${build_state_dir}" ] && [ "${previous_build_signature}" != "${build_signature}" ]; then
  echo "Build configuration changed; cleaning out.gcc"
  make clean
fi
mkdir -p "${build_state_dir}"
printf '%s\n' "${build_signature}" >"${build_signature_file}"

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
clean_status=0
run_trice_clean_if_needed || clean_status=$?
if [ "${clean_status}" -ne 0 ]; then
  if [ "${make_status}" -eq 0 ]; then
    make_status="${clean_status}"
  fi
fi

# Normal controlled end.
#
# Disable traps before exiting because cleanup has already been completed.
trap - INT TERM EXIT
exit "${make_status}"
