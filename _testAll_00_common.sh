#!/usr/bin/env bash
#
# Shared helper functions for the testAll.sh script family.
#
# Direct invocation:
# - Not intended. This script is sourced by the individual
#   _testAll_XX_*.sh scripts.
#
# Important environment variables:
# - LOG_DIR
#   Target directory for per-step logs. Default: ./temp/testAll
# - GOCACHE
#   If unset, a repo-local cache under ./.gocache is used.
# - TRICE_TMP_DIR, TRICE_TIL_JSON, TRICE_LI_JSON
#   Shared paths used by the existing Trice helper scripts. The default JSON
#   files stay in the repository root to match the CGO tests and the manual
#   workflows.
#
# Goal of the split:
# - Each actual action lives in its own directly executable script.
# - testAll.sh only orchestrates those steps.
# - Each step writes its own detailed log into ./temp/testAll/.

set -u

# ROOT points to the directory that contains this helper script.
# In this repository that is also the project root, because _testAll_00_common.sh
# lives next to testAll.sh and the individual _testAll_XX_*.sh step scripts.
ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# Allow outer callers to override the log paths, but provide sensible defaults
# for normal local runs.
LOG_DIR="${LOG_DIR:-$ROOT/temp/testAll}"

# Create the detailed step log directory immediately when this file is sourced.
# mkdir -p does nothing if the directory already exists.
mkdir -p "$LOG_DIR"

log() {
  # Write one line to the terminal and append it to the current step log.
  # "$*" joins all function arguments into one text line.
  printf '%s\n' "$*" | tee -a "$LOGFILE"
}

init_logfile() {
  # Initialize the per-step log file from the current script name and add a
  # timestamped start line at the top of that log.
  LOGFILE="$LOG_DIR/$(basename "${BASH_SOURCE[1]}" .sh).log"
  : >"$LOGFILE"
  log "Starting $(basename "${BASH_SOURCE[1]}" .sh) at $(date)"
}

run_cmd() {
  # Log the command first so the step log shows what was attempted.
  log "+ $*"
  # Run the command, merge stderr into stdout, and append everything to the
  # step log.
  "$@" 2>&1 | tee -a "$LOGFILE"
  # In a pipeline, "$?" would only return the exit code of the last command
  # (here: tee). PIPESTATUS[0] gives us the exit code of the first command,
  # which is the actual command we wanted to run.
  local rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "Command failed with exit code $rc: $*"
  fi
  return "$rc"
}

has_command() {
  # command -v checks whether a tool name exists in PATH.
  command -v "$1" >/dev/null 2>&1
}

grep_log() {
  # Case-insensitive extended regex search in a log file.
  # Return code:
  # - 0: pattern found
  # - 1: pattern not found
  local pattern="$1"
  local file="$2"
  grep -Eiq "$pattern" "$file"
}

get_mode() {
  # Accept only the supported selection values and normalize "no argument" to "quick".
  case "${1:-quick}" in
    quick | full) printf '%s\n' "${1:-quick}" ;;
    *)
      printf 'Unsupported selection: %s\n' "${1:-}" >&2
      printf 'Allowed: quick, full\n' >&2
      exit 2
      ;;
  esac
}

# Every step sources this file, so initialize the shared environment here.
# Direct step calls default to quick mode. Callers such as _testAll_run.sh can
# export SELECTED beforehand to override that default for the whole run.
export SELECTED="${SELECTED:-quick}"
export GOCACHE="${GOCACHE:-$ROOT/.gocache}"
export TRICE_TMP_DIR="${TRICE_TMP_DIR:-$ROOT/temp/testAll}"
export TRICE_TIL_JSON="${TRICE_TIL_JSON:-$ROOT/demoTIL.json}"
export TRICE_LI_JSON="${TRICE_LI_JSON:-$ROOT/demoLI.json}"
mkdir -p "$GOCACHE" "$TRICE_TMP_DIR"
