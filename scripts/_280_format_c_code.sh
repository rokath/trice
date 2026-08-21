#!/usr/bin/env bash
#
# _280_format_c_code.sh
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
#   ./scripts/_280_format_c_code.sh                 # defaults to FORMAT mode
#   ./scripts/_280_format_c_code.sh check           # explicit CHECK mode
#   ./scripts/_280_format_c_code.sh format          # FORMAT mode (modify files)
#   ./scripts/_280_format_c_code.sh check --verbose
#   ./scripts/_280_format_c_code.sh format --verbose
#
# Environment:
#   CLANG_FILTER_CMD can override how clang-filter is invoked.
#   By default we use:
#       go run ./cmd/clang-filter
#   which means no binary needs to be checked in or built manually.
#   CLANG_FORMAT_BIN can select the required clang-format executable when it is
#   not named `clang-format` on the current platform.
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

# clang-format output is not guaranteed to be stable across releases. Keep the
# repository and CI on the exact release that produced the checked-in files so
# `format` and `check` cannot disagree merely because they run on different
# operating systems or rolling CI images.
CLANG_FORMAT_BIN="${CLANG_FORMAT_BIN:-clang-format}"
CLANG_FORMAT_REQUIRED_VERSION="19.1.7"

if ! command -v "$CLANG_FORMAT_BIN" >/dev/null 2>&1; then
  echo "clang-format: Required executable not found: $CLANG_FORMAT_BIN" >&2
  echo "Install clang-format $CLANG_FORMAT_REQUIRED_VERSION or set CLANG_FORMAT_BIN to that executable." >&2
  exit 1
fi

CLANG_FORMAT_VERSION_OUTPUT="$("$CLANG_FORMAT_BIN" --version)" || {
  echo "clang-format: Failed to query the formatter version from $CLANG_FORMAT_BIN." >&2
  exit 1
}
case "$CLANG_FORMAT_VERSION_OUTPUT" in
  *"version $CLANG_FORMAT_REQUIRED_VERSION"*) ;;
  *)
    echo "clang-format: Unsupported formatter version." >&2
    echo "  required: $CLANG_FORMAT_REQUIRED_VERSION" >&2
    echo "  detected: $CLANG_FORMAT_VERSION_OUTPUT" >&2
    echo "Install the required version or set CLANG_FORMAT_BIN to its executable." >&2
    exit 1
    ;;
esac

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
# 5. Validate the configuration and prepare isolated stdin processing.
#
# clang-format also reads .clang-format-ignore itself when a real file name is
# passed. Releases with the old negation implementation can incorrectly ignore
# unrelated files when the ignore file contains re-inclusion patterns. The Go
# filter above already made the authoritative selection, so process each file
# via stdin with a guaranteed-nonexistent assumed path. This preserves language
# detection without triggering clang-format's second ignore-file evaluation.
###############################################################################
if ! "$CLANG_FORMAT_BIN" -style=file:.clang-format -dump-config >/dev/null; then
  echo "clang-format: Failed to parse .clang-format." >&2
  "$CLANG_FORMAT_BIN" --version >&2 || true
  exit 1
fi

FORMAT_TMP_DIR="$(mktemp -d)" || {
  echo "clang-format: Failed to create a temporary directory." >&2
  exit 1
}
FORMAT_TMP_FILE="$FORMAT_TMP_DIR/output"

# cleanup_format_tmp removes only artifacts created by this script invocation.
cleanup_format_tmp() {
  rm -f "$FORMAT_TMP_FILE"
  rmdir "$FORMAT_TMP_DIR" 2>/dev/null || true
}
trap cleanup_format_tmp EXIT

# clang_format_file writes formatted output or replacement XML for one source
# file to stdout. The nonexistent assumed path retains the source extension but
# cannot match clang-format's native ignore processing.
clang_format_file() {
  local source_file="$1"
  shift
  local extension="${source_file##*.}"
  local assumed_file="$FORMAT_TMP_DIR/input.$extension"
  "$CLANG_FORMAT_BIN" -style=file:.clang-format --assume-filename="$assumed_file" "$@" <"$source_file"
}

###############################################################################
# 6. FORMAT MODE — apply clang-format output to changed files only.
#
# Direct -i processing would invoke clang-format's native ignore handling. A
# temporary output also ensures a formatter failure cannot partially overwrite
# the source file.
###############################################################################
if [ "$MODE" = "format" ]; then
  if [ "$VERBOSE" -eq 1 ]; then
    echo "clang-format: The following files will be processed:"
    printf "  %s\n" "${FILTERED_FILES[@]}"
    echo
    echo "clang-format: Running in FORMAT mode (in-place changes)."
  fi
  for f in "${FILTERED_FILES[@]}"; do
    if ! clang_format_file "$f" >"$FORMAT_TMP_FILE"; then
      echo "clang-format: Failed to format '$f'." >&2
      exit 1
    fi
    if ! cmp -s "$f" "$FORMAT_TMP_FILE"; then
      cp "$FORMAT_TMP_FILE" "$f"
    fi
  done
  if [ "$VERBOSE" -eq 1 ]; then
    echo "clang-format: Formatting completed."
  fi
  exit 0
fi

###############################################################################
# 7. CHECK MODE — do NOT modify files, but detect formatting problems.
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
  if ! clang_format_file "$f" -output-replacements-xml >"$FORMAT_TMP_FILE"; then
    echo "clang-format: Failed to check '$f'." >&2
    exit 1
  fi
  if grep -q "<replacement " "$FORMAT_TMP_FILE"; then
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
  echo "To fix them locally, run: ./scripts/_280_format_c_code.sh format"
  exit 1
fi

exit 0
