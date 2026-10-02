// SPDX-License-Identifier: MIT

package scripts_test

import (
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"testing"

	"github.com/stretchr/testify/assert"
)

// pcWorkerFixture replaces Go, not the worker: real shell scheduling, exit-code
// collection, failure excerpts and diagnostic reruns execute in an isolated repo.
func pcWorkerFixture(t *testing.T) string {
	t.Helper()
	root := scriptFixture(t, "scripts/_160_pc_target_test_worker.sh")
	writeFixture(t, root, "_test/a/generated_cgoPackage.go", "package cgot\n")
	writeFixture(t, root, "_test/a/generated_cgoPackage_test.go", "package cgot\n")
	writeFixture(t, root, "bin/go", `#!/usr/bin/env bash
set -eu
case "$1" in
  clean) exit 0 ;;
  list) printf 'fixture/a\nfixture/b\nfixture/c\nfixture/d\n'; exit 0 ;;
  test) ;;
  *) echo "unexpected Go command: $*"; exit 2 ;;
esac
for package; do :; done
name="${package##*/}"
mkdir -p ../events ../active
printf '%s\n' "$*" > "../events/$name.$TRICE_PC_TEST_MODE.args"
touch "../active/$name"
trap 'rm -f "../active/$name"' EXIT
if [ "${CANCEL_PROBE:-0}" = "$TRICE_PC_TEST_MODE" ]; then
  bash -c 'trap "sleep 0.2; touch ../events/child-stopped; exit 0" TERM; touch ../events/child-ready; while :; do sleep 0.05; done' &
  child=$!
  wait "$child"
  exit 0
fi
if [ "${EXPECT_OVERLAP:-0}" = 1 ] && [ "$TRICE_PC_TEST_MODE" = auto ]; then
  if [ "$name" = c ] || [ "$name" = d ]; then
    if [ -f ../active/a ] || [ -f ../active/b ]; then echo 'error: job limit exceeded'; exit 1; fi
  fi
  if [ "$name" = a ]; then
    attempts=0
    until [ -f ../events/overlap ]; do
      sleep 0.01
      attempts=$((attempts+1))
      if [ "$attempts" -gt 500 ]; then echo 'error: jobs did not overlap'; exit 1; fi
    done
  elif [ "$name" = b ]; then
    attempts=0
    until [ -f ../active/a ]; do
      sleep 0.01
      attempts=$((attempts+1))
      if [ "$attempts" -gt 500 ]; then echo 'error: first job did not overlap'; exit 1; fi
    done
    touch ../events/overlap
  fi
fi
if [ "$name" = b ] && [ "${FAIL_B:-0}" = 1 ] && [ "$TRICE_PC_TEST_MODE" = auto ]; then
  echo 'Target mode=directMode, execution=directModeBulk'
  echo 'EXPECTATION FAILURE triceCheck.c:123 channel=direct'
  echo 'want: "hello\n"'
  echo 'got: "wrong\n"'
  exit 1
fi
echo "PASS $name $TRICE_PC_TEST_MODE"
`)
	return root
}

// TestPCWorkerBatchingAndFailures covers serial/parallel success, fail-fast,
// --no-stop, and a passing diagnostic that must not erase the bulk failure.
func TestPCWorkerBatchingAndFailures(t *testing.T) {
	for _, tc := range []struct {
		name, jobs, fail, noStop string
		later                    bool
	}{
		{"serial_success", "1", "0", "0", true},
		{"parallel_success", "2", "0", "0", true},
		{"parallel_fail_fast_finishes_batch_only", "2", "1", "0", false},
		{"parallel_no_stop_keeps_bulk_failure", "2", "1", "1", true},
	} {
		t.Run(tc.name, func(t *testing.T) {
			root := pcWorkerFixture(t)
			overlap := "0"
			if tc.jobs == "2" {
				overlap = "1"
			}
			out, err := runFixture(t, root, "mkdir -p temp/log; bash scripts/_160_pc_target_test_worker.sh full", map[string]string{
				"TRICE_PC_TEST_JOBS": tc.jobs, "TRICE_PC_TEST_MODE": "auto", "TRICE_TEST_NO_STOP": tc.noStop,
				"TRICE_ID_WORKFLOW": "bind", "FAIL_B": tc.fail, "EXPECT_OVERLAP": overlap,
			})
			if tc.fail == "1" {
				assert.Error(t, err, out)
				assert.Contains(t, out, "PC FAIL [bind] fixture/b")
				assert.Contains(t, out, "triceCheck.c:123 channel=direct")
				assert.Contains(t, out, `want: "hello\n"`)
				assert.Contains(t, out, `got: "wrong\n"`)
				assert.Contains(t, out, "passes line by line; inspect bulk")
				assert.Contains(t, out, "Reproduce after the same ID preparation:")
			} else {
				assert.NoError(t, err, out)
			}
			_, statErr := os.Stat(filepath.Join(root, "events", "c.auto.args"))
			assert.Equal(t, tc.later, statErr == nil, out)
			if tc.jobs == "2" {
				_, err = os.Stat(filepath.Join(root, "events", "overlap"))
				assert.NoError(t, err, out)
			}
			active, err := os.ReadDir(filepath.Join(root, "active"))
			assert.NoError(t, err)
			assert.Empty(t, active, "all children finish before state restoration")
			logs, err := filepath.Glob(filepath.Join(root, "temp/log/pc-bind.*/*/output.log"))
			assert.NoError(t, err)
			wantLogs := 2
			if tc.later {
				wantLogs = 4
			}
			assert.Len(t, logs, wantLogs)
			args, err := os.ReadFile(filepath.Join(root, "events/a.auto.args"))
			assert.NoError(t, err)
			assert.Contains(t, string(args), "-p 1 -count=1")
			assert.NotContains(t, string(args), "-run", "other tests in the same package remain included")
		})
	}
}

// TestPCWorkerRejectsInvalidControls checks validation before any Go command.
func TestPCWorkerRejectsInvalidControls(t *testing.T) {
	for _, jobs := range []string{"0", "-1", "many", "01"} {
		t.Run(jobs, func(t *testing.T) {
			root := pcWorkerFixture(t)
			out, err := runFixture(t, root, "bash scripts/_160_pc_target_test_worker.sh full", map[string]string{"TRICE_PC_TEST_JOBS": jobs})
			assert.Error(t, err)
			assert.Contains(t, out, "positive integer required")
			assert.False(t, strings.Contains(out, "go clean"))
		})
	}
}

// TestPCWorkerCancellationReachesDescendants waits for a real child handshake
// before signaling the worker. Its grandchild must stop before cleanup returns.
func TestPCWorkerCancellationReachesDescendants(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("POSIX process-group signal assertion; normal worker execution is tested on Windows")
	}
	for _, mode := range []string{"auto", "line-by-line"} {
		t.Run(mode, func(t *testing.T) {
			root := pcWorkerFixture(t)
			out, err := runFixture(t, root, `mkdir -p temp/log
bash scripts/_160_pc_target_test_worker.sh full > worker.out 2>&1 &
worker=$!
attempts=0
until [ -f events/child-ready ]; do
  sleep 0.01
  attempts=$((attempts+1))
  if [ "$attempts" -gt 500 ]; then kill -TERM "$worker"; cat worker.out; exit 1; fi
done
kill -TERM "$worker"
status=0
wait "$worker" || status=$?
cat worker.out
printf 'WorkerExit:%s\n' "$status"
[ "$status" -eq 143 ] && [ -f events/child-stopped ]`, map[string]string{
				"TRICE_PC_TEST_JOBS": "1", "TRICE_PC_TEST_MODE": "auto", "CANCEL_PROBE": mode, "FAIL_B": "1",
			})
			assert.NoError(t, err, out)
			assert.Contains(t, out, "WorkerExit:143")
			_, statErr := os.Stat(filepath.Join(root, "events", "c.auto.args"))
			assert.True(t, os.IsNotExist(statErr), "no further configuration is launched after cancellation")
		})
	}
}
