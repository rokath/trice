#!/usr/bin/env bash
#
# Test 515: Productive CE/SL end-to-end checks and isolated feature examples.
# Usage: ./scripts/_515_test_logging_features.sh [quick|full] [--quiet]
# Full/CI requires every tool; quick reports unavailable groups as skipped.

set -u
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"

# A fresh copy contains current tracked bytes, including local source edits,
# but no previous build outputs. Failed copies remain beside the detailed log.
FEATURE_COPY_DIR=""

# logging_tools checks a whole group before execution. A missing prerequisite
# must never turn a full release check into successful but incomplete evidence.
logging_tools() {
  local group="$1" tool missing=0
  shift
  for tool in "$@"; do
    if ! has_command "$tool"; then
      log "MISSING TOOL: $tool ($group)"
      missing=1
    fi
  done
  if [ "$missing" -ne 0 ]; then
    if [ "$SELECTED" = "full" ]; then
      log "FAIL: $group requires the missing tools listed above; install them and rerun this step full"
    else
      log "SKIP: $group; install the missing tools and rerun this step"
    fi
    return 1
  fi
}

# run_logging_go_checks selects existing tests without repeating the ordinary
# unit suite. Check named PASS records too: Go exits successfully for an empty
# selection, and a skipped parent/subtest is not completed release evidence.
run_logging_go_checks() {
  local package="$1" name status expression=""
  shift
  for name in "$@"; do expression="${expression:+$expression|}$name"; done
  run_cmd env TRICE_BIND_INTEGRATION=1 go test "$package" \
    -run "^($expression)$" -count=1 -v || {
    status=$?
    log "FAIL: CE/SL integration failed in $package"
    return "$status"
  }
  if grep -Eq '^[[:space:]]*--- SKIP:' "$LOGFILE"; then
    log "FAIL: a selected CE/SL integration test was skipped; see the test output above"
    return 1
  fi
  for name in "$@"; do
    if ! grep -Fq -- "--- PASS: $name (" "$LOGFILE"; then
      log "FAIL: required test $name did not report PASS; check the selection and prerequisites"
      return 1
    fi
  done
}

# create_feature_copy uses the index only as a file list, then archives the
# worktree bytes. Do not use git archive HEAD: that would omit uncommitted fixes.
# The copied directory layout keeps the examples' relative source paths valid.
create_feature_copy() {
  if [ -n "$FEATURE_COPY_DIR" ]; then return 0; fi
  FEATURE_COPY_DIR="$(mktemp -d "$LOG_DIR/logging-features.XXXXXX")" || return 1
  log "INFO: isolated feature examples (retained on failure): $FEATURE_COPY_DIR"
  mkdir -p "$FEATURE_COPY_DIR/project" || return 1
  git ls-files -z -- src examples/PC_features examples/G0B1_features examples/exampleData \
    >"$FEATURE_COPY_DIR/paths" || {
    log "FAIL: cannot enumerate tracked feature-example sources"
    return 1
  }
  run_cmd tar -cf "$FEATURE_COPY_DIR/sources.tar" --null -T "$FEATURE_COPY_DIR/paths" || return 1
  run_cmd tar -xf "$FEATURE_COPY_DIR/sources.tar" -C "$FEATURE_COPY_DIR/project" || return 1
}

# run_feature_example never instruments the original checkout. In particular,
# G0B1's shared exampleData sources and pre-existing generated files stay intact.
run_feature_example() {
  local relative="$1" status
  create_feature_copy || return 1
  run_cmd "$FEATURE_COPY_DIR/project/$relative" || {
    status=$?
    log "FAIL: $relative; isolated sources and outputs retained in $FEATURE_COPY_DIR"
    return "$status"
  }
}

main() {
  local pc_compiler=cc artifact
  SELECTED="$(get_mode "${1:-$SELECTED}")" || return 2
  init_logfile

  if logging_tools "CE/SL compiler and decoder integration" go clang clang++ clangd; then
    log "INFO: productive CE/SL: Bind and Insert/Clean, C/C++, text/JSON/KV, disabled logging and single evaluation"
    run_logging_go_checks ./internal/args \
      TestContextEnrichmentTargetToDecoder TestContextInsertCleanTargetToDecoder || return $?
    log "INFO: retained CE proofs: direct callsites and the unsupported Rebase scope boundary"
    run_logging_go_checks ./internal/id \
      TestContextEnrichmentPoC TestContextEnrichmentPoCRebaseScopeBoundary || return $?
  elif [ "$SELECTED" = "full" ]; then
    return 1
  fi
  log "INFO: experimental TestContextEnrichmentRebasePoC is a separate opt-in proof, not productive CE support; see UM Testing"

  has_command cc || pc_compiler=gcc
  if logging_tools "PC feature example" git tar trice "$pc_compiler"; then
    run_feature_example examples/PC_features/check_output.sh || return $?
  elif [ "$SELECTED" = "full" ]; then
    return 1
  fi

  if logging_tools "G0B1 feature build" git tar trice make arm-none-eabi-gcc arm-none-eabi-objcopy arm-none-eabi-size; then
    run_feature_example examples/G0B1_features/check_build.sh || return $?
    for artifact in G0B1.elf G0B1.hex G0B1.bin; do
      if [ ! -s "$FEATURE_COPY_DIR/project/examples/G0B1_features/out.gcc/$artifact" ]; then
        log "FAIL: expected nonempty feature-build artifact: examples/G0B1_features/out.gcc/$artifact"
        log "INFO: inspect the retained build in $FEATURE_COPY_DIR"
        return 1
      fi
    done
    log "PASS: G0B1 feature firmware built; MCU execution requires a separate board test"
  elif [ "$SELECTED" = "full" ]; then
    return 1
  fi

  if [ -n "$FEATURE_COPY_DIR" ]; then
    # Only remove the private tree created above, after every available check
    # succeeded. The detailed step log remains in LOG_DIR for later inspection.
    case "$FEATURE_COPY_DIR" in
      "$LOG_DIR"/logging-features.*) rm -rf -- "$FEATURE_COPY_DIR" || return 1 ;;
      *)
        log "FAIL: unexpected feature-copy path retained: $FEATURE_COPY_DIR"
        return 1
        ;;
    esac
  fi
}

main "$@"
