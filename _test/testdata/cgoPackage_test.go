// SPDX-License-Identifier: MIT

package cgot

import (
	"fmt"
	"os"
	"testing"

	"github.com/stretchr/testify/assert"
)

const (
	// pcTestModeEnvironment lets the PC-test runner select an execution strategy
	// without changing or duplicating any target-configuration directory.
	pcTestModeEnvironment = "TRICE_PC_TEST_MODE"
	pcTestModeBulk        = "bulk"
	pcTestModeAuto        = "auto"
	pcTestModeLineByLine  = "line-by-line"
)

// TestMain - see for example https://medium.com/goingogo/why-use-testmain-for-testing-in-go-dafb52b406bc
func TestMain(m *testing.M) {
	g.getGlobalVarsDefaults() // Do stuff BEFORE the package tests!
	exitVal := m.Run()        // Run the package tests sequentially in alphabetical order.
	os.Exit(exitVal)          // Do stuff AFTER the package tests!
}

// setup should be called on the begin of each test function, if global variables are used/changed.
func setup(t *testing.T) func() {
	// Setup code here ///////////////////
	g.setGlobalVarsDefaults()
	fmt.Println(t.Name(), "...")

	// tear down later //////////////////
	return func() {
		// tear-down code here
		fmt.Println(t.Name(), "...done.")
	}
}

// selectedTargetMode applies the runner-selected strategy. Auto batches framed
// channels while preserving unframed and special configurations. Explicit bulk
// remains available for the original deferred-buffer regression probes.
// An empty environment value preserves direct `go test` compatibility.
func selectedTargetMode(t *testing.T) string {
	t.Helper()
	switch os.Getenv(pcTestModeEnvironment) {
	case "":
		return targetMode
	case pcTestModeAuto:
		return bulkTargetMode(targetMode)
	case pcTestModeBulk:
		switch targetMode {
		case "deferredModeLineByLineAndBulk", "deferredModeBulk":
			return "deferredModeBulk"
		default:
			t.Skipf("%s is not bulk-capable for target mode %q", pcTestModeEnvironment, targetMode)
			return ""
		}
	case pcTestModeLineByLine:
		switch targetMode {
		case "deferredModeLineByLineAndBulk", "deferredModeLineByLine", "deferredModeBulk":
			return "deferredModeLineByLine"
		default:
			return targetMode
		}
	default:
		t.Fatalf("unsupported %s value %q", pcTestModeEnvironment, os.Getenv(pcTestModeEnvironment))
		return ""
	}
}

func TestTriceLog(t *testing.T) {
	defer setup(t)() // This executes setup(t) and puts the returned function into the defer list.
	mode := selectedTargetMode(t)
	t.Logf("Target mode=%s, execution=%s", targetMode, mode)
	switch mode {
	case "directModeBulk":
		triceLogFramedChannels(t, true, false)
	case "combinedModeBulk":
		triceLogFramedChannels(t, true, true)
	case "deferredModeDrainedBulk":
		triceLogFramedChannels(t, false, true)
	case "deferredModeLineByLineAndBulk":
		assert.NotNil(t, triceLog)
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
		triceLogBulk(t, triceLog, testLines, targetActivityC)
	case "deferredModeLineByLine":
		assert.NotNil(t, triceLog)
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
	case "deferredModeBulk":
		assert.NotNil(t, triceLog)
		triceLogBulk(t, triceLog, testLines, targetActivityC)
	case "directMode":
		assert.NotNil(t, triceLog)
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
	case "combinedMode":
		assert.NotNil(t, triceLogDirect)
		assert.NotNil(t, triceLogDeferred)
		triceLogDirectAndDeferred(t, triceLogDirect, triceLogDeferred, testLines, targetActivityC)
	case "specificTest":
		specificTest(t, triceLog)
	default:
		//assert.Fail(t, "unexpected targetMode", targetMode)
	}
}

type specificTestFunc func(t *testing.T, triceLog logF)

// TestBulkOutputDiagnostics preserves exact source mapping for empty and
// multiline expectations and keeps missing/extra/control-byte output visible.
func TestBulkOutputDiagnostics(t *testing.T) {
	result := []results{{line: 10, exps: "first\n"}, {line: 20, exps: ""}, {line: 30, exps: "a\tb\nsecond\n"}}
	for _, tc := range []struct{ name, actual, problem string }{
		{"exact_multiline_and_empty", "first\na\tb\nsecond\n", ""},
		{"changed_value", "first\na\tc\nsecond\n", "triceCheck.c:30"},
		{"missing_record", "first\n", "triceCheck.c:30"},
		{"unexpected_prefix", "extra\nfirst\na\tb\nsecond\n", "triceCheck.c:10"},
		{"extra_record_after_last_expectation", "first\na\tb\nsecond\nextra\n", "extra output after final expectation"},
	} {
		t.Run(tc.name, func(t *testing.T) {
			err := bulkOutputError(result, tc.actual, "deferred")
			if tc.problem == "" {
				assert.NoError(t, err)
				return
			}
			if assert.Error(t, err) {
				assert.Contains(t, err.Error(), tc.problem)
				assert.Contains(t, err.Error(), "channel=deferred")
				if tc.name == "changed_value" {
					assert.Contains(t, err.Error(), `want: "a\tb\nsecond\n"`)
					assert.Contains(t, err.Error(), `got:  "a\tc\nsecond\n"`)
				}
			}
		})
	}
}

// Default: No-Op
var specificTest specificTestFunc = func(t *testing.T, triceLog logF) {
	// do nothing
}
