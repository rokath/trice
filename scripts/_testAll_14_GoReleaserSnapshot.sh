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

verify_release_artifact() {
  local artifact="$1"
  if [ ! -s "$artifact" ]; then
    log "FAIL: missing release artifact: $artifact"
    exit 1
  fi
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

  if ! compgen -G "$DIST_DIR/trice_*_amd64.deb" >/dev/null; then
    log "FAIL: missing Debian package in ./dist/"
    exit 1
  fi
  if ! compgen -G "$DIST_DIR/trice_*_amd64.rpm" >/dev/null; then
    log "FAIL: missing RPM package in ./dist/"
    exit 1
  fi
  if ! compgen -G "$DIST_DIR/trice_*_amd64.apk" >/dev/null; then
    log "FAIL: missing APK package in ./dist/"
    exit 1
  fi
}

smoke_test_linux_archive() {
  local unpack_dir="$ROOT_DIR/temp/testAll-release-archive"
  local trice_bin
  rm -rf "$unpack_dir"
  mkdir -p "$unpack_dir"
  tar -xzf "$DIST_DIR/trice_tool_linux_amd64.tar.gz" -C "$unpack_dir"
  trice_bin="$unpack_dir/trice_tool_linux_amd64/trice"
  if [ ! -x "$trice_bin" ]; then
    log "FAIL: unpacked Linux archive does not contain an executable trice binary"
    exit 1
  fi
  "$trice_bin" version
  "$trice_bin" help
  local output
  output="$("$trice_bin" log -port HEX -args '09 92 19 06 45 0b 10 56 3a,00' -pw MySecret -pf cobs -li off -hs off -color none -prefix off -ts off -i .github/fixtures/trice-smoke-til.json)"
  printf '%s\n' "$output"
  case "$output" in
  *"Hello!"*) ;;
  *)
    log "FAIL: decoded smoke output did not contain Hello!"
    exit 1
    ;;
  esac
}

compile_target_sources() {
  local source_zip
  local target_root
  local source_file

  source_zip="$(find "$DIST_DIR" -maxdepth 1 -name 'trice_target_sources_*.zip' -print -quit)"
  unzip -q "$source_zip" -d "$ROOT_DIR/temp/testAll-target-sources"
  target_root="$(find "$ROOT_DIR/temp/testAll-target-sources" -maxdepth 1 -type d -name 'trice_target_sources_*' -print -quit)"
  if [ -z "$target_root" ]; then
    log "FAIL: could not locate unpacked target sources"
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
    cc -I "$target_root/src" -c "$source_file" -o "/tmp/$(basename "$source_file").o"
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

  if [ -f "$ROOT_DIR/temp/release/TriceUserManual.pdf" ]; then
    run_cmd mkdir -p "$ROOT_DIR/dist" || {
      log "FAIL: could not ensure ./dist/ exists for the local manual PDF copy"
      exit 1
    }
    run_cmd cp -f "$ROOT_DIR/temp/release/TriceUserManual.pdf" "$ROOT_DIR/dist/TriceUserManual.pdf" || {
      log "FAIL: could not copy the generated manual PDF into ./dist/"
      exit 1
    }
  else
    log "WARN: temp/release/TriceUserManual.pdf was not generated; dist/ will not contain the manual PDF"
  fi

  verify_snapshot_layout
  smoke_test_linux_archive
  compile_target_sources
  verify_release_pdf

  log "OK: local release artifacts generated under ./dist/"
}

main "$@"
