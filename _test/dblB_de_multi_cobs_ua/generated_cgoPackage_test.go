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
	case "deferredModeLinebyLine":
		assert.NotNil(t, triceLog)
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
	case "deferredMode":
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