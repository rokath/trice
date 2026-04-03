#!/usr/bin/env bash
#
# clang-format.sh
#
# Central formatting and formatting-check tool for the project.
#
# This script:
#   - Enumerates *all* tracked C/C++ source/header files via `git ls-files`
#   - Filters them using `.clang-format-ignore` by calling a Go-based helper
#     (`cmd/clang-filter`), executed via `go run`
#   - Runs clang-format either in:
#       * FORMAT MODE (in-place changes)
#       * CHECK MODE (no changes, CI-friendly, prints GitHub annotations)
#
# The goal:
#   - One single source of truth for ignored paths → .clang-format-ignore
#   - Identical behavior on macOS, Linux, and Windows Git Bash
#   - Simple integration into GitHub Actions
#
# Usage:
#   ./scripts/_format_c_code.sh                 # defaults to FORMAT mode
#   ./scripts/_format_c_code.sh check           # explicit CHECK mode
#   ./scripts/_format_c_code.sh format          # FORMAT mode (modify files)
#   ./scripts/_format_c_code.sh check --verbose
#   ./scripts/_format_c_code.sh format --verbose
#
# Environment:
#   CLANG_FILTER_CMD can override how clang-filter is invoked.
#   By default we use:
#       go run ./cmd/clang-filter
#   which means no binary needs to be checked in or built manually.
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

###############################################################################
# 1. Decide which mode we run in — FORMAT or CHECK
###############################################################################
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

###############################################################################
# 2. Define how clang-filter should be invoked.
#
# clang-filter is a small Go program that implements gitignore-style
# path filtering, using `.clang-format-ignore`.
#
# By default we call:
#       go run ./cmd/clang-filter
#
# But users may override this via CLANG_FILTER_CMD if they prefer
# a pre-built binary, e.g.:
#       CLANG_FILTER_CMD=./clang-filter ./clang-format.sh check
###############################################################################
CLANG_FILTER_CMD="${CLANG_FILTER_CMD:-go run ./cmd/clang-filter}"

###############################################################################
# 3. Collect *all* tracked C/C++ source/header files from git.
#
# We include all common extensions:
#   .c .h .cpp .hpp .cc .hh .cxx .hxx
#
# Using `git ls-files` ensures:
#   - Only tracked files are formatted (vendor directories may be ignored)
#   - Works identically in CI and locally
###############################################################################
ALL_FILES="$(
  git ls-files \
    '*.c' '*.h' \
    '*.cpp' '*.hpp' \
    '*.cc' '*.hh' \
    '*.cxx' '*.hxx'
)"

if [ -z "$ALL_FILES" ]; then
  exit 0
fi

###############################################################################
# 4. Filter files using `.clang-format-ignore` via clang-filter
#
# clang-filter receives file paths on STDIN and prints only those that
# *should NOT be ignored*, according to Gitignore rules.
#
# The result becomes the final list of files used for formatting/checking.
###############################################################################
FILTERED_FILES=()

while IFS= read -r f; do
  # Skip empty lines (just in case)
  [ -n "$f" ] && [ -e "$f" ] && FILTERED_FILES+=("$f")
done < <(printf '%s\n' "$ALL_FILES" | $CLANG_FILTER_CMD)

if [ "${#FILTERED_FILES[@]}" -eq 0 ]; then
  exit 0
fi

###############################################################################
# 5. FORMAT MODE — apply clang-format in-place (-i)
#
# Used locally before committing changes.
###############################################################################
if [ "$MODE" = "format" ]; then
  if [ "${#FILTERED_FILES[@]}" -eq 0 ]; then
    exit 0
  fi
  if [ "$VERBOSE" -eq 1 ]; then
    echo "clang-format: The following files will be processed:"
    printf "  %s\n" "${FILTERED_FILES[@]}"
    echo
    echo "clang-format: Running in FORMAT mode (in-place changes)."
  fi
  clang-format -style=file -i "${FILTERED_FILES[@]}"
  if [ "$VERBOSE" -eq 1 ]; then
    echo "clang-format: Formatting completed."
  fi
  exit 0
fi

###############################################################################
# 6. CHECK MODE — do NOT modify files, but detect formatting problems.
#
# This mode is CI-friendly:
#   - For each file, run clang-format with -output-replacements-xml
#   - If the XML contains <replacement ...> tags, the file needs reformatting
#   - Print GitHub Actions annotations via "::error file=..."
#   - Exit with code 1 if any file fails (so CI fails properly)
###############################################################################
FORMAT_ERRORS=0
NEEDS_FORMAT=()

for f in "${FILTERED_FILES[@]}"; do
  # clang-format outputs an XML diff where <replacement> tags represent
  # formatting operations that *would* be applied. If any are found,
  # the file is not correctly formatted.
  if clang-format -style=file -output-replacements-xml "$f" | grep -q "<replacement "; then
    # GitHub Actions annotation: makes clickable errors in PR UI
    echo "::error file=$f::File is not formatted according to .clang-format"
    NEEDS_FORMAT+=("$f")
    FORMAT_ERRORS=1
  fi
done

if [ "$FORMAT_ERRORS" -ne 0 ]; then
  echo "not ok - C/C++ files require formatting"
  printf "  %s\n" "${NEEDS_FORMAT[@]}"
  if [ "$VERBOSE" -eq 1 ]; then
    echo
  fi
  echo "To fix them locally, run: ./scripts/_format_c_code.sh format"
  exit 1
fi

exit 0
