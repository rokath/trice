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

// selectedTargetMode applies the runner-selected execution strategy to pure
// deferred configurations. Direct, combined, and special configurations keep
// their configured behavior because they do not have an equivalent bulk path.
// An empty environment value preserves direct `go test` compatibility.
func selectedTargetMode(t *testing.T) string {
	t.Helper()
	switch os.Getenv(pcTestModeEnvironment) {
	case "":
		return targetMode
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
	switch selectedTargetMode(t) {
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

// Default: No-Op
var specificTest specificTestFunc = func(t *testing.T, triceLog logF) {
	// do nothing
}
