#!/usr/bin/env bash
#
# _280_format_c_code.sh
#
# Central formatting and formatting-check tool for the project.
#
# This script:
#   - Enumerates *all* tracked C/C++ source/header files via `git ls-files`
#   - Lets clang-format apply `.clang-format-ignore` natively
#   - Processes the file list in one clang-format batch
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
#   ./scripts/_280_format_c_code.sh setup           # install/validate tool only
#   ./scripts/_280_format_c_code.sh check --verbose
#   ./scripts/_280_format_c_code.sh format --verbose
#
# Environment:
#   CLANG_FORMAT_BIN can select the required clang-format executable when it is
#   not named `clang-format` on the current platform. An explicit selection is
#   respected; unset it to enable automatic setup.
#   FORMAT and SETUP install a missing canonical formatter into ./temp/tools using
#   Python 3's venv and pip, just like CI. CHECK mode only reuses available tools
#   and never downloads them. The system Python/clang-format are not modified.
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

###############################################################################
# Decide which mode to run: FORMAT, CHECK, or tool-only SETUP.
###############################################################################
MODE="format"
VERBOSE=0

for arg in "$@"; do
  case "$arg" in
    format | check | setup)
      MODE="$arg"
      ;;
    -v | --verbose)
      VERBOSE=1
      ;;
    *)
      echo "Unknown argument: '$arg'"
      echo "Usage: $0 [format|check|setup] [--verbose]"
      exit 2
      ;;
  esac
done

###############################################################################
# Select the canonical formatter, independent of the host's default version.
###############################################################################

# clang-format output is not guaranteed to be stable across releases. Keep the
# repository and CI on the exact release that produced the checked-in files so
# `format` and `check` cannot disagree merely because they run on different
# operating systems or rolling CI images.
CLANG_FORMAT_REQUIRED_VERSION="23.1.2"

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

# find_cached_clang_format prefers native wheel binaries over Python launchers,
# so moving a checkout or losing its bootstrap interpreter does not break reuse.
find_cached_clang_format() {
  local tool_dir="$1"
  local candidate
  for candidate in \
    "$tool_dir/clang_format/data/bin/clang-format" \
    "$tool_dir/clang_format/data/bin/clang-format.exe" \
    "$tool_dir"/lib/python*/site-packages/clang_format/data/bin/clang-format \
    "$tool_dir/Lib/site-packages/clang_format/data/bin/clang-format.exe" \
    "$tool_dir/bin/clang-format" \
    "$tool_dir/Scripts/clang-format.exe"; do
    if clang_format_has_required_version "$candidate"; then
      CLANG_FORMAT_BIN="$candidate"
      return 0
    fi
  done
  return 1
}

# ensure_clang_format prefers a compatible selected/system binary, then a local
# cache. Only format/setup may bootstrap that cache; failed setup stops
# before any source is formatted. Separate platform/architecture directories
# avoid reusing native binaries across dual-boot systems or shared checkouts.
ensure_clang_format() {
  local requested="${CLANG_FORMAT_BIN:-clang-format}"
  local detected
  local candidate
  local tool_dir
  local python_command=()
  local venv_python=""
  local platform

  platform="$(uname -s)"
  case "$platform" in
    MINGW* | MSYS* | CYGWIN*) platform="windows" ;;
  esac
  tool_dir="./temp/tools/clang-format-$CLANG_FORMAT_REQUIRED_VERSION/$platform-$(uname -m)"

  if clang_format_has_required_version "$requested"; then
    CLANG_FORMAT_BIN="$requested"
    return 0
  fi
  detected="$CLANG_FORMAT_VERSION_OUTPUT"
  if [ -z "${CLANG_FORMAT_BIN:-}" ] && find_cached_clang_format "$tool_dir"; then
    return 0
  fi

  if [ "$MODE" = "check" ]; then
    # Without the canonical formatter there is no reliable formatting verdict.
    if ! command -v "$requested" >/dev/null 2>&1; then
      echo "MISSING TOOL: $requested"
    fi
    echo "SKIP: clang-format check requires $CLANG_FORMAT_REQUIRED_VERSION for reproducible output; detected $detected."
    echo "Hint: run ./scripts/_280_format_c_code.sh setup for automatic setup, or set CLANG_FORMAT_BIN."
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
  if find_cached_clang_format "$tool_dir"; then
    echo "clang-format: Using $CLANG_FORMAT_VERSION_OUTPUT from $CLANG_FORMAT_BIN."
    return 0
  fi
  echo "clang-format: Setup did not produce a working version $CLANG_FORMAT_REQUIRED_VERSION in $tool_dir; detected $CLANG_FORMAT_VERSION_OUTPUT." >&2
  return 1
}

ensure_clang_format

if [ "$MODE" = "setup" ]; then
  echo "clang-format: Ready: $CLANG_FORMAT_VERSION_OUTPUT from $CLANG_FORMAT_BIN."
  exit 0
fi

###############################################################################
# Validate configuration before any in-place edit, then collect tracked paths.
# Native ignore handling keeps vendor and scratch-pad files out of the batch.
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
# A private list avoids collisions between concurrent checks and command-line
# length limits on Windows. Git leaves spaces and UTF-8 path bytes intact.
FILES_LIST="$FORMAT_TMP_DIR/files"

# cleanup_format_tmp removes only artifacts created by this script invocation.
# Invoked indirectly by the EXIT trap installed by the formatting workflow.
# shellcheck disable=SC2329
cleanup_format_tmp() {
  rm -f "$FILES_LIST"
  rmdir "$FORMAT_TMP_DIR" 2>/dev/null || true
}
trap cleanup_format_tmp EXIT

if ! git -c core.quotePath=false ls-files \
  '*.c' '*.h' '*.cpp' '*.hpp' '*.cc' '*.hh' '*.cxx' '*.hxx' >"$FILES_LIST"; then
  echo "clang-format: Failed to list tracked C/C++ files in the repository." >&2
  exit 1
fi
if [ ! -s "$FILES_LIST" ]; then
  exit 0
fi

# One native process handles the entire batch; --verbose reports processed paths
# after ignore rules have been applied, rather than claiming ignored files ran.
FORMAT_ARGS=(--style=file:.clang-format --files="$FILES_LIST")
if [ "$VERBOSE" -eq 1 ]; then
  echo "clang-format: Running in $MODE mode with $CLANG_FORMAT_VERSION_OUTPUT."
  FORMAT_ARGS+=(--verbose)
fi
if [ "$MODE" = "format" ]; then
  if ! "$CLANG_FORMAT_BIN" -i "${FORMAT_ARGS[@]}"; then
    echo "clang-format: Formatting failed; inspect the file diagnostics above." >&2
    exit 1
  fi
elif ! "$CLANG_FORMAT_BIN" --dry-run --Werror "${FORMAT_ARGS[@]}"; then
  echo "::error::C/C++ formatting check failed; inspect the file diagnostics above."
  echo "To fix formatting locally, run: ./scripts/_280_format_c_code.sh format"
  exit 1
fi

exit 0
