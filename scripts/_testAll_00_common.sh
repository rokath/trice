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
#   Target directory for per-step logs. Default: ./temp/log
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
# - Each step writes its own detailed log into ./temp/log/.

set -u

# ROOT points to the repository root. The helper scripts themselves live in
# ./scripts/, so derive the project root from the parent directory.
SCRIPTS_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPTS_DIR/.." && pwd)"
cd "$ROOT" || exit 1

# Allow outer callers to override the log paths, but provide sensible defaults
# for normal local runs.
mkdir -p "./temp/log" "./.gocache"
LOG_DIR="${LOG_DIR:-$(cd "./temp/log" && pwd)}"

# Support a quiet mode for aggregated testAll runs:
# - direct step invocation: keep terminal output plus logfile output
# - ./scripts/testAll.sh: only write full details to the logfile
QUIET=0
for arg in "$@"; do
  if [ "$arg" = "--quiet" ]; then
    QUIET=1
  fi
done
if [ "$QUIET" -eq 1 ]; then
  filtered_args=()
  for arg in "$@"; do
    if [ "$arg" != "--quiet" ]; then
      filtered_args+=("$arg")
    fi
  done
  set -- "${filtered_args[@]}"
fi

log() {
  # Write one line to the terminal and append it to the current step log.
  # "$*" joins all function arguments into one text line.
  if [ "$QUIET" -eq 1 ]; then
    printf '%s\n' "$*" >>"$LOGFILE"
  else
    printf '%s\n' "$*" | tee -a "$LOGFILE"
  fi
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
  local rc
  if [ "$QUIET" -eq 1 ]; then
    "$@" >>"$LOGFILE" 2>&1
    rc=$?
  else
    "$@" 2>&1 | tee -a "$LOGFILE"
    # In a pipeline, "$?" would only return the exit code of the last command
    # (here: tee). PIPESTATUS[0] gives us the exit code of the first command,
    # which is the actual command we wanted to run.
    rc=${PIPESTATUS[0]}
  fi
  if [ "$rc" -ne 0 ]; then
    log "Command failed with exit code $rc: $*"
  fi
  return "$rc"
}

log_pipe() {
  # Mirror pipeline output into the logfile and optionally into the terminal.
  if [ "$QUIET" -eq 1 ]; then
    cat >>"$LOGFILE"
  else
    tee -a "$LOGFILE"
  fi
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
export TRICE_TMP_DIR="${TRICE_TMP_DIR:-$LOG_DIR}"
export TRICE_TIL_JSON="${TRICE_TIL_JSON:-./demoTIL.json}"
export TRICE_LI_JSON="${TRICE_LI_JSON:-./demoLI.json}"
mkdir -p "$TRICE_TMP_DIR"
export GOCACHE="${GOCACHE:-$(cd "./.gocache" && pwd)}"
