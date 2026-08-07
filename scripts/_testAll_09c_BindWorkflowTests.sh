#!/usr/bin/env bash
#
# Step 09c: Exercises public Bind workflows and transactional state restoration.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_testAll_00_common.sh
source "$SCRIPT_DIR/_testAll_00_common.sh"

fixture_abs=""
fixture_rel=""
project_abs=""
baseline_abs=""
state_worker_mode=""

# cleanup_fixture removes only the uniquely named test directory created below.
cleanup_fixture() {
  if [ -z "$fixture_abs" ]; then
    return 0
  fi
  case "$fixture_abs" in
    "$ROOT"/temp/bind-workflow.*) rm -rf -- "$fixture_abs" ;;
    *) log "WARN: retained unexpected workflow fixture: $fixture_abs" ;;
  esac
}

# fail_workflow records one actionable assertion failure.
fail_workflow() {
  log "FAIL: $*"
  return 1
}

# create_fixture builds one isolated project controlled through the public
# repository workflow variables. Runtime files stay below ignored ./temp.
create_fixture() {
  fixture_abs="$(mktemp -d "$ROOT/temp/bind-workflow.XXXXXX")" || return 1
  fixture_rel="${fixture_abs#"$ROOT/"}"
  project_abs="$fixture_abs/project"
  baseline_abs="$fixture_abs/baseline"
  mkdir -p "$project_abs" "$baseline_abs" || return 1

  printf '%s\n' \
    '// SPDX-License-Identifier: MIT' \
    '#include "trice.h"' \
    'static inline void fixture_log(int value) {' \
    '    trice("msg:inline=%d\n", value);' \
    '}' \
    'int main(void) {' \
    '    fixture_log(1);' \
    '    trice("msg:main\n");' \
    '    return 0;' \
    '}' >"$project_abs/main.c"

  printf '%s\n' \
    '// SPDX-License-Identifier: MIT' \
    '#ifndef BIND_WORKFLOW_TRICE_CONFIG_H_' \
    '#define BIND_WORKFLOW_TRICE_CONFIG_H_' \
    '#define TRICE_CLEAN 1' \
    '#define TRICE_BUFFER TRICE_STACK_BUFFER' \
    '#define TRICE_DIRECT_OUTPUT 1' \
    '#define TRICE_DEFERRED_OUTPUT 0' \
    '#define TRICE_DIRECT_AUXILIARY8 1' \
    '#define TRICE_CYCLE_COUNTER 0' \
    '#endif' >"$project_abs/triceConfig.h"

  printf '%s\n' \
    '// SPDX-License-Identifier: MIT' \
    '#ifndef BIND_WORKFLOW_EXCLUDED_H_' \
    '#define BIND_WORKFLOW_EXCLUDED_H_' \
    '#endif' >"$project_abs/excluded.h"
  printf '{}\n' >"$project_abs/til.json"
  printf '{}\n' >"$project_abs/li.json"

  cp "$project_abs/main.c" "$baseline_abs/main.c"
  cp "$project_abs/triceConfig.h" "$baseline_abs/triceConfig.h"
  cp "$project_abs/excluded.h" "$baseline_abs/excluded.h"
  cp "$project_abs/til.json" "$baseline_abs/til.json"
  cp "$project_abs/li.json" "$baseline_abs/li.json"

  unset TRICE_DEFAULTS
  export TRICE_TIL_JSON="$fixture_rel/project/til.json"
  export TRICE_LI_JSON="$fixture_rel/project/li.json"
  export TRICE_LI_ROOT="."
  export TRICE_BIND_DIR="$fixture_rel/project/bind"
  export TRICE_ID_OPTIONS="-IDMin 1000 -IDMax 1010 -IDMethod upward"
  export TRICE_EXCLUDES="-exclude $fixture_rel/project/excluded.h "
  export TRICE_ALIASES="$TRICE_EXCLUDES"
  export TRICE_PRJ_FILES="-src $fixture_rel/project "
  export TRICE_STATE_TEST_SOURCE="$fixture_rel/project/main.c"
}

# reset_fixture restores the exact initial fixture without relying on a Trice
# transition, making every state-restoration case independent.
reset_fixture() {
  cp "$baseline_abs/main.c" "$project_abs/main.c"
  cp "$baseline_abs/triceConfig.h" "$project_abs/triceConfig.h"
  cp "$baseline_abs/excluded.h" "$project_abs/excluded.h"
  cp "$baseline_abs/til.json" "$project_abs/til.json"
  cp "$baseline_abs/li.json" "$project_abs/li.json"
  rm -rf -- "$project_abs/bind"
}

# verify_fixture_restored proves byte-exact source/list restoration and absence
# of artifacts that did not exist when the child wrapper started.
verify_fixture_restored() {
  cmp "$baseline_abs/main.c" "$project_abs/main.c" >/dev/null || fail_workflow "main.c was not restored"
  cmp "$baseline_abs/triceConfig.h" "$project_abs/triceConfig.h" >/dev/null || fail_workflow "triceConfig.h was not restored"
  cmp "$baseline_abs/excluded.h" "$project_abs/excluded.h" >/dev/null || fail_workflow "excluded.h was not restored"
  cmp "$baseline_abs/til.json" "$project_abs/til.json" >/dev/null || fail_workflow "til.json was not restored"
  cmp "$baseline_abs/li.json" "$project_abs/li.json" >/dev/null || fail_workflow "li.json was not restored"
  [ ! -e "$project_abs/bind" ] || fail_workflow "initially absent bind directory was not removed"
}

# test_cli_help executes the required public long and short help forms against
# the freshly built repository binary.
test_cli_help() {
  run_cmd trice bind -h || return 1
  run_cmd trice bind --help || return 1
  run_cmd trice b -h || return 1
  run_cmd trice b --help || return 1
  run_cmd trice i -h || return 1
  if grep_log 'flag: help requested' "$LOGFILE"; then
    fail_workflow "CLI help emitted 'flag: help requested'"
    return 1
  fi
  grep -q -- '-bindDir' "$LOGFILE" || fail_workflow "Bind help omits -bindDir"
  grep -q -- '-defaultStampSize' "$LOGFILE" || fail_workflow "Bind help omits shared Insert options"
}

# verify_bound_fixture checks the externally visible Bound-state invariants.
verify_bound_fixture() {
  grep -q 'trice-bind: keep as last include' "$project_abs/main.c" || fail_workflow "Bind did not add its owner include"
  if grep -Eq '(iD|Id|ID)[[:space:]]*\([[:space:]]*[1-9][0-9]*' "$project_abs/main.c"; then
    fail_workflow "Bound source contains a positive inserted ID"
    return 1
  fi
  [ -d "$project_abs/bind" ] || fail_workflow "Bind directory was not created"
  find "$project_abs/bind" -type f -name '*.h' -print | grep -q . || fail_workflow "No generated sidecar was created"
  grep -q '"Type"' "$project_abs/til.json" || fail_workflow "til.json has no generated entries"
  grep -q 'main.c' "$project_abs/li.json" || fail_workflow "li.json has no fixture location"
}

# test_public_workflows covers migration, Bind idempotence, safe re-migration,
# re-migration idempotence, and the complete return to Inserted.
test_public_workflows() {
  local inserted="$fixture_abs/inserted"
  local bound="$fixture_abs/bound-snapshot"
  local remigrated="$fixture_abs/remigrated"

  reset_fixture
  mkdir -p "$inserted" "$bound" "$remigrated"

  run_cmd "$ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || return 1
  grep -Eq '(iD|Id|ID)[[:space:]]*\([[:space:]]*[1-9][0-9]*' "$project_abs/main.c" || return 1
  cp "$project_abs/main.c" "$inserted/main.c"
  cp "$project_abs/triceConfig.h" "$inserted/triceConfig.h"
  cp "$project_abs/til.json" "$inserted/til.json"
  cp "$project_abs/li.json" "$inserted/li.json"

  run_cmd "$ROOT/trice_cleanIDs_in_examples_and_test_folder.sh" || return 1
  run_cmd "$ROOT/trice_bindIDs_in_examples_and_test_folder.sh" || return 1
  verify_bound_fixture || return 1
  cmp "$inserted/til.json" "$project_abs/til.json" >/dev/null || fail_workflow "Inserted and Bound TIL content differs"

  cp "$project_abs/main.c" "$bound/main.c"
  cp "$project_abs/triceConfig.h" "$bound/triceConfig.h"
  cp "$project_abs/til.json" "$bound/til.json"
  cp "$project_abs/li.json" "$bound/li.json"
  cp -R "$project_abs/bind" "$bound/bind"
  run_cmd "$ROOT/trice_bindIDs_in_examples_and_test_folder.sh" || return 1
  cmp "$bound/main.c" "$project_abs/main.c" >/dev/null || fail_workflow "Second Bind changed main.c"
  cmp "$bound/triceConfig.h" "$project_abs/triceConfig.h" >/dev/null || fail_workflow "Second Bind changed triceConfig.h"
  cmp "$bound/til.json" "$project_abs/til.json" >/dev/null || fail_workflow "Second Bind changed til.json"
  cmp "$bound/li.json" "$project_abs/li.json" >/dev/null || fail_workflow "Second Bind changed li.json"
  diff -r "$bound/bind" "$project_abs/bind" >/dev/null || fail_workflow "Second Bind changed sidecar content"

  run_cmd "$ROOT/trice_remigrateBindToClean_in_examples_and_test_folder.sh" || return 1
  if grep -q 'trice-bind: keep as last include' "$project_abs/main.c"; then
    fail_workflow "Re-migration left an active owner include"
    return 1
  fi
  if find "$project_abs/bind" -type f -name '*.h' -print 2>/dev/null | grep -q .; then
    fail_workflow "Re-migration left generated sidecars"
    return 1
  fi
  cp "$project_abs/main.c" "$remigrated/main.c"
  cp "$project_abs/triceConfig.h" "$remigrated/triceConfig.h"
  run_cmd "$ROOT/trice_remigrateBindToClean_in_examples_and_test_folder.sh" || return 1
  cmp "$remigrated/main.c" "$project_abs/main.c" >/dev/null || fail_workflow "Second re-migration changed main.c"
  cmp "$remigrated/triceConfig.h" "$project_abs/triceConfig.h" >/dev/null || fail_workflow "Second re-migration changed triceConfig.h"

  run_cmd "$ROOT/trice_insertIDs_in_examples_and_test_folder.sh" || return 1
  cmp "$inserted/main.c" "$project_abs/main.c" >/dev/null || fail_workflow "Re-migrated Insert source differs"
  cmp "$inserted/triceConfig.h" "$project_abs/triceConfig.h" >/dev/null || fail_workflow "Re-migrated Insert config differs"
  cmp "$inserted/til.json" "$project_abs/til.json" >/dev/null || fail_workflow "Re-migrated Insert til.json differs"
  cmp "$inserted/li.json" "$project_abs/li.json" >/dev/null || fail_workflow "Re-migrated Insert li.json differs"
}

# state_worker mutates a managed file after preparation and then selects one of
# the success, failure, SIGINT, or SIGTERM exits under test.
state_worker() {
  printf '%s\n' '/* state worker mutation */' >>"$ROOT/$TRICE_STATE_TEST_SOURCE"
  case "$state_worker_mode" in
    success) return 0 ;;
    failure) return 23 ;;
    INT) kill -INT "$$" ;;
    TERM) kill -TERM "$$" ;;
    *) return 99 ;;
  esac
}

# state_child owns one real managed workflow. Its EXIT/signal trap is installed
# by _trice_test_state.sh and therefore exercises production wrapper behavior.
state_child() {
  state_worker_mode="$1"
  # shellcheck source=./_trice_test_state.sh
  source "$SCRIPT_DIR/_trice_test_state.sh"
  trice_test_run_managed_workflow bind none state_worker
}

# run_state_case checks the signal-compatible status and exact postcondition.
run_state_case() {
  local mode="$1"
  local expected="$2"
  local actual=0

  reset_fixture
  log "+ state restoration child: $mode"
  "$SCRIPT_DIR/_testAll_09c_BindWorkflowTests.sh" --quiet --state-child "$mode" || actual=$?
  if [ "$actual" -ne "$expected" ]; then
    fail_workflow "state child $mode returned $actual, expected $expected"
    return 1
  fi
  verify_fixture_restored
}

# test_state_restoration covers every exit class required by the specification.
test_state_restoration() {
  run_state_case success 0 || return 1
  run_state_case failure 23 || return 1
  run_state_case INT 130 || return 1
  run_state_case TERM 143 || return 1
}

# main executes public integration tests after the repository tool build step.
main() {
  init_logfile
  export LOGFILE
  trap 'cleanup_fixture' EXIT

  has_command go || {
    fail_workflow "Go is required for public re-migration tests"
    return 1
  }
  has_command trice || {
    fail_workflow "the repository Trice binary is not available"
    return 1
  }
  create_fixture || return 1
  test_cli_help || return 1
  test_public_workflows || return 1
  test_state_restoration || return 1

  cleanup_fixture
  fixture_abs=""
  trap - EXIT
}

if [ "${1:-}" = "--state-child" ]; then
  state_child "${2:-missing}"
  exit $?
fi

main "$@"
