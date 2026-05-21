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

  mapfile -t matches < <(compgen -G "$pattern" | sort || true)

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
  smoke_test_linux_archive
  compile_target_sources
  verify_release_pdf

  log "OK: local release artifacts generated under ./dist/"
}

main "$@"
