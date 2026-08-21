#!/usr/bin/env bash
#
# Transactional state ownership for Insert/Bind test wrappers. This file is
# sourced and is not a standalone command.

set -u

TRICE_STATE_SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
TRICE_STATE_ROOT="$(cd -- "$TRICE_STATE_SCRIPT_DIR/.." && pwd)"

# shellcheck source=./_130_trice_id_workflow.sh
source "$TRICE_STATE_SCRIPT_DIR/_130_trice_id_workflow.sh"

# trice_state_log uses the testAll logger when available and stays useful for
# direct workflow-wrapper invocation.
trice_state_log() {
  if declare -F log >/dev/null 2>&1; then
    log "$*"
  else
    printf '%s\n' "$*"
  fi
}

# trice_state_run_command preserves the command line in the workflow log and
# delegates output capture to testAll when its run_cmd helper is available.
trice_state_run_command() {
  if declare -F run_cmd >/dev/null 2>&1; then
    run_cmd "$@"
  else
    printf '+ %s\n' "$*"
    "$@"
  fi
}

# trice_state_relative_path validates one repository-relative path and prints a
# normalized form suitable for a tar manifest. Parent traversal is rejected so
# restore-time deletion can never escape the repository.
trice_state_relative_path() {
  local path="${1#./}"
  local description="$2"

  case "$path" in
    "" | /* | [A-Za-z]:* | .. | ../* | */../* | */..)
      trice_state_log "FAIL: $description must stay inside the repository: $1"
      return 1
      ;;
  esac
  printf '%s\n' "$path"
}

# trice_state_append_source_files expands the shared -src options with the same
# C/C++ extensions accepted by internal/id and appends paths to the raw manifest.
trice_state_append_source_files() {
  local raw_manifest="$1"
  local option
  local source
  local relative_source

  # The established TRICE_PRJ_FILES interface is a shell option string. Its
  # paths therefore follow the same no-embedded-whitespace convention as the
  # public Insert/Clean helpers.
  # shellcheck disable=SC2086
  set -- $TRICE_PRJ_FILES
  while [ "$#" -gt 0 ]; do
    option="$1"
    shift
    if [ "$option" != "-src" ]; then
      trice_state_log "FAIL: unsupported shared project option while snapshotting: $option"
      return 1
    fi
    if [ "$#" -eq 0 ]; then
      trice_state_log "FAIL: missing path after -src in TRICE_PRJ_FILES"
      return 1
    fi
    source="$1"
    shift
    relative_source="$(trice_state_relative_path "$source" "source path")" || return 1
    if [ ! -e "$TRICE_STATE_ROOT/$relative_source" ]; then
      trice_state_log "FAIL: managed source path does not exist: $relative_source"
      return 1
    fi
    if [ -f "$TRICE_STATE_ROOT/$relative_source" ]; then
      printf '%s\n' "$relative_source" >>"$raw_manifest"
      continue
    fi
    if [ ! -d "$TRICE_STATE_ROOT/$relative_source" ]; then
      trice_state_log "FAIL: managed source path is neither a file nor directory: $relative_source"
      return 1
    fi
    (
      cd "$TRICE_STATE_ROOT" || exit 1
      find "$relative_source" -type f \( \
        -name '*.c' -o -name '*.cc' -o -name '*.cp' -o -name '*.cxx' -o \
        -name '*.cpp' -o -name '*.CPP' -o -name '*.c++' -o -name '*.C' -o \
        -name '*.h' -o -name '*.hh' -o -name '*.hp' -o -name '*.hxx' -o \
        -name '*.hpp' -o -name '*.HPP' -o -name '*.h++' -o -name '*.H' -o \
        -name '*.ixx' -o -name '*.inc' \
        \) -print
    ) >>"$raw_manifest" || return 1
  done
}

# trice_state_append_optional_file records whether a list existed initially and
# archives it only when there are bytes or metadata to restore.
trice_state_append_optional_file() {
  local path="$1"
  local description="$2"
  local raw_manifest="$3"
  local absent_manifest="$4"
  local relative

  if [ "$path" = "off" ] || [ "$path" = "none" ]; then
    return 0
  fi
  relative="$(trice_state_relative_path "$path" "$description")" || return 1
  if [ -e "$TRICE_STATE_ROOT/$relative" ] || [ -L "$TRICE_STATE_ROOT/$relative" ]; then
    printf '%s\n' "$relative" >>"$raw_manifest"
  else
    printf '%s\n' "$relative" >>"$absent_manifest"
  fi
}

# trice_test_state_snapshot captures every file that Insert, Clean, Bind, or
# re-migration may alter. It also claims sole ownership of the ID lifecycle.
trice_test_state_snapshot() {
  local state_parent
  local raw_manifest
  local relative_bind_dir

  if [ "${TRICE_TEST_STATE_ACTIVE:-0}" -eq 1 ]; then
    trice_state_log "FAIL: a Trice test-state owner is already active"
    return 1
  fi
  if ! command -v tar >/dev/null 2>&1; then
    trice_state_log "FAIL: tar is required for exact Trice test-state restoration"
    return 1
  fi

  state_parent="${TRICE_TMP_DIR:-$TRICE_STATE_ROOT/temp}"
  mkdir -p "$state_parent" || return 1
  state_parent="$(cd "$state_parent" && pwd -P)" || return 1
  TRICE_TEST_STATE_PARENT="$state_parent"
  TRICE_TEST_STATE_DIR="$(mktemp -d "$state_parent/trice-state.XXXXXX")" || return 1
  TRICE_TEST_STATE_ARCHIVE="$TRICE_TEST_STATE_DIR/state.tar"
  TRICE_TEST_STATE_MANIFEST="$TRICE_TEST_STATE_DIR/manifest"
  TRICE_TEST_STATE_ABSENT="$TRICE_TEST_STATE_DIR/absent"
  raw_manifest="$TRICE_TEST_STATE_DIR/manifest.raw"
  : >"$raw_manifest"
  : >"$TRICE_TEST_STATE_ABSENT"

  trice_state_append_source_files "$raw_manifest" || return 1
  trice_state_append_optional_file "$TRICE_TIL_JSON" "TIL path" "$raw_manifest" "$TRICE_TEST_STATE_ABSENT" || return 1
  trice_state_append_optional_file "$TRICE_LI_JSON" "LI path" "$raw_manifest" "$TRICE_TEST_STATE_ABSENT" || return 1

  relative_bind_dir="$(trice_state_relative_path "$TRICE_BIND_DIR" "bind directory")" || return 1
  if [ "$relative_bind_dir" = "." ]; then
    trice_state_log "FAIL: bind directory must not be the repository root"
    return 1
  fi
  TRICE_TEST_STATE_BIND_DIR="$relative_bind_dir"
  if [ -L "$TRICE_STATE_ROOT/$relative_bind_dir" ]; then
    trice_state_log "FAIL: bind directory must not be a symbolic link: $relative_bind_dir"
    return 1
  fi
  if [ -e "$TRICE_STATE_ROOT/$relative_bind_dir" ]; then
    if [ ! -d "$TRICE_STATE_ROOT/$relative_bind_dir" ]; then
      trice_state_log "FAIL: bind directory path is not a directory: $relative_bind_dir"
      return 1
    fi
    printf '%s\n' "$relative_bind_dir" >>"$raw_manifest"
  else
    printf '%s\n' "$relative_bind_dir" >>"$TRICE_TEST_STATE_ABSENT"
  fi

  sort -u "$raw_manifest" >"$TRICE_TEST_STATE_MANIFEST"
  (
    cd "$TRICE_STATE_ROOT" || exit 1
    tar -cf "$TRICE_TEST_STATE_ARCHIVE" -T "$TRICE_TEST_STATE_MANIFEST"
  ) || {
    trice_state_log "FAIL: could not archive the initial Trice test state"
    return 1
  }

  TRICE_TEST_STATE_ACTIVE=1
  export TRICE_ID_WORKFLOW_OWNER=1
  trice_state_log "State snapshot: $TRICE_TEST_STATE_DIR"
  trice_state_log "State snapshot files: $(wc -l <"$TRICE_TEST_STATE_MANIFEST" | tr -d ' ')"
}

# trice_test_state_restore restores archived bytes and removes only paths that
# were proven absent initially. The validated bind directory is the sole
# recursive deletion target.
trice_test_state_restore() {
  local relative
  local bind_absolute

  if [ "${TRICE_TEST_STATE_ACTIVE:-0}" -ne 1 ]; then
    return 0
  fi
  bind_absolute="$TRICE_STATE_ROOT/$TRICE_TEST_STATE_BIND_DIR"
  case "$bind_absolute" in
    "$TRICE_STATE_ROOT"/*) ;;
    *)
      trice_state_log "FAIL: refusing to restore an unvalidated bind directory: $bind_absolute"
      return 1
      ;;
  esac
  if [ "$bind_absolute" = "$TRICE_STATE_ROOT" ]; then
    trice_state_log "FAIL: refusing to remove the repository root"
    return 1
  fi

  rm -rf -- "$bind_absolute" || {
    trice_state_log "FAIL: could not remove generated bind directory: $TRICE_TEST_STATE_BIND_DIR"
    return 1
  }
  while IFS= read -r relative; do
    [ -n "$relative" ] || continue
    if [ "$relative" = "$TRICE_TEST_STATE_BIND_DIR" ]; then
      continue
    fi
    rm -f -- "$TRICE_STATE_ROOT/$relative" || {
      trice_state_log "FAIL: could not remove initially absent generated file: $relative"
      return 1
    }
  done <"$TRICE_TEST_STATE_ABSENT"
  (
    cd "$TRICE_STATE_ROOT" || exit 1
    tar -xf "$TRICE_TEST_STATE_ARCHIVE"
  ) || {
    trice_state_log "FAIL: could not extract the initial Trice test state"
    return 1
  }

  TRICE_TEST_STATE_ACTIVE=0
  unset TRICE_ID_WORKFLOW_OWNER
  unset TRICE_ID_WORKFLOW
  trice_state_log "State restoration: exact initial files restored"
  case "$TRICE_TEST_STATE_DIR" in
    "$TRICE_TEST_STATE_PARENT"/trice-state.*)
      rm -rf -- "$TRICE_TEST_STATE_DIR"
      ;;
    *)
      trice_state_log "WARN: retained unexpected state directory for manual inspection: $TRICE_TEST_STATE_DIR"
      ;;
  esac
}

# trice_test_prepare_workflow reaches the requested test state while keeping
# Bind-only tests independent of the legacy Insert/Clean lifecycle.
trice_test_prepare_workflow() {
  local workflow="$1"

  trice_state_log "Prepared ID state requested: $workflow"
  if [ "$workflow" = "bind" ]; then
    # Canonical sources are checked in Bound. Regenerating their sidecars is
    # sufficient and preserves stable file keys throughout quick test runs.
    trice_state_run_command "$TRICE_STATE_ROOT/trice_bindIDs_in_examples_and_test_folder.sh"
    return $?
  fi

  # Bound source ownership is versioned while BindDir is generated and may be
  # absent or stale after a checkout. Refresh it transactionally before asking
  # re-migration to validate descriptor and location line numbers. Only full
  # test runs select the Inserted or Off workflows that need this transition.
  trice_state_run_command "$TRICE_STATE_ROOT/trice_bindIDs_in_examples_and_test_folder.sh" || return 1
  trice_state_run_command "$TRICE_STATE_ROOT/scripts/_250_legacy_remigrate_bind_to_clean.sh" || return 1
  trice_state_run_command "$TRICE_STATE_ROOT/scripts/_240_legacy_clean_ids.sh" || return 1
  case "$workflow" in
    insert)
      trice_state_run_command "$TRICE_STATE_ROOT/scripts/_230_legacy_insert_ids.sh"
      ;;
    off)
      # TRICE_OFF builds use the clean source state and need no ID generator.
      return 0
      ;;
    *)
      trice_state_log "FAIL: unsupported managed ID workflow: $workflow"
      return 2
      ;;
  esac
}

# trice_test_configure_include_path gives PC builds an isolated include path and
# Example builds only the additional bind directory required in Bound state.
trice_test_configure_include_path() {
  local workflow="$1"
  local include_mode="$2"
  local bind_absolute="$TRICE_STATE_ROOT/$TRICE_TEST_STATE_BIND_DIR"
  local bind_native="$bind_absolute"
  local separator=':'

  if [ "${C_INCLUDE_PATH+x}" = "x" ]; then
    TRICE_TEST_SAVED_C_INCLUDE_PATH_SET=1
    TRICE_TEST_SAVED_C_INCLUDE_PATH="$C_INCLUDE_PATH"
  else
    TRICE_TEST_SAVED_C_INCLUDE_PATH_SET=0
    TRICE_TEST_SAVED_C_INCLUDE_PATH=""
  fi

  if [ "${TRICE_BIND_INCLUDE_DIR+x}" = "x" ]; then
    TRICE_TEST_SAVED_BIND_INCLUDE_DIR_SET=1
    TRICE_TEST_SAVED_BIND_INCLUDE_DIR="$TRICE_BIND_INCLUDE_DIR"
  else
    TRICE_TEST_SAVED_BIND_INCLUDE_DIR_SET=0
    TRICE_TEST_SAVED_BIND_INCLUDE_DIR=""
  fi
  TRICE_TEST_INCLUDE_PATH_SAVED=1

  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      separator=';'
      if command -v cygpath >/dev/null 2>&1; then
        bind_native="$(cygpath -m "$bind_absolute")"
      fi
      ;;
  esac

  if [ "$workflow" = "bind" ]; then
    # CMake/MSVC does not consume C_INCLUDE_PATH, so expose the same absolute
    # directory in a build-system-neutral environment variable as well.
    export TRICE_BIND_INCLUDE_DIR="$bind_native"
  else
    unset TRICE_BIND_INCLUDE_DIR || true
  fi

  case "$include_mode:$workflow" in
    pc:insert)
      export C_INCLUDE_PATH=""
      ;;
    pc:bind)
      export C_INCLUDE_PATH="$bind_absolute"
      ;;
    example:insert | example:off) ;;
    example:bind)
      if [ -n "${C_INCLUDE_PATH:-}" ]; then
        export C_INCLUDE_PATH="$bind_absolute$separator$C_INCLUDE_PATH"
      else
        export C_INCLUDE_PATH="$bind_absolute"
      fi
      ;;
    none:*) ;;
    *)
      trice_state_log "FAIL: unsupported include-path mode: $include_mode:$workflow"
      return 2
      ;;
  esac
  trice_state_log "C_INCLUDE_PATH for $workflow/$include_mode: ${C_INCLUDE_PATH-<unset>}"
  trice_state_log "TRICE_BIND_INCLUDE_DIR for $workflow/$include_mode: ${TRICE_BIND_INCLUDE_DIR-<unset>}"
}

# trice_test_restore_include_path restores the caller's exact environment even
# when a build or signal interrupted the managed workflow.
trice_test_restore_include_path() {
  if [ "${TRICE_TEST_INCLUDE_PATH_SAVED:-0}" -ne 1 ]; then
    return 0
  fi
  if [ "${TRICE_TEST_SAVED_C_INCLUDE_PATH_SET:-0}" -eq 1 ]; then
    export C_INCLUDE_PATH="$TRICE_TEST_SAVED_C_INCLUDE_PATH"
  else
    unset C_INCLUDE_PATH || true
  fi
  if [ "${TRICE_TEST_SAVED_BIND_INCLUDE_DIR_SET:-0}" -eq 1 ]; then
    export TRICE_BIND_INCLUDE_DIR="$TRICE_TEST_SAVED_BIND_INCLUDE_DIR"
  else
    unset TRICE_BIND_INCLUDE_DIR || true
  fi
  TRICE_TEST_INCLUDE_PATH_SAVED=0
}

# trice_test_discard_incomplete_snapshot removes only a validated temporary
# snapshot directory when preparation failed before a restorable archive existed.
trice_test_discard_incomplete_snapshot() {
  if [ "${TRICE_TEST_STATE_ACTIVE:-0}" -eq 1 ] || [ -z "${TRICE_TEST_STATE_DIR:-}" ]; then
    return 0
  fi
  case "$TRICE_TEST_STATE_DIR" in
    "$TRICE_TEST_STATE_PARENT"/trice-state.*)
      rm -rf -- "$TRICE_TEST_STATE_DIR"
      ;;
  esac
}

# trice_test_finish is the sole trap target. A failed restoration turns an
# otherwise successful test into a failure while preserving an earlier cause.
trice_test_finish() {
  local status="${1:-$?}"
  local restore_status=0

  trap - INT TERM EXIT
  cd "$TRICE_STATE_ROOT" >/dev/null 2>&1 || true
  trice_test_restore_include_path
  trice_test_state_restore || restore_status=$?
  trice_test_discard_incomplete_snapshot || true
  if [ "$restore_status" -ne 0 ]; then
    trice_state_log "FAIL: state restoration failed with exit code $restore_status"
    if [ "$status" -eq 0 ]; then
      status="$restore_status"
    fi
  fi
  trice_state_log "ID workflow ${TRICE_TEST_ACTIVE_WORKFLOW:-unknown} end status: $status"
  exit "$status"
}

# trice_test_run_managed_workflow owns traps, state preparation, include-path
# setup, one shared worker invocation, and exact restoration.
trice_test_run_managed_workflow() {
  local workflow="$1"
  local include_mode="$2"
  local worker="$3"
  shift 3

  TRICE_TEST_ACTIVE_WORKFLOW="$workflow"
  export TRICE_ID_WORKFLOW="$workflow"
  trap 'trice_test_finish $?' EXIT
  trap 'trice_test_finish 130' INT
  trap 'trice_test_finish 143' TERM

  trice_test_state_snapshot || return 1
  trice_test_prepare_workflow "$workflow" || return 1
  trice_test_configure_include_path "$workflow" "$include_mode" || return 1
  trice_state_log "Build/test worker: $worker $*"
  trice_state_run_command "$worker" "$@" || return $?
  trice_test_finish 0
}

TRICE_TEST_STATE_ACTIVE=0
TRICE_TEST_INCLUDE_PATH_SAVED=0
