#!/usr/bin/env bash
#
# Central clang-format helper script.
#
# Usage:
#   ./clang-format.sh           # format mode (default) → formats files in-place
#   ./clang-format.sh format    # same as above
#   ./clang-format.sh check     # CI mode → no changes, error if formatting differs
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
# 2. Collect file list (macOS-compatible, no mapfile)
#
# IMPORTANT:
# - We build a newline-separated list of files in a temporary variable.
# - Then we sort+uniq to remove duplicates.
###############################################################################

FILE_LIST=""

# src files
for f in ./src/*.c ./src/*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# _test files
for f in ./_test/*/*.c ./_test/*/*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# example headers
for f in ./examples/*_inst/Core/Inc/trice*.h; do
  [ -e "$f" ] && FILE_LIST="${FILE_LIST}${f}\n"
done

# Create sorted unique array
# (printf handles the \n escapes properly)
FILES=($(printf "%b" "$FILE_LIST" | sort -u))

if [ "${#FILES[@]}" -eq 0 ]; then
  echo "clang-format: No matching files found."
  exit 0
fi

echo "clang-format: Processing the following files:"
printf "  %s\n" "${FILES[@]}"
echo

###############################################################################
# 3. FORMAT MODE
###############################################################################
if [ "$MODE" = "format" ]; then
  echo "clang-format: Running in FORMAT mode (in-place changes)."
  clang-format -style=file -i "${FILES[@]}"
  echo
  echo "Formatting completed. Commit your changes."
  exit 0
fi

###############################################################################
# 4. CHECK MODE (no in-place changes)
###############################################################################
echo "clang-format: Running in CHECK mode (no changes)."
echo

FORMAT_ERRORS=0
NEEDS_FORMAT=()

for f in "${FILES[@]}"; do
  echo "Checking $f"
  if clang-format -style=file -output-replacements-xml "$f" | grep -q "<replacement "; then
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
  exit 1
fi

echo "clang-format: All files correctly formatted."
exit 0
