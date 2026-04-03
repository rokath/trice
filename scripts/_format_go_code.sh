#!/usr/bin/env bash
#
# _format_go_code.sh
#
# Central formatting and formatting-check tool for tracked Go source files.
#
# This script:
#   - Enumerates all tracked *.go files via `git ls-files`
#   - Runs gofmt either in:
#       * FORMAT MODE (in-place changes)
#       * CHECK MODE (list only, CI-friendly)
#
# The goal:
#   - One single entry-point for Go formatting locally and in CI
#   - Identical behavior on macOS, Linux, and Windows Git Bash
#   - No accidental formatting of untracked files
#
# Usage:
#   ./scripts/_format_go_code.sh                 # defaults to FORMAT mode
#   ./scripts/_format_go_code.sh check           # explicit CHECK mode
#   ./scripts/_format_go_code.sh format          # FORMAT mode (modify files)
#   ./scripts/_format_go_code.sh check --verbose
#   ./scripts/_format_go_code.sh format --verbose
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

MODE="format"
VERBOSE=0

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

if ! command -v gofmt >/dev/null 2>&1; then
  echo "gofmt is not installed."
  echo "Install Go first so gofmt is available on PATH."
  exit 127
fi

GO_FILES=()
while IFS= read -r f; do
  [ -n "$f" ] && [ -e "$f" ] && GO_FILES+=("$f")
done < <(git ls-files '*.go')

if [ "${#GO_FILES[@]}" -eq 0 ]; then
  exit 0
fi

NEEDS_FORMAT=()
while IFS= read -r f; do
  [ -n "$f" ] && NEEDS_FORMAT+=("$f")
done < <(gofmt -l "${GO_FILES[@]}")

if [ "$MODE" = "format" ]; then
  if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
    exit 0
  fi

  if [ "$VERBOSE" -eq 1 ]; then
    echo "gofmt: Running in FORMAT mode (in-place changes)."
    printf "  %s\n" "${NEEDS_FORMAT[@]}"
  fi

  LOG_DIR="$REPO_ROOT/temp/log"
  LOG_FILE="$LOG_DIR/format_go_code.log"
  mkdir -p "$LOG_DIR"
  if ! gofmt -d "${NEEDS_FORMAT[@]}" >"$LOG_FILE"; then
    :
  fi
  gofmt -w "${NEEDS_FORMAT[@]}"

  if [ "$VERBOSE" -eq 1 ]; then
    echo
    echo "gofmt: Formatting completed."
    echo "gofmt: Change log written to $LOG_FILE"
  fi
  exit 0
fi

if [ "${#NEEDS_FORMAT[@]}" -eq 0 ]; then
  exit 0
fi

if [ "$VERBOSE" -eq 1 ]; then
  echo "gofmt: Running in CHECK mode (list only)."
  printf "  %s\n" "${NEEDS_FORMAT[@]}"
fi

echo "not ok - Go files require formatting"
echo "To fix them locally, run: ./scripts/_format_go_code.sh format"
exit 1
