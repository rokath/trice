#!/usr/bin/env bash
#
# Step 09b: Runs Trice bind generator, target, PoC, and verified preprocessor checks.
#
# Direct invocation:
# - ./scripts/_testAll_09b_BindTests.sh
#
# Log file:
# - ./temp/log/_testAll_09b_BindTests.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

# has_bind_compilers reports whether both required language frontends are available.
has_bind_compilers() {
  local c_found=0
  local cpp_found=0
  local compiler
  for compiler in cc gcc clang; do
    if has_command "$compiler"; then
      c_found=1
      break
    fi
  done
  for compiler in c++ g++ clang++; do
    if has_command "$compiler"; then
      cpp_found=1
      break
    fi
  done
  [ "$c_found" -eq 1 ] && [ "$cpp_found" -eq 1 ]
}

main() {
  local poc_dir="$ROOT/examples/PoC_bind_generator"
  local poc_build="$poc_dir/build"
  local poc_executable="$poc_build/bin/PoC_bind_generator"

  init_logfile
  if ! has_command go; then
    log "MISSING TOOL: go"
    log "SKIP: Trice bind tests require Go"
    exit 0
  fi
  if ! has_bind_compilers; then
    log "MISSING TOOL: compatible C and C++ compilers"
    log "SKIP: Trice bind target integration requires GCC- or Clang-compatible frontends"
    exit 0
  fi

  run_cmd env TRICE_BIND_INTEGRATION=1 go test ./internal/id \
    -run '^TestBind(GeneratedTargetCompilesCAndCPP|CanonicalTriceCheckGeneratesCompleteSidecar)$' \
    -count=1 || {
    log "FAIL: generated target or canonical Trice bind integration failed"
    exit 1
  }

  run_cmd "$ROOT/docs/scratchPad/TriceBind/Trice_bind_Verification_PoCs/run_all.sh" || {
    log "FAIL: verified Trice bind preprocessor mechanisms regressed"
    exit 1
  }

  if ! has_command cmake; then
    log "MISSING TOOL: cmake"
    log "SKIP: Generator PoC build not installed; generator and target integration already passed"
    exit 0
  fi

  run_cmd cmake -S "$poc_dir" -B "$poc_build" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$poc_build/bin" \
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG="$poc_build/bin" || {
    log "FAIL: cannot configure examples/PoC_bind_generator"
    exit 1
  }
  run_cmd cmake --build "$poc_build" --config Debug --parallel || {
    log "FAIL: cannot build examples/PoC_bind_generator"
    exit 1
  }

  if [ -f "$poc_executable.exe" ]; then
    poc_executable="$poc_executable.exe"
  fi
  if [ ! -f "$poc_executable" ]; then
    log "FAIL: expected PoC executable at $poc_build/bin/PoC_bind_generator[.exe]"
    log "Generated build entries:"
    find "$poc_build" -type f -name 'PoC_bind_generator*' -print | log_pipe
    exit 1
  fi

  run_cmd cmake -E chdir "$poc_dir" "$poc_executable" || {
    log "FAIL: generated bind PoC executable failed"
    exit 1
  }
  run_cmd cmake -E chdir "$poc_dir" go run ../../cmd/trice log \
    -p FILEBUFFER -args log.bin -pf TCOBS -d16 -prefix off -hs off \
    -i til.json -li li.json -color none || {
    log "FAIL: generated bind PoC log could not be decoded"
    exit 1
  }
  if grep_log 'ERROR:' "$LOGFILE"; then
    log "FAIL: generated bind PoC decoder reported malformed data"
    exit 2
  fi
}

main "$@"
