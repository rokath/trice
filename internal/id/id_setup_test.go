package id

import (
	"os"
	"testing"

	"github.com/spf13/afero"
	"github.com/tj/assert"
)

// In Go, each package generates an individual test binary and they are tested parallel.
// All package tests are executed sequentially but use the same global variables.
// Therefore we have to reset the global variables in each test function.

// TestMain - see for example https://medium.com/goingogo/why-use-testmain-for-testing-in-go-dafb52b406bc
func TestMain(m *testing.M) {
	//log.Println("Do stuff BEFORE the id package tests!")
	ExtendMacrosWithParamCount = true
	exitVal := m.Run()
	//log.Println("Do stuff AFTER the id package tests!")

	os.Exit(exitVal)
}

// SetupTest - look for "This is the greatest answer in this thread" in  https://stackoverflow.com/questions/23729790/how-can-i-do-test-setup-using-the-testing-package-in-go
func SetupTest(t *testing.T, fSys *afero.Afero) func() {
	//fmt.Println("Setup code for", t.Name())
	ResetGlobalVars(t)

	// create empty til.json
	assert.Nil(t, fSys.WriteFile(FnJSON, []byte(``), 0777))

	// create empty li.json
	assert.Nil(t, fSys.WriteFile(LIFnJSON, []byte(``), 0777))

	// tear down later
	return func() {
		//fmt.Println("tear-down code for ", t.Name())
	}
}

func ResetGlobalVars(t *testing.T) {
	Verbose = false
	DryRun = false
	FnJSON = t.Name() + "_til.json"
	LIFnJSON = t.Name() + "_li.json"
	Min = TriceID(1000)
	Max = TriceID(7999)
	SearchMethod = "random"
	LiPathIsRelative = false
	Srcs = nil
	IDRange = nil
	ExtendMacrosWithParamCount = false
	DefaultTriceBitWidth = "32"
	DefaultStampSize = 32
	StampSizeId = " ID(0),"
	TriceCacheEnabled = false
	var err error
	UserHomeDir, err = os.UserHomeDir()
	assert.Nil(t, err)
}
