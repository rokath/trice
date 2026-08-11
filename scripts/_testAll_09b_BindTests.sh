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

# run_verified_bind_pocs preserves the reference PoCs byte-for-byte. MinGW
# executables write CRLF to redirected stdout, so only the Windows comparison
# ignores that platform text-mode difference through a temporary diff shim.
run_verified_bind_pocs() {
  local poc_runner="$ROOT/experiments/TriceBind/30_Preprocessor_Verification/run_all.sh"
  local shim_dir=""
  local system_diff=""
  local status=0

  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      system_diff="$(command -v diff)" || return 1
      shim_dir="$(mktemp -d "$TRICE_TMP_DIR/trice-poc-diff.XXXXXX")" || return 1
      printf '%s\n' \
        '#!/usr/bin/env bash' \
        'exec "$TRICE_SYSTEM_DIFF" --strip-trailing-cr "$@"' >"$shim_dir/diff"
      chmod +x "$shim_dir/diff"
      run_cmd env TRICE_SYSTEM_DIFF="$system_diff" PATH="$shim_dir:$PATH" "$poc_runner" || status=$?
      case "$shim_dir" in
        "$TRICE_TMP_DIR"/trice-poc-diff.*) rm -rf -- "$shim_dir" ;;
        *) log "WARN: retained unexpected PoC diff shim: $shim_dir" ;;
      esac
      return "$status"
      ;;
    *) run_cmd "$poc_runner" ;;
  esac
}

main() {
  local poc_dir="$ROOT/experiments/TriceBind/40_MVP_Generator"
  local poc_build="$poc_dir/build"
  local poc_executable="$poc_build/bin/PoC_bind_generator"
  local go_executable

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
  # Pass the executable selected through PATH back to CMake on every configure
  # run. This replaces an obsolete find_program cache entry after Go was moved,
  # replaced, or installed at a different path on another supported host.
  go_executable="$(command -v go)" || {
    log "FAIL: cannot resolve the Go executable from PATH"
    exit 1
  }

  run_cmd env TRICE_BIND_INTEGRATION=1 go test ./internal/id \
    -run '^TestBind(GeneratedTargetCompilesCAndCPP|CanonicalTriceCheckGeneratesCompleteSidecar|MVP2RebaseCompilesCAndCPP|MVP2CounterGuardsAndGeneratedInvariants|MVP2RebaseEmitsStableRuntimeIDs)$' \
    -count=1 || {
    log "FAIL: generated target, canonical, or MVP2 Trice bind integration failed"
    exit 1
  }

  run_verified_bind_pocs || {
    log "FAIL: verified Trice bind preprocessor mechanisms regressed"
    exit 1
  }

  run_cmd "$ROOT/experiments/TriceBind/60_MVP2_Local_Counter_Rebase/run.sh" || {
    log "FAIL: verified local-counter Rebase PoC regressed"
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
    -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG="$poc_build/bin" \
    -DGO_EXECUTABLE:FILEPATH="$go_executable" || {
    log "FAIL: cannot configure experiments/TriceBind/40_MVP_Generator"
    exit 1
  }
  run_cmd cmake --build "$poc_build" --config Debug --parallel || {
    log "FAIL: cannot build experiments/TriceBind/40_MVP_Generator"
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
  run_cmd cmake -E chdir "$poc_dir" go run ../../../cmd/trice log \
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
