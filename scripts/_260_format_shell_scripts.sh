#!/usr/bin/env bash
#
# _260_format_shell_scripts.sh
#
# Central formatting and formatting-check tool for tracked shell scripts.
#
# This script:
#   - Enumerates all tracked *.sh files via `git ls-files`
#   - Runs one pinned shfmt version either in:
#       * FORMAT MODE (in-place changes)
#       * CHECK MODE (diff only, CI-friendly)
#
# The goal:
#   - One single entry-point for shell formatting locally and in CI
#   - Identical behavior on macOS, Linux, and Windows Git Bash
#   - No accidental formatting of untracked files
#
# Usage:
#   ./scripts/_260_format_shell_scripts.sh                 # defaults to FORMAT mode
#   ./scripts/_260_format_shell_scripts.sh check           # explicit CHECK mode
#   ./scripts/_260_format_shell_scripts.sh format          # FORMAT mode (modify files)
#   ./scripts/_260_format_shell_scripts.sh check --verbose
#   ./scripts/_260_format_shell_scripts.sh format --verbose
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1
# shellcheck source=./_105_setup_test_tools.sh
source "$SCRIPT_DIR/_105_setup_test_tools.sh"

MODE="format"
VERBOSE=0
SHFMT_VERSION="v${TRICE_TOOL_SHFMT_VERSION:-3.14.1}"
TOOLS_DIR="$REPO_ROOT/temp/tools"
SHFMT_BIN="${SHFMT_BIN:-$TOOLS_DIR/shfmt}"

for arg in "$@"; do
  case "$arg" in
    format | check)
      MODE="$arg"
      ;;
    -v | --verbose)
      VERBOSE=1
      ;;
    *)
      echo "Unknown argument: '$arg'"
      echo "Usage: $0 [format|check] [--verbose]"
      exit 2
      ;;
  esac
done

ensure_shfmt() {
  # Native Windows Go installations append .exe even when called from Git Bash.
  if [ ! -x "$SHFMT_BIN" ] && [ -x "$SHFMT_BIN.exe" ]; then
    SHFMT_BIN="$SHFMT_BIN.exe"
  fi
  if [ -x "$SHFMT_BIN" ] && trice_tool_matches "$SHFMT_BIN" "${SHFMT_VERSION#v}"; then
    return 0
  fi
  # Formatting and checking share the same reuse policy for a matching host tool.
  if command -v shfmt >/dev/null 2>&1 && trice_tool_matches shfmt "${SHFMT_VERSION#v}"; then
    SHFMT_BIN="$(command -v shfmt)"
    return 0
  fi

  # A differently versioned formatter cannot give a reproducible verdict.
  # Reuse a canonical system tool before offering checkout-local installation.
  if [ "$MODE" = "check" ]; then
    if [ ! -x "$SHFMT_BIN" ] && command -v shfmt >/dev/null 2>&1; then
      SHFMT_BIN="$(command -v shfmt)"
    fi
    if [ -x "$SHFMT_BIN" ]; then
      if trice_tool_matches "$SHFMT_BIN" "${SHFMT_VERSION#v}"; then
        return 0
      fi
      echo "shfmt check requires $SHFMT_VERSION; detected $("$SHFMT_BIN" --version 2>&1)."
    else
      echo "INFO: shfmt $SHFMT_VERSION is not cached; preparing the declared formatter."
    fi
  fi

  if [ "$VERBOSE" -eq 1 ]; then
    echo "Installing pinned shfmt $SHFMT_VERSION into $TOOLS_DIR"
  fi

  # Share release-first preparation and disk planning with suite entry points.
  install_go_test_tool "mvdan.cc/sh/v3/cmd/shfmt@$SHFMT_VERSION" || exit 2
  SHFMT_BIN="$(command -v shfmt)"
  if [ ! -x "$SHFMT_BIN" ] && [ -x "$SHFMT_BIN.exe" ]; then
    SHFMT_BIN="$SHFMT_BIN.exe"
  fi

  if ! [ -x "$SHFMT_BIN" ]; then
    echo "Failed to install shfmt into $SHFMT_BIN" >&2
    exit 1
  fi

  if ! trice_tool_matches "$SHFMT_BIN" "${SHFMT_VERSION#v}"; then
    echo "Installed shfmt version mismatch. Expected $SHFMT_VERSION." >&2
    "$SHFMT_BIN" --version >&2 || true
    exit 1
  fi
}

ensure_shfmt

SHELL_FILES=()
while IFS= read -r f; do
  # Tracked notes and archives are not maintained shell sources.
  case "$f" in docs/_scratchPad/*) continue ;; esac
  [ -n "$f" ] && [ -e "$f" ] && SHELL_FILES+=("$f")
done < <(git ls-files '*.sh')

if [ "${#SHELL_FILES[@]}" -eq 0 ]; then
  exit 0
fi

SHFMT_ARGS=(-ln=bash -i 2 -ci)

NEEDS_FORMAT=()
while IFS= read -r f; do
  [ -n "$f" ] && NEEDS_FORMAT+=("$f")
done < <("$SHFMT_BIN" "${SHFMT_ARGS[@]}" -l "${SHELL_FILES[@]}")

if [ "$MODE" = "format" ]; then
  if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
    exit 0
  fi

  if [ "$VERBOSE" -eq 1 ]; then
    echo "shfmt $SHFMT_VERSION: Running in FORMAT mode (in-place changes)."
    printf "  %s\n" "${NEEDS_FORMAT[@]}"
  fi

  LOG_DIR="$REPO_ROOT/temp/log"
  LOG_FILE="$LOG_DIR/format_shell_scripts.log"
  mkdir -p "$LOG_DIR"

  if ! "$SHFMT_BIN" "${SHFMT_ARGS[@]}" -d "${NEEDS_FORMAT[@]}" >"$LOG_FILE"; then
    :
  fi

  "$SHFMT_BIN" "${SHFMT_ARGS[@]}" -w "${NEEDS_FORMAT[@]}"

  if [ "$VERBOSE" -eq 1 ]; then
    echo
    echo "shfmt $SHFMT_VERSION: Formatting completed."
    echo "shfmt: Change log written to $LOG_FILE"
  fi

  exit 0
fi

if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
  exit 0
fi

if [ "$VERBOSE" -eq 1 ]; then
  echo "shfmt $SHFMT_VERSION: Running in CHECK mode (diff only)."
  "$SHFMT_BIN" "${SHFMT_ARGS[@]}" -d "${NEEDS_FORMAT[@]}"
fi

echo "not ok - shell scripts require formatting"
echo "To fix them locally, run: ./scripts/_260_format_shell_scripts.sh format"
exit 1
