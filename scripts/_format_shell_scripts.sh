#!/usr/bin/env bash
#
# _format_shell_scripts.sh
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
#   ./scripts/_format_shell_scripts.sh                 # defaults to FORMAT mode
#   ./scripts/_format_shell_scripts.sh check           # explicit CHECK mode
#   ./scripts/_format_shell_scripts.sh format          # FORMAT mode (modify files)
#   ./scripts/_format_shell_scripts.sh check --verbose
#   ./scripts/_format_shell_scripts.sh format --verbose
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

MODE="format"
VERBOSE=0
SHFMT_VERSION="v3.8.0"
TOOLS_DIR="$REPO_ROOT/temp/tools"
SHFMT_BIN="$TOOLS_DIR/shfmt"

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
  mkdir -p "$TOOLS_DIR"

  if [ -x "$SHFMT_BIN" ] && "$SHFMT_BIN" --version 2>/dev/null | grep -qx "$SHFMT_VERSION"; then
    return 0
  fi

  if ! command -v go >/dev/null 2>&1; then
    echo "go is required to install pinned shfmt $SHFMT_VERSION." >&2
    echo "Install Go first, then rerun: ./scripts/_format_shell_scripts.sh $MODE" >&2
    exit 127
  fi

  if [ "$VERBOSE" -eq 1 ]; then
    echo "Installing pinned shfmt $SHFMT_VERSION into $TOOLS_DIR"
  fi

  GOBIN="$TOOLS_DIR" go install "mvdan.cc/sh/v3/cmd/shfmt@$SHFMT_VERSION"

  if ! [ -x "$SHFMT_BIN" ]; then
    echo "Failed to install shfmt into $SHFMT_BIN" >&2
    exit 1
  fi

  if ! "$SHFMT_BIN" --version 2>/dev/null | grep -qx "$SHFMT_VERSION"; then
    echo "Installed shfmt version mismatch. Expected $SHFMT_VERSION." >&2
    "$SHFMT_BIN" --version >&2 || true
    exit 1
  fi
}

ensure_shfmt

SHELL_FILES=()
while IFS= read -r f; do
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
echo "To fix them locally, run: ./scripts/_format_shell_scripts.sh format"
exit 1
