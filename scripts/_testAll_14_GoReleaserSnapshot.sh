#!/usr/bin/env bash
#
# Step 14: Generates all local GoReleaser snapshot assets into ./dist/.
#
# Direct invocation:
# - ./_testAll_14_GoReleaserSnapshot.sh
#
# Log file:
# - ./temp/log/_testAll_14_GoReleaserSnapshot.log

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/_testAll_00_common.sh"
ROOT_DIR="$(cd -- "$SCRIPT_DIR/.." && pwd)"
DIST_DIR="$ROOT_DIR/dist"

dump_release_artifact_context() {
  log "INFO: generated files under ./dist and ./docs:"
  if [ -d "$DIST_DIR" ]; then
    find "$DIST_DIR" -maxdepth 4 -type f -print 2>/dev/null | sort | while IFS= read -r line; do
      log "INFO: $line"
    done
  else
    log "INFO: ./dist does not exist"
  fi
  if [ -d "$ROOT_DIR/docs" ]; then
    find "$ROOT_DIR/docs" -maxdepth 2 \( -name 'TriceUserManual.md' -o -name 'TriceUserManual.pdf' \) -type f -print 2>/dev/null | sort | while IFS= read -r line; do
      log "INFO: $line"
    done
  else
    log "INFO: ./docs does not exist"
  fi
}

verify_release_artifact() {
  local artifact="$1"
  if [ ! -s "$artifact" ]; then
    log "FAIL: missing or empty release artifact: $artifact"
    dump_release_artifact_context
    exit 1
  fi
  log "OK: $artifact ($(wc -c <"$artifact") bytes)"
}

verify_release_glob() {
  local pattern="$1"
  local matches=()
  local match

  while IFS= read -r match; do
    matches+=("$match")
  done < <(compgen -G "$pattern" | sort || true)

  if [ "${#matches[@]}" -eq 0 ]; then
    log "FAIL: no release artifact matches: $pattern"
    dump_release_artifact_context
    exit 1
  fi

  log "OK: $pattern"
  printf '%s\n' "${matches[@]}" | while IFS= read -r match; do
    log "OK:   $match"
  done
}

verify_snapshot_layout() {
  verify_release_artifact "$DIST_DIR/trice_tool_linux_amd64.tar.gz"
  verify_release_artifact "$DIST_DIR/trice_tool_darwin_amd64.tar.gz"
  verify_release_artifact "$DIST_DIR/trice_tool_windows_amd64.zip"
  verify_release_artifact "$DIST_DIR/TriceUserManual.pdf"

  local source_zip
  source_zip="$(find "$DIST_DIR" -maxdepth 1 -name 'trice_target_sources_*.zip' -print -quit)"
  if [ -z "$source_zip" ]; then
    log "FAIL: missing target source archive in ./dist/"
    exit 1
  fi

  verify_release_glob "$DIST_DIR/trice_*_amd64.deb"
  verify_release_glob "$DIST_DIR/trice_*_amd64.rpm"
  verify_release_glob "$DIST_DIR/trice_*_amd64.apk"
}

smoke_test_host_archive() {
  local unpack_dir="$ROOT_DIR/temp/testAll-release-archive"
  local uname_s
  local uname_m
  local host_os
  local host_arch
  local archive
  local archive_dir
  local trice_bin
  local trice_name
  local output

  uname_s="$(uname -s)"
  uname_m="$(uname -m)"

  case "$uname_s" in
    Darwin)
      host_os="darwin"
      trice_name="trice"
      ;;
    Linux)
      host_os="linux"
      trice_name="trice"
      ;;
    MINGW* | MSYS* | CYGWIN*)
      host_os="windows"
      trice_name="trice.exe"
      ;;
    *)
      log "SKIP: unsupported host OS for release archive smoke test: $uname_s"
      return 0
      ;;
  esac

  case "$uname_m" in
    x86_64 | amd64)
      host_arch="amd64"
      ;;
    arm64 | aarch64)
      host_arch="arm64"
      ;;
    armv7*)
      host_arch="arm7"
      ;;
    armv6*)
      host_arch="arm6"
      ;;
    *)
      log "SKIP: unsupported host architecture for release archive smoke test: $uname_m"
      return 0
      ;;
  esac

  archive_dir="trice_tool_${host_os}_${host_arch}"

  case "$host_os" in
    windows)
      archive="$DIST_DIR/${archive_dir}.zip"
      ;;
    *)
      archive="$DIST_DIR/${archive_dir}.tar.gz"
      ;;
  esac

  if [ ! -s "$archive" ]; then
    log "FAIL: missing host release archive for smoke test: $archive"
    dump_release_artifact_context
    exit 1
  fi

  rm -rf "$unpack_dir"
  mkdir -p "$unpack_dir"

  case "$host_os" in
    windows)
      unzip -q "$archive" -d "$unpack_dir"
      ;;
    *)
      tar -xzf "$archive" -C "$unpack_dir"
      ;;
  esac

  trice_bin="$unpack_dir/$archive_dir/$trice_name"

  if [ ! -f "$trice_bin" ]; then
    log "FAIL: unpacked host archive does not contain trice binary: $trice_bin"
    exit 1
  fi

  if [ "$host_os" != "windows" ] && [ ! -x "$trice_bin" ]; then
    log "FAIL: unpacked host archive does not contain an executable trice binary: $trice_bin"
    exit 1
  fi

  output="$("$trice_bin" version 2>&1)" || {
    log "FAIL: smoke test failed: trice version"
    log "$output"
    exit 1
  }

  output="$("$trice_bin" help 2>&1)" || {
    log "FAIL: smoke test failed: trice help"
    log "$output"
    exit 1
  }

  output="$("$trice_bin" log -port HEX -args '09 92 19 06 45 0b 10 56 3a,00' -pw MySecret -pf cobs -li off -hs off -color none -prefix off -ts off -i .github/fixtures/trice-smoke-til.json 2>&1)" || {
    log "FAIL: smoke test failed: trice log"
    log "$output"
    exit 1
  }

  case "$output" in
    *"Hello!"*) ;;
    *)
      log "FAIL: decoded smoke output did not contain Hello!"
      log "$output"
      exit 1
      ;;
  esac
}

compile_target_sources() {
  local source_zip
  local target_parent
  local target_root
  local source_file
  local c_compiler

  source_zip="$(find "$DIST_DIR" -maxdepth 1 -name 'trice_target_sources_*.zip' -print -quit)"
  if [ -z "$source_zip" ] || [ ! -s "$source_zip" ]; then
    log "FAIL: missing target source archive in ./dist/"
    exit 1
  fi

  target_parent="$ROOT_DIR/temp/testAll-target-sources"
  rm -rf "$target_parent"
  mkdir -p "$target_parent"

  unzip -q "$source_zip" -d "$target_parent"

  target_root="$(find "$target_parent" -maxdepth 1 -type d -name 'trice_target_sources_*' -print -quit)"
  if [ -z "$target_root" ]; then
    log "FAIL: could not locate unpacked target sources"
    exit 1
  fi

  c_compiler="${CC:-}"
  if [ -z "$c_compiler" ]; then
    if command -v cc >/dev/null 2>&1; then
      c_compiler="cc"
    elif command -v gcc >/dev/null 2>&1; then
      c_compiler="gcc"
    elif command -v clang >/dev/null 2>&1; then
      c_compiler="clang"
    fi
  fi

  if [ -z "$c_compiler" ] || ! command -v "$c_compiler" >/dev/null 2>&1; then
    log "FAIL: C compiler not found"
    log "Hint: install gcc/clang or run with CC=gcc $0"
    exit 1
  fi

  cat >"$target_root/src/triceConfig.h" <<'EOF'
#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_AUXILIARY8 1
#define TRICE_CYCLE_COUNTER 0
#endif
EOF

  for source_file in "$target_root"/src/*.c; do
    case "$(basename "$source_file")" in
      SEGGER_RTT.c | triceUart.c)
        continue
        ;;
    esac
    "$c_compiler" -I "$target_root/src" -c "$source_file" -o "/tmp/$(basename "$source_file").o"
  done
}

verify_release_pdf() {
  local size

  verify_release_artifact "$DIST_DIR/TriceUserManual.pdf"
  size="$(wc -c <"$DIST_DIR/TriceUserManual.pdf")"
  if [ "$size" -le 100000 ]; then
    log "FAIL: release PDF looks too small ($size bytes)"
    exit 1
  fi
}

main() {
  init_logfile

  if ! has_command goreleaser; then
    log "SKIP: goreleaser not installed locally; skipping local release-asset build"
    exit 0
  fi

  if ! has_command npx; then
    log "SKIP: npx not installed locally; skipping local release-asset build"
    log "INFO: The local GoReleaser snapshot needs Node.js because the manual PDF is generated headlessly."
    exit 0
  fi

  run_cmd goreleaser release --snapshot --clean || {
    log "FAIL: local GoReleaser snapshot build failed"
    exit 1
  }

  if [ ! -s "$ROOT_DIR/docs/TriceUserManual.pdf" ]; then
    log "FAIL: docs/TriceUserManual.pdf was not generated or is empty"
    dump_release_artifact_context
    exit 1
  fi

  # GoReleaser uploads docs/TriceUserManual.pdf as a release extra_file. The
  # local snapshot check also copies it to ./dist so the expected artifact set
  # is visible in one directory for smoke tests and CI artifact upload.
  run_cmd mkdir -p "$ROOT_DIR/dist" || {
    log "FAIL: could not ensure ./dist/ exists for the local manual PDF copy"
    exit 1
  }
  run_cmd cp -f "$ROOT_DIR/docs/TriceUserManual.pdf" "$ROOT_DIR/dist/TriceUserManual.pdf" || {
    log "FAIL: could not copy docs/TriceUserManual.pdf into ./dist/"
    exit 1
  }

  verify_snapshot_layout
  smoke_test_host_archive
  compile_target_sources
  verify_release_pdf

  log "OK: local release artifacts generated under ./dist/"
}

main "$@"
