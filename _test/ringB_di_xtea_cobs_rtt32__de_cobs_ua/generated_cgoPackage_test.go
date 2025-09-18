package cgot

import (
	"fmt"
	"os"
	"testing"
)

var (
	doTestTriceLogLineByLine        bool
	doTestTriceLogBulk              bool
	doTestTriceLogDirectAndDeferred bool
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

func TestTriceLogLineByLine(t *testing.T) {
	defer setup(t)() // This executes setup(t) and puts the returned function into the defer list.
	if doTestTriceLogLineByLine && triceLog != nil {
		triceLogLineByLine(t, triceLog, testLines, targetActivityC)
	}
}

func TestTriceLogBulk(t *testing.T) {
	defer setup(t)() // This executes setup(t) and puts the returned function into the defer list.
	if doTestTriceLogBulk && triceLog != nil {
		triceLogBulk(t, triceLog, testLines, targetActivityC)
	}
}

func TestTriceLogDirectAndDeferred(t *testing.T) {
	defer setup(t)() // This executes setup(t) and puts the returned function into the defer list.
	if doTestTriceLogDirectAndDeferred && triceLogDirect != nil && triceLogDeferred != nil {
		triceLogDirectAndDeferred(t, triceLogDirect, triceLogDeferred, testLines, targetActivityC)
	}
}
