#!/usr/bin/env bash
#
# Central clang-format helper script.
#
# Usage:
#   ./clang-format.sh           # format mode (default) → formats files in-place
#   ./clang-format.sh format    # same as above
#   ./clang-format.sh check     # CI mode → no changes, errors if formatting differs
#
# This script:
#   - Works on macOS (bash 3.2), Linux, and Windows (via Git Bash / MSYS2 / WSL)
#   - Uses the same file list everywhere
#   - Respects .clang-format-ignore with simple glob patterns (*, **, ?)
#

set -euo pipefail

###############################################################################
# 1. Determine mode (format or check)
###############################################################################
MODE="${1:-format}"

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
# 2. Collect file list (macOS-compatible, no 'mapfile', no bash 4 features)
#
# IMPORTANT:
#   - This is the single source of truth for which files we run clang-format on.
#   - Adjust the patterns below if your project layout changes.
###############################################################################

FILE_LIST=""

# src/*.c and src/*.h
for f in ./src/*.c ./src/*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# _test/*/*.c and _test/*/*.h
for f in ./_test/*/*.c ./_test/*/*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# specific example headers
for f in ./examples/*_inst/Core/Inc/trice*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# Convert the newline-separated list into a sorted, unique array.
FILES=($(printf "%b" "$FILE_LIST" | sort -u))

if [ "${#FILES[@]}" -eq 0 ]; then
  echo "clang-format: No matching files found."
  exit 0
fi

###############################################################################
# 3. Load .clang-format-ignore and convert patterns to regexes
#
# We implement a simple, robust ignore mechanism:
#   - lines starting with '#' or empty lines are ignored
#   - supports '*', '**', and '?' roughly like .gitignore
#   - we convert each glob pattern to a regular expression
#   - paths are matched relative to the repository root (no leading './')
###############################################################################

IGNORE_PATTERNS=()
IGNORE_REGEXES=()

# Trim leading and trailing whitespace (POSIX-friendly)
trim() {
  # usage: trim "  text  "
  # prints: text
  local s="$1"
  # remove leading whitespace
  s="${s#"${s%%[![:space:]]*}"}"
  # remove trailing whitespace
  s="${s%"${s##*[![:space:]]}"}"
  printf '%s' "$s"
}

# Convert a glob pattern (with *, **, ?) into a regex string
glob_to_regex() {
  local pat="$1"
  local i char next len
  local rx="^"

  # Normalize: strip leading "./" if present
  pat="${pat#./}"

  len=${#pat}
  i=0
  while [ "$i" -lt "$len" ]; do
    char="${pat:$i:1}"

    case "$char" in
      '*')
        # Check for '**' (two consecutive '*')
        if [ $((i + 1)) -lt "$len" ] && [ "${pat:$((i + 1)):1}" = "*" ]; then
          # '**' → '.*' (match anything, including '/')
          rx="${rx}.*"
          i=$((i + 1))  # skip the second '*'
        else
          # '*' → '[^/]*' (match anything except '/')
          rx="${rx}[^/]*"
        fi
        ;;
      '?')
        # '?' → '[^/]' (match any single character except '/')
        rx="${rx}[^/]"
        ;;
      '.')
        # '.' is special in regex → escape it
        rx="${rx}\."
        ;;
      '/')
        # '/' stays '/'
        rx="${rx}/"
        ;;
      '\\')
        # backslash → escape it in regex
        rx="${rx}\\\\"
        ;;
      '['|']'|'('|')'|'{'|'}'|'+'|'|'|'$'|'^')
        # escape regex meta characters
        rx="${rx}\\$char"
        ;;
      *)
        # normal character, use as-is
        rx="${rx}$char"
        ;;
    esac

    i=$((i + 1))
  done

  rx="${rx}$"
  printf '%s\n' "$rx"
}

# Load ignore patterns if the file exists
if [ -f .clang-format-ignore ]; then
  while IFS= read -r line || [ -n "$line" ]; do
    line="$(trim "$line")"

    # Skip empty lines and comments
    [ -z "$line" ] && continue
    case "$line" in
      \#*) continue ;;
    esac

    IGNORE_PATTERNS+=("$line")
  done < .clang-format-ignore
fi

# Build the regex list corresponding to each ignore pattern
if [ "${#IGNORE_PATTERNS[@]}" -gt 0 ]; then
  for pat in "${IGNORE_PATTERNS[@]}"; do
    IGNORE_REGEXES+=("$(glob_to_regex "$pat")")
  done
fi

# Returns 0 (true) if the given relative path should be ignored
is_ignored() {
  local rel_path="$1"
  local idx rx

  # Normalize: strip leading "./" again, ensure relative format
  rel_path="${rel_path#./}"

  if [ "${#IGNORE_REGEXES[@]}" -eq 0 ]; then
    return 1  # nothing to ignore
  fi

  idx=0
  while [ "$idx" -lt "${#IGNORE_REGEXES[@]}" ]; do
    rx="${IGNORE_REGEXES[$idx]}"
    # grep -E: extended regex match
    if printf '%s\n' "$rel_path" | grep -Eq "$rx"; then
      return 0
    fi
    idx=$((idx + 1))
  done

  return 1
}

###############################################################################
# 4. Filter the file list using .clang-format-ignore
###############################################################################

FILTERED_FILES=()
for f in "${FILES[@]}"; do
  rel="${f#./}"  # relative path without leading "./"

  if is_ignored "$rel"; then
    echo "clang-format: Ignoring $rel (matches .clang-format-ignore)"
    continue
  fi

  FILTERED_FILES+=("$f")
done

FILES=("${FILTERED_FILES[@]}")

if [ "${#FILES[@]}" -eq 0 ]; then
  echo "clang-format: No files left to process after applying .clang-format-ignore."
  exit 0
fi

echo "clang-format: Processing the following files:"
printf "  %s\n" "${FILES[@]}"
echo

###############################################################################
# 5. FORMAT MODE: run clang-format in-place
###############################################################################
if [ "$MODE" = "format" ]; then
  echo "clang-format: Running in FORMAT mode (in-place changes)."
  clang-format -style=file -i "${FILES[@]}"
  echo
  echo "clang-format: Formatting completed. Commit your changes."
  exit 0
fi

###############################################################################
# 6. CHECK MODE: detect formatting issues without changing files
#
# Strategy:
#   - For each file, run:
#       clang-format -style=file -output-replacements-xml
#   - If the XML contains '<replacement ', clang-format would change the file.
#   - We print a GitHub Actions ::error annotation and remember the file.
#   - Exit 1 if any file needs changes.
###############################################################################
echo "clang-format: Running in CHECK mode (no changes)."
echo

FORMAT_ERRORS=0
NEEDS_FORMAT=()

for f in "${FILES[@]}"; do
  echo "Checking $f"
  if clang-format -style=file -output-replacements-xml "$f" | grep -q "<replacement "; then
    # GitHub Actions log annotation (clickable per-file error)
    echo "::error file=$f::File is not formatted according to .clang-format"
    NEEDS_FORMAT+=("$f")
    FORMAT_ERRORS=1
  fi
done

echo

if [ "$FORMAT_ERRORS" -ne 0 ]; then
  echo "clang-format: The following files need formatting:"
  printf "  %s\n" "${NEEDS_FORMAT[@]}"
  echo
  echo "Fix locally by running:"
  echo "  ./clang-format.sh"
  echo "or:"
  echo "  ./clang-format.sh format"
  exit 1
fi

echo "clang-format: All files are correctly formatted."
exit 0
