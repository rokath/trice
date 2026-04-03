#!/usr/bin/env bash
#
# format_shell_scripts.sh
#
# Central formatting and formatting-check tool for tracked shell scripts.
#
# This script:
#   - Enumerates all tracked *.sh files via `git ls-files`
#   - Runs shfmt either in:
#       * FORMAT MODE (in-place changes)
#       * CHECK MODE (diff only, CI-friendly)
#
# The goal:
#   - One single entry-point for shell formatting locally and in CI
#   - Identical behavior on macOS, Linux, and Windows Git Bash
#   - No accidental formatting of untracked files
#
# Usage:
#   ./scripts/format_shell_scripts.sh                 # defaults to CHECK mode
#   ./scripts/format_shell_scripts.sh check           # explicit CHECK mode
#   ./scripts/format_shell_scripts.sh format          # FORMAT mode (modify files)
#   ./scripts/format_shell_scripts.sh check --verbose
#   ./scripts/format_shell_scripts.sh format --verbose
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

MODE="check"
VERBOSE=0

for arg in "$@"; do
  case "$arg" in
    format|check)
      MODE="$arg"
      ;;
    -v|--verbose)
      VERBOSE=1
      ;;
    *)
      echo "Unknown argument: '$arg'"
      echo "Usage: $0 [format|check] [--verbose]"
      exit 2
      ;;
  esac
done

if ! command -v shfmt >/dev/null 2>&1; then
  echo "shfmt is not installed."
  echo "Install it first, for example with: brew install shfmt"
  exit 127
fi

SHELL_FILES=()
while IFS= read -r f; do
  [ -n "$f" ] && SHELL_FILES+=("$f")
done < <(git ls-files '*.sh')

if [ "${#SHELL_FILES[@]}" -eq 0 ]; then
  echo "shfmt: No tracked shell scripts found."
  exit 0
fi

NEEDS_FORMAT=()
while IFS= read -r f; do
  [ -n "$f" ] && NEEDS_FORMAT+=("$f")
done < <(shfmt -l "${SHELL_FILES[@]}")

if [ "$VERBOSE" -eq 1 ]; then
  echo "shfmt: The following files will be processed:"
  printf "  %s\n" "${SHELL_FILES[@]}"
  echo
fi

if [ "$MODE" = "format" ]; then
  if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
    echo "ok"
    exit 0
  fi

  if [ "$VERBOSE" -eq 1 ]; then
    echo "shfmt: Running in FORMAT mode (in-place changes)."
    printf "  %s\n" "${NEEDS_FORMAT[@]}"
  fi

  shfmt -w "${NEEDS_FORMAT[@]}"

  LOG_DIR="$REPO_ROOT/temp/log"
  LOG_FILE="$LOG_DIR/format_shell_scripts.log"
  mkdir -p "$LOG_DIR"
  {
    echo "shfmt format run changed these files:"
    printf "%s\n" "${NEEDS_FORMAT[@]}"
  } > "$LOG_FILE"

  if [ "$VERBOSE" -eq 1 ]; then
    echo
    echo "shfmt: Formatting completed."
    echo "shfmt: Change log written to $LOG_FILE"
  else
    echo "ok - changes applied, see ./temp/log/format_shell_scripts.log"
  fi
  exit 0
fi

if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
  echo "ok"
  exit 0
fi

if [ "$VERBOSE" -eq 1 ]; then
  echo "shfmt: Running in CHECK mode (diff only)."
  shfmt -d "${NEEDS_FORMAT[@]}"
fi

echo "not ok - use additionally --verbose for details"
echo "To fix them locally, run: ./scripts/format_shell_scripts.sh format"
exit 1
