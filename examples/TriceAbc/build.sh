#!/usr/bin/env bash
# Usage: ./build.sh. Build nine native PC nodes; no embedded hardware is needed.
# Requires Bash and a C compiler, plus Go or an installed Trice executable.
# Use CC=clang ./build.sh to select a compiler explicitly.
# Select Bind with TRICE_ID_WORKFLOW=bind; the default remains Insert/Clean.
# The preparation/cleanup helpers come first; generation and compilation follow.
set -euo pipefail

# build.sh
#
# Build every TriceAbc demo node in a host-native way.
#
# The script keeps three responsibilities together because they depend on the
# same exact source state:
# 1. prepare IDs in private build copies of the demo sources,
# 2. generate the shared ABC and TIL-C helper files,
# 3. compile the nine demo binaries.
#
# After a successful insert step the script always runs `trice clean` during
# cleanup of the private copies. Original sources remain unchanged in both modes.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

# Use the shared ID files and store paths relative to the repository root.
TRICE_TIL_JSON=${TRICE_TIL_JSON:-"${ROOT}/demoTIL.json"}
TRICE_LI_JSON=${TRICE_LI_JSON:-"${ROOT}/demoLI.json"}
TRICE_LI_ROOT=${TRICE_LI_ROOT:-"${ROOT}"}
# Resolve relative overrides against the repository root, independently of
# demo.sh's directory change. All phases must use the same TIL/LI files.
for metadata_variable in TRICE_TIL_JSON TRICE_LI_JSON TRICE_LI_ROOT; do
  metadata_path=${!metadata_variable}
  case "$metadata_path" in
    off | none | /* | [[:alpha:]]:*) ;;
    *) printf -v "$metadata_variable" '%s/%s' "$ROOT" "$metadata_path" ;;
  esac
done
source "${ROOT}/scripts/_120_setup_trice_environment.sh"

# Both workflows use disposable source copies, so switching workflows never
# needs to remove Bind ownership or inline IDs from the user's original files.
SOURCE_DIR="${SCRIPT_DIR}/build/source"
ID_WORKFLOW=${TRICE_ID_WORKFLOW:-insert}
case "$ID_WORKFLOW" in
  bind | insert) ;;
  *)
    echo "error: TRICE_ID_WORKFLOW must be bind or insert, got '$ID_WORKFLOW'." >&2
    exit 1
    ;;
esac
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
      -til "${TRICE_TIL_JSON}" \
      -li "${TRICE_LI_JSON}" \
      -liRoot "${TRICE_LI_ROOT}" \
      "${ID_SOURCES[@]}"; then
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

  # Arrays preserve include and source paths when the checkout contains spaces.
  # Each node's configuration precedes shared and generated library headers.
  "${CC_BIN}" -I"${SOURCE_DIR}/${name}" "${BASE_CFLAGS[@]}" "$@" \
    "${ROOT}"/src/*.c "$link_unused" -o "build/${name}${EXE_SUFFIX}"
}

CC_BIN="$(find_compiler)"
EXE_SUFFIX=""

# Project headers come first; library headers and fallback configuration come last.
BASE_CFLAGS=(-std=c99 -Wall -Wextra -pedantic -O2 -ffunction-sections -fdata-sections
  "-I${SOURCE_DIR}" "-I${SOURCE_DIR}/NodeLib" "-I${SOURCE_DIR}/BcSim"
  "-I${SCRIPT_DIR}/build/sidecars" "-I${ROOT}/src" "-I${ROOT}/src/default_conf")

# Every node compiles src/*.c; its configuration selects TX/RX features.
# Details: ../../docs/rm/memory_usage.md
link_unused=-Wl,--gc-sections
case "$(uname -s)" in Darwin) link_unused=-Wl,-dead_strip ;; esac

case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW* | MSYS* | CYGWIN*)
    EXE_SUFFIX=".exe"
    BASE_CFLAGS+=(-D_CRT_SECURE_NO_WARNINGS)
    ;;
esac

cd "${SCRIPT_DIR}"
mkdir -p build NodeLib

# Recreate only our generated source tree; deleted application files must not
# survive from an older build. Keep the user-owned ABC selection header intact.
rm -rf "${SOURCE_DIR}"
mkdir -p "${SOURCE_DIR}"
ID_SOURCES=()
for directory in BcSim NodeLib N1_tx N2_tx N3_bi N4_rx N5_rx N6_rx N7_bi N8_bi N9_bi; do
  cp -R "${SCRIPT_DIR}/${directory}" "${SOURCE_DIR}/${directory}"
  if [ "$directory" != BcSim ]; then
    ID_SOURCES+=(-src "${SOURCE_DIR}/${directory}")
  fi
done

# Remove the previous generated format table before scanning NodeLib. Its
# quoted message text is data, not source calls; the table is recreated below.
rm -f "${SOURCE_DIR}/NodeLib/til.c" "${SOURCE_DIR}/NodeLib/nodeAbc.c"

echo "prepare: trice ${ID_WORKFLOW} examples/TriceAbc (private build sources)"
trice_prepare_location_file
BIND_OPTIONS=()
if [ "$ID_WORKFLOW" = bind ]; then
  BIND_OPTIONS=(-genDir "${SCRIPT_DIR}/build/sidecars")
fi
run_trice "$ID_WORKFLOW" \
  -til "${TRICE_TIL_JSON}" \
  -li "${TRICE_LI_JSON}" \
  -liRoot "${TRICE_LI_ROOT}" \
  "${ID_SOURCES[@]}" ${BIND_OPTIONS[@]+"${BIND_OPTIONS[@]}"}

if [ "$ID_WORKFLOW" = insert ]; then ids_inserted=1; fi

echo "prepare: generate shared NodeLib/nodeAbc.{h,c}"
(
  cd "${ROOT}"
  # A bare target uses -genDir directly; an explicit ABC path is relative to
  # the TIL directory and would be wrong with an independently selected TIL.
  run_trice generate -i "${TRICE_TIL_JSON}" -abc nodeAbc -genDir "${SOURCE_DIR}/NodeLib"
)

echo "prepare: generate shared NodeLib/til.c"

(
  cd "${ROOT}"

  # logC filters the historical TIL through the prepared sources and sidecars and
  # writes only metadata used by the current TriceAbc programs.
  run_trice generate -i "${TRICE_TIL_JSON}" -src "${SOURCE_DIR}" \
    ${BIND_OPTIONS[@]+"${BIND_OPTIONS[@]}"} -logC="${SOURCE_DIR}/NodeLib/til.c"
)

for generated_table in til.c nodeAbc.c; do
  if [ ! -s "${SOURCE_DIR}/NodeLib/${generated_table}" ]; then
    echo "error: expected nonempty generated table ${SOURCE_DIR}/NodeLib/${generated_table} was not created." >&2
    exit 1
  fi
done

COMMON_NODE_SOURCES=("${SOURCE_DIR}/BcSim/BcSim.c" "${SOURCE_DIR}/NodeLib/node.c")

# Only receive-capable nodes need the generated application dispatch table.
# Library sources are already included by build_node above.
RX_SOURCES=("${SOURCE_DIR}/NodeLib/nodeAbc.c")

build_node N1_tx \
  "${SOURCE_DIR}/N1_tx/main.c" \
  "${COMMON_NODE_SOURCES[@]}"

build_node N2_tx \
  "${SOURCE_DIR}/N2_tx/main.c" \
  "${COMMON_NODE_SOURCES[@]}"

build_node N3_bi \
  "${SOURCE_DIR}/N3_bi/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}"

build_node N4_rx \
  "${SOURCE_DIR}/N4_rx/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}"

build_node N5_rx \
  "${SOURCE_DIR}/N5_rx/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}"

build_node N6_rx \
  "${SOURCE_DIR}/N6_rx/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}" \
  "${SOURCE_DIR}/NodeLib/til.c"

build_node N7_bi \
  "${SOURCE_DIR}/N7_bi/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}" \
  "${SOURCE_DIR}/NodeLib/til.c"

build_node N8_bi \
  "${SOURCE_DIR}/N8_bi/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}"

build_node N9_bi \
  "${SOURCE_DIR}/N9_bi/main.c" \
  "${COMMON_NODE_SOURCES[@]}" \
  "${RX_SOURCES[@]}"

echo "built TriceAbc demo binaries in examples/TriceAbc/build/"
