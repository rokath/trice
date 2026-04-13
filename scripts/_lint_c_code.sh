#!/usr/bin/env bash
#
# Run static analysis on the Trice C library sources with a selectable backend.
# The initial backend is cppcheck. Later, other tools such as clang-tidy can be
# added without changing the file selection logic.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT" || exit 1

TOOL="cppcheck"
VERBOSE=0
BUFFER="ring"
OUTPUT="deferred"
ENDIAN="little"
BUILTIN="0"

while [ "$#" -gt 0 ]; do
  case "$1" in
  cppcheck)
    TOOL="$1"
    ;;
  --buffer)
    BUFFER="${2:-}"
    shift
    ;;
  --output)
    OUTPUT="${2:-}"
    shift
    ;;
  --endian)
    ENDIAN="${2:-}"
    shift
    ;;
  --builtin)
    BUILTIN="${2:-}"
    shift
    ;;
  -v | --verbose)
    VERBOSE=1
    ;;
  *)
    echo "Unknown argument: '$1'"
    echo "Usage: $0 [cppcheck] [--buffer ring|double|stack|static] [--output deferred|direct|both] [--endian little|big] [--builtin 0|1] [--verbose]"
    exit 2
    ;;
  esac
  shift
done

LINT_FILES=(
  src/cobs.h
  src/cobsDecode.c
  src/cobsEncode.c
  src/tcobs.h
  src/tcobsv1Decode.c
  src/tcobsv1Encode.c
  src/tcobsv1Internal.h
  src/trice.c
  src/trice.h
  src/trice16.c
  src/trice16.h
  src/trice16McuOrder.h
  src/trice16McuReverse.h
  src/trice32.c
  src/trice32.h
  src/trice32McuOrder.h
  src/trice32McuReverse.h
  src/trice64.c
  src/trice64.h
  src/trice64McuOrder.h
  src/trice64McuReverse.h
  src/trice8.c
  src/trice8.h
  src/trice8McuOrder.h
  src/trice8McuReverse.h
  src/triceAuxiliary.c
  src/triceDefaultConfig.h
  src/triceDoubleBuffer.c
  src/triceMcuOrder.h
  src/triceMcuReverse.h
  src/triceOff.h
  src/triceOn.h
  src/triceRingBuffer.c
  src/triceStackBuffer.c
  src/triceStaticBuffer.c
  src/triceUart.c
  src/triceVariadic.h
  src/xtea.c
  src/xtea.h
)

if [ "${#LINT_FILES[@]}" -eq 0 ]; then
  exit 0
fi

if [ "$VERBOSE" -eq 1 ]; then
  echo "C lint files:"
  printf "  %s\n" "${LINT_FILES[@]}"
fi

case "$BUFFER" in
ring)
  TRICE_BUFFER_DEFINE="TRICE_RING_BUFFER"
  ;;
double)
  TRICE_BUFFER_DEFINE="TRICE_DOUBLE_BUFFER"
  ;;
stack)
  TRICE_BUFFER_DEFINE="TRICE_STACK_BUFFER"
  ;;
static)
  TRICE_BUFFER_DEFINE="TRICE_STATIC_BUFFER"
  ;;
*)
  echo "Unsupported buffer mode: $BUFFER" >&2
  exit 2
  ;;
esac

case "$OUTPUT" in
deferred)
  TRICE_DEFERRED_OUTPUT_DEFINE=1
  TRICE_DIRECT_OUTPUT_DEFINE=0
  TRICE_DEFERRED_AUXILIARY8_DEFINE=1
  TRICE_DIRECT_AUXILIARY32_DEFINE=0
  ;;
direct)
  TRICE_DEFERRED_OUTPUT_DEFINE=0
  TRICE_DIRECT_OUTPUT_DEFINE=1
  TRICE_DEFERRED_AUXILIARY8_DEFINE=0
  TRICE_DIRECT_AUXILIARY32_DEFINE=1
  ;;
both)
  TRICE_DEFERRED_OUTPUT_DEFINE=1
  TRICE_DIRECT_OUTPUT_DEFINE=1
  TRICE_DEFERRED_AUXILIARY8_DEFINE=1
  TRICE_DIRECT_AUXILIARY32_DEFINE=1
  ;;
*)
  echo "Unsupported output mode: $OUTPUT" >&2
  exit 2
  ;;
esac

case "$ENDIAN" in
little)
  TRICE_MCU_IS_BIG_ENDIAN_DEFINE=0
  ;;
big)
  TRICE_MCU_IS_BIG_ENDIAN_DEFINE=1
  ;;
*)
  echo "Unsupported endian mode: $ENDIAN" >&2
  exit 2
  ;;
esac

case "$BUILTIN" in
0 | 1)
  ;;
*)
  echo "Unsupported builtin mode: $BUILTIN" >&2
  exit 2
  ;;
esac

if { [ "$BUFFER" = "ring" ] || [ "$BUFFER" = "double" ]; } && [ "$OUTPUT" = "direct" ]; then
  echo "Invalid configuration: buffer '$BUFFER' requires output 'deferred' or 'both'" >&2
  exit 2
fi

if { [ "$BUFFER" = "stack" ] || [ "$BUFFER" = "static" ]; } && [ "$OUTPUT" != "direct" ]; then
  echo "Invalid configuration: buffer '$BUFFER' requires output 'direct'" >&2
  exit 2
fi

if [ "$VERBOSE" -eq 1 ]; then
  echo "Lint profile: buffer=$BUFFER output=$OUTPUT endian=$ENDIAN builtin=$BUILTIN"
fi

find_cppcheck() {
  if command -v cppcheck >/dev/null 2>&1; then
    command -v cppcheck
    return 0
  fi

  if [ -x "/c/Program Files/Cppcheck/cppcheck.exe" ]; then
    printf '%s\n' "/c/Program Files/Cppcheck/cppcheck.exe"
    return 0
  fi

  return 1
}

run_cppcheck() {
  local cppcheck_bin
  if ! cppcheck_bin="$(find_cppcheck)"; then
    echo "cppcheck is not installed" >&2
    exit 1
  fi

  # cppcheck does not model compiler-specific __has_builtin(...) probing well.
  # Define it to 0 so Clang-only feature tests stay parseable during analysis.
  # tcobsv1Encode combines a fixed sigil prefix with a variable offset.
  # cppcheck 2.20 reports a false-positive badBitmaskCheck at this site.
  "$cppcheck_bin" \
    --error-exitcode=1 \
    --enable=warning,style,performance,portability \
    --inline-suppr \
    --std=c99 \
    --language=c \
    --quiet \
    -D"__has_builtin(x)=$BUILTIN" \
    -D"TRICE_BUFFER=$TRICE_BUFFER_DEFINE" \
    -D"TRICE_DEFERRED_OUTPUT=$TRICE_DEFERRED_OUTPUT_DEFINE" \
    -D"TRICE_DIRECT_OUTPUT=$TRICE_DIRECT_OUTPUT_DEFINE" \
    -D"TRICE_DEFERRED_AUXILIARY8=$TRICE_DEFERRED_AUXILIARY8_DEFINE" \
    -D"TRICE_DIRECT_AUXILIARY32=$TRICE_DIRECT_AUXILIARY32_DEFINE" \
    -D"TRICE_MCU_IS_BIG_ENDIAN=$TRICE_MCU_IS_BIG_ENDIAN_DEFINE" \
    --suppress=badBitmaskCheck:src/tcobsv1Encode.c \
    --suppress=missingIncludeSystem \
    -Isrc \
    "${LINT_FILES[@]}"
}

case "$TOOL" in
cppcheck) run_cppcheck ;;
*)
  echo "Unsupported lint tool: $TOOL" >&2
  exit 2
  ;;
esac
