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
#   ./clang-format.sh           # defaults to CHECK mode
#   ./clang-format.sh check     # explicit CHECK mode
#   ./clang-format.sh format    # FORMAT mode (modify files)
#
# Environment:
#   CLANG_FILTER_CMD can override how clang-filter is invoked.
#   By default we use:
#       go run ./cmd/clang-filter
#   which means no binary needs to be checked in or built manually.
#

set -euo pipefail

###############################################################################
# 1. Decide which mode we run in — FORMAT or CHECK
###############################################################################
MODE="${1:-check}"   # default mode is CHECK if no argument is given

case "$MODE" in
  format|check)
    ;;
  *)
    echo "Unknown mode: '$MODE'"
    echo "Usage: $0 [format|check]"
    exit 2
    ;;
esac

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
  echo "clang-format: No tracked C/C++ files found."
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
  [ -n "$f" ] && FILTERED_FILES+=("$f")
done < <(printf '%s\n' "$ALL_FILES" | $CLANG_FILTER_CMD)

if [ "${#FILTERED_FILES[@]}" -eq 0 ]; then
  echo "clang-format: No files remain after applying .clang-format-ignore."
  exit 0
fi

echo "clang-format: The following files will be processed:"
printf "  %s\n" "${FILTERED_FILES[@]}"
echo

###############################################################################
# 5. FORMAT MODE — apply clang-format in-place (-i)
#
# Used locally before committing changes.
###############################################################################
if [ "$MODE" = "format" ]; then
  echo "clang-format: Running in FORMAT mode (in-place changes)."
  clang-format -style=file -i "${FILTERED_FILES[@]}"
  echo "clang-format: Formatting completed."
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
echo "clang-format: Running in CHECK mode (no changes)."
FORMAT_ERRORS=0
NEEDS_FORMAT=()

for f in "${FILTERED_FILES[@]}"; do
  echo "Checking $f"

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

echo

if [ "$FORMAT_ERRORS" -ne 0 ]; then
  echo "clang-format: The following files require formatting:"
  printf "  %s\n" "${NEEDS_FORMAT[@]}"
  echo
  echo "To fix them locally, run:"
  echo "  ./clang-format.sh format"
  exit 1
fi

echo "clang-format: All files are correctly formatted."
exit 0
