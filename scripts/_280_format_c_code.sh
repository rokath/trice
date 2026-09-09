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
#   not named `clang-format` on the current platform. An explicit selection is
#   respected; unset it to enable automatic setup.
#   FORMAT mode installs a missing canonical formatter into ./temp/tools using
#   Python 3's venv and pip, just like CI. CHECK mode only reuses available tools
#   and never downloads them. The system Python/clang-format are not modified.
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
CLANG_FORMAT_REQUIRED_VERSION="19.1.7"

# clang_format_has_required_version validates actual executables, not just a
# cache directory's existence. This also detects incomplete installs and venvs
# whose Python executable stopped working after moving the repository.
clang_format_has_required_version() {
  if ! command -v "$1" >/dev/null 2>&1; then
    CLANG_FORMAT_VERSION_OUTPUT="not installed ($1)"
    return 1
  fi
  CLANG_FORMAT_VERSION_OUTPUT="$("$1" --version 2>&1)" || return 1
  case "$CLANG_FORMAT_VERSION_OUTPUT" in
    *"version $CLANG_FORMAT_REQUIRED_VERSION" | *"version $CLANG_FORMAT_REQUIRED_VERSION "*) return 0 ;;
    *) return 1 ;;
  esac
}

# ensure_clang_format prefers a compatible selected/system binary, then a local
# cache. Only explicit formatting may bootstrap that cache; failed setup stops
# before any source is formatted. Separate platform/architecture directories
# avoid reusing native binaries across dual-boot systems or shared checkouts.
ensure_clang_format() {
  local requested="${CLANG_FORMAT_BIN:-clang-format}"
  local detected
  local candidate
  local tool_dir
  local python_command=()
  local venv_python=""

  tool_dir="./temp/tools/clang-format-$CLANG_FORMAT_REQUIRED_VERSION/$(uname -s)-$(uname -m)"
  if clang_format_has_required_version "$requested"; then
    CLANG_FORMAT_BIN="$requested"
    return 0
  fi
  detected="$CLANG_FORMAT_VERSION_OUTPUT"
  if [ -z "${CLANG_FORMAT_BIN:-}" ]; then
    for candidate in "$tool_dir/bin/clang-format" "$tool_dir/Scripts/clang-format.exe"; do
      if clang_format_has_required_version "$candidate"; then
        CLANG_FORMAT_BIN="$candidate"
        return 0
      fi
    done
  fi

  if [ "$MODE" = "check" ]; then
    # Without the canonical formatter there is no reliable formatting verdict.
    if ! command -v "$requested" >/dev/null 2>&1; then
      echo "MISSING TOOL: $requested"
    fi
    echo "SKIP: clang-format check requires $CLANG_FORMAT_REQUIRED_VERSION for reproducible output; detected $detected."
    echo "Hint: run ./scripts/_280_format_c_code.sh format for automatic setup, or set CLANG_FORMAT_BIN."
    exit 0
  fi
  if [ -n "${CLANG_FORMAT_BIN:-}" ]; then
    echo "clang-format: Explicit CLANG_FORMAT_BIN requires version $CLANG_FORMAT_REQUIRED_VERSION; detected $detected." >&2
    echo "Unset CLANG_FORMAT_BIN to use automatic setup, or select the required version." >&2
    return 1
  fi

  # Try the native Windows launcher first; python3 may be a Store alias there.
  if command -v py >/dev/null 2>&1 && py -3 -c 'import sys; sys.exit(sys.version_info[0] != 3)' >/dev/null 2>&1; then
    python_command=(py -3)
  else
    for candidate in python3 python; do
      if command -v "$candidate" >/dev/null 2>&1 && "$candidate" -c 'import sys; sys.exit(sys.version_info[0] != 3)' >/dev/null 2>&1; then
        python_command=("$candidate")
        break
      fi
    done
  fi
  if [ "${#python_command[@]}" -eq 0 ]; then
    echo "clang-format: Automatic setup requires Python 3 with venv and pip." >&2
    echo "Install Python 3, or set CLANG_FORMAT_BIN to clang-format $CLANG_FORMAT_REQUIRED_VERSION." >&2
    return 1
  fi

  echo "clang-format: Preparing $CLANG_FORMAT_REQUIRED_VERSION in $tool_dir (detected $detected)."
  if ! "${python_command[@]}" -m venv "$tool_dir"; then
    echo "clang-format: Cannot create $tool_dir; ensure the selected Python includes venv/ensurepip (python3-venv on Debian)." >&2
    return 1
  fi
  for candidate in "$tool_dir/bin/python" "$tool_dir/Scripts/python.exe"; do
    if [ -x "$candidate" ]; then
      venv_python="$candidate"
      break
    fi
  done
  if [ -z "$venv_python" ]; then
    echo "clang-format: No Python executable found in $tool_dir/bin or $tool_dir/Scripts after venv setup." >&2
    return 1
  fi
  # Download wheels only: automatic setup must never start a native LLVM build.
  # Reinstallation repairs an incomplete cache; subsequent valid runs skip pip.
  if ! "$venv_python" -m pip install --disable-pip-version-check --no-input --no-cache-dir \
    --only-binary=:all: --force-reinstall --retries 3 --timeout 20 "clang-format==$CLANG_FORMAT_REQUIRED_VERSION"; then
    echo "clang-format: Installation into $tool_dir failed; check network/wheel availability and rerun. No C/C++ sources were formatted." >&2
    return 1
  fi
  for candidate in "$tool_dir/bin/clang-format" "$tool_dir/Scripts/clang-format.exe"; do
    if clang_format_has_required_version "$candidate"; then
      CLANG_FORMAT_BIN="$candidate"
      echo "clang-format: Using $CLANG_FORMAT_VERSION_OUTPUT from $CLANG_FORMAT_BIN."
      return 0
    fi
  done
  echo "clang-format: Setup did not produce a working version $CLANG_FORMAT_REQUIRED_VERSION in $tool_dir; detected $CLANG_FORMAT_VERSION_OUTPUT." >&2
  return 1
}

ensure_clang_format

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
