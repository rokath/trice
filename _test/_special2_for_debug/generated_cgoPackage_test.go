package cgot

import (
	"fmt"
	"os"
	"testing"

	"github.com/tj/assert"
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

func TestTriceLog(t *testing.T) {
	defer setup(t)() // This executes setup(t) and puts the returned function into the defer list.
	switch targetMode {
	case "deferredMode":
		assert.NotNil(t, triceLog)
		// triceLogBulk executes each triceCheck.c test line, gets its binary output and
		// collects all these outputs into one (big) buffer. Then the Trice log functionality
		// is started once for this (big) buffer and the whole output is generated. Afterwards
		// this generated output is compared line by line with the expected results. The
		// function triceLogBulk is much faster than triceLogLineByLine but difficult to debug.
		triceLogBulk(t, triceLog, testLines, targetActivityC)
	case "directMode":
		assert.NotNil(t, triceLog)
		// triceLogLineByLine executes each triceCheck.c test line, gets its binary output and
		// restarts the whole Trice log functionality for this, resulting in a long test duration.
		// This test is avoidable for only-deferred modes which allow doTestTriceLogBulk=true,
		// but useful for debugging.
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
	case "combinedMode":
		assert.NotNil(t, triceLogDirect)
		assert.NotNil(t, triceLogDeferred)
		triceLogDirectAndDeferred(t, triceLogDirect, triceLogDeferred, testLines, targetActivityC)
	default:
		//assert.Fail(t, "unexpected targetMode", targetMode)
	}
}
