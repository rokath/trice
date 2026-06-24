#!/usr/bin/env bash
set -euo pipefail

# build.sh
#
# Build every TriceAbc demo node in a host-native way.
#
# The script keeps three responsibilities together because they depend on the
# same exact source state:
# 1. insert IDs into the demo sources,
# 2. generate the shared ABC and TIL-C helper files,
# 3. compile the nine demo binaries.
#
# After a successful insert step the script always runs `trice clean` during
# cleanup so the demo sources do not remain unnecessarily in inserted form.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

ids_inserted=0

run_trice() {
  if [ "${TRICE_BIN:-}" != "" ]; then
    "${TRICE_BIN}" "$@"
    return
  fi

  if command -v go >/dev/null 2>&1; then
    (
      cd "${ROOT}"
      go run ./cmd/trice "$@"
    )
    return
  fi

  if command -v trice >/dev/null 2>&1; then
    trice "$@"
    return
  fi

  echo "error: neither Go nor a trice binary is available." >&2
  echo "Install Go or run with TRICE_BIN=/path/to/trice." >&2
  exit 1
}

cleanup() {
  local status="${1:-$?}"

  trap - EXIT INT TERM
  cd "${SCRIPT_DIR}" >/dev/null 2>&1 || true

  if [ "${ids_inserted}" -eq 1 ]; then
    echo "cleanup: trice clean examples/TriceAbc"
    if ! run_trice clean \
      -til "${ROOT}/demoTIL.json" \
      -li "${ROOT}/demoLI.json" \
      -src "${SCRIPT_DIR}/NodeLib" \
      -src "${SCRIPT_DIR}/N1_tx" \
      -src "${SCRIPT_DIR}/N2_tx" \
      -src "${SCRIPT_DIR}/N3_bi" \
      -src "${SCRIPT_DIR}/N4_rx" \
      -src "${SCRIPT_DIR}/N5_rx" \
      -src "${SCRIPT_DIR}/N6_rx" \
      -src "${SCRIPT_DIR}/N7_bi" \
      -src "${SCRIPT_DIR}/N8_bi" \
      -src "${SCRIPT_DIR}/N9_bi"; then
      echo "warning: trice clean failed" >&2
      if [ "${status}" -eq 0 ]; then
        status=1
      fi
    fi
  fi

  exit "${status}"
}

trap 'cleanup $?' EXIT
trap 'cleanup 130' INT
trap 'cleanup 143' TERM

find_compiler() {
  if [ "${CC:-}" != "" ]; then
    if command -v "$CC" >/dev/null 2>&1; then
      printf '%s\n' "$CC"
      return 0
    fi
    echo "error: CC is set to '$CC', but that compiler was not found." >&2
    return 1
  fi

  for c in gcc clang cc; do
    if command -v "$c" >/dev/null 2>&1; then
      printf '%s\n' "$c"
      return 0
    fi
  done

  echo "error: no C compiler found in PATH." >&2
  return 1
}

build_node() {
  local name="$1"
  shift

  echo "build: ${name}"

  # BASE_CFLAGS is intentionally word-split into separate compiler options.
  # shellcheck disable=SC2086
  "${CC_BIN}" ${BASE_CFLAGS:-} -I"${SCRIPT_DIR}/${name}" "$@" -o "build/${name}${EXE_SUFFIX}"
}

CC_BIN="$(find_compiler)"
EXE_SUFFIX=""

BASE_CFLAGS="-std=c99 -Wall -Wextra -pedantic -O2 -I${ROOT}/src -I${SCRIPT_DIR} -I${SCRIPT_DIR}/NodeLib -I${SCRIPT_DIR}/BcSim"

if "${CC_BIN}" --version 2>/dev/null | grep -qi clang; then
  BASE_CFLAGS="${BASE_CFLAGS} -Wno-gnu-zero-variadic-macro-arguments"
fi

case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW* | MSYS* | CYGWIN*)
    EXE_SUFFIX=".exe"
    BASE_CFLAGS="${BASE_CFLAGS} -D_CRT_SECURE_NO_WARNINGS"
    ;;
esac

cd "${SCRIPT_DIR}"
mkdir -p build NodeLib

echo "prepare: trice insert examples/TriceAbc"
run_trice insert \
  -til "${ROOT}/demoTIL.json" \
  -li "${ROOT}/demoLI.json" \
  -src "${SCRIPT_DIR}/NodeLib" \
  -src "${SCRIPT_DIR}/N1_tx" \
  -src "${SCRIPT_DIR}/N2_tx" \
  -src "${SCRIPT_DIR}/N3_bi" \
  -src "${SCRIPT_DIR}/N4_rx" \
  -src "${SCRIPT_DIR}/N5_rx" \
  -src "${SCRIPT_DIR}/N6_rx" \
  -src "${SCRIPT_DIR}/N7_bi" \
  -src "${SCRIPT_DIR}/N8_bi" \
  -src "${SCRIPT_DIR}/N9_bi"

ids_inserted=1

echo "prepare: generate shared NodeLib/nodeAbc.{h,c}"
(
  cd "${ROOT}"
  run_trice generate -i "${ROOT}/demoTIL.json" -abc "examples/TriceAbc/NodeLib/nodeAbc"
)

echo "prepare: generate shared NodeLib/til.c"
rm -f "${SCRIPT_DIR}/NodeLib/til.c"

(
  cd "${ROOT}"

  # Since Issue #680, bare -tilC writes ./til.c. Pass the target path
  # explicitly so the demo always receives its private NodeLib/til.c.
  run_trice generate -i "${ROOT}/demoTIL.json" -tilC="examples/TriceAbc/NodeLib/til.c"
)

if [ ! -f "${SCRIPT_DIR}/NodeLib/til.c" ]; then
  echo "error: expected generated file ${SCRIPT_DIR}/NodeLib/til.c was not created." >&2
  exit 1
fi

COMMON_NODE_SOURCES="
	${SCRIPT_DIR}/BcSim/BcSim.c
	${SCRIPT_DIR}/NodeLib/node.c
"

TX_SOURCES="
	${ROOT}/src/trice.c
	${ROOT}/src/trice8.c
	${ROOT}/src/trice16.c
	${ROOT}/src/trice32.c
	${ROOT}/src/trice64.c
	${ROOT}/src/triceStackBuffer.c
	${ROOT}/src/triceX0.c
	${ROOT}/src/cobsEncode.c
	${ROOT}/src/tcobsv1Encode.c
"

RX_SOURCES="
	${ROOT}/src/triceRx.c
	${ROOT}/src/cobsDecode.c
	${SCRIPT_DIR}/NodeLib/nodeAbc.c
"

build_node N1_tx \
  "${SCRIPT_DIR}/N1_tx/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES}

build_node N2_tx \
  "${SCRIPT_DIR}/N2_tx/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES}

build_node N3_bi \
  "${SCRIPT_DIR}/N3_bi/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES} \
  ${RX_SOURCES}

build_node N4_rx \
  "${SCRIPT_DIR}/N4_rx/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${RX_SOURCES}

build_node N5_rx \
  "${SCRIPT_DIR}/N5_rx/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${RX_SOURCES}

build_node N6_rx \
  "${SCRIPT_DIR}/N6_rx/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${RX_SOURCES} \
  "${SCRIPT_DIR}/NodeLib/til.c"

build_node N7_bi \
  "${SCRIPT_DIR}/N7_bi/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES} \
  ${RX_SOURCES} \
  "${SCRIPT_DIR}/NodeLib/til.c"

build_node N8_bi \
  "${SCRIPT_DIR}/N8_bi/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES} \
  ${RX_SOURCES}

build_node N9_bi \
  "${SCRIPT_DIR}/N9_bi/main.c" \
  ${COMMON_NODE_SOURCES} \
  ${TX_SOURCES} \
  ${RX_SOURCES}

echo "built TriceAbc demo binaries in examples/TriceAbc/build/"
