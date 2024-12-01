package id

import (
	"bytes"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"testing"

	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

const (
	Home  = "/home/tst/"           // home folder for id package tests
	Cache = Home + ".trice/cache/" // cache folder
	Proj  = "/my/project/"         // project folder
)

var (
	B      bytes.Buffer
	W      = io.Writer(&B) // args.Handler io.Writer output
	FSys   *afero.Afero    // ram file system for the tests
	SFName string          // source file name in test (changes with each test)
	CCache string          // cleaned cache file name in test (changes with each test)
	ICache string          // inserted file name in test (changes with each test)
	g      globalDefaults  // gD holds global vars default values
)

func init() {
	// All id tests should be executed only on a memory mapped file system.
	FSys = &afero.Afero{Fs: afero.NewMemMapFs()}
}

// Setup should be called on the begin of each id test function, if global variables are used/changed.
func Setup(t *testing.T) func() {
	// Setup code here ///////////////////
	g.SetGlobalVars(t)
	IDData.TagList = IDData.TagList[:0]

	// set folders
	UserHomeDir = Home                          // overwrite global variable for the id package tests
	msg.FatalOnErr(FSys.MkdirAll(Proj, 0o700))  // create project folder
	msg.FatalOnErr(FSys.MkdirAll(Cache, 0o700)) // create cache (it could be deleted in some tests)

	// create til.json and li.json
	FnJSON = Proj + t.Name() + "_til.json"
	assert.Nil(t, FSys.WriteFile(FnJSON, nil, 0777))
	LIFnJSON = Proj + t.Name() + "_li.json"
	assert.Nil(t, FSys.WriteFile(LIFnJSON, nil, 0777))

	SFName = Proj + t.Name() + "_file.c"
	CCache = filepath.Join(Cache, "/cleaned", SFName)
	ICache = filepath.Join(Cache, "/inserted", SFName)

	fmt.Println(t.Name(), "...")
	// tear down later //////////////////
	return func() {
		// tear-down code here
		fmt.Println(t.Name(), "...done.")
		B.Reset() // Clear output generated during the test.

		// Remove without error check, because file or folder could not exist after a test.
		FSys.Remove(FnJSON)
		FSys.Remove(LIFnJSON)
		FSys.Remove(SFName)
		FSys.Remove(CCache)
		FSys.Remove(ICache)
	}
}

// TestMain - see for example https://medium.com/goingogo/why-use-testmain-for-testing-in-go-dafb52b406bc
func TestMain(m *testing.M) {

	// log.Println("Do stuff BEFORE the id package tests!")
	g.GetGlobalVars()
	// Run the id tests sequentially in alphabetical order.
	exitVal := m.Run()

	//log.Println("Do stuff AFTER the id package tests!")
	os.Exit(exitVal)
}

// SrcFile is used by some id tests.
type SrcFile struct {
	Fn          string
	Clean       string
	InsertedIDs string
}

type globalDefaults struct {
	defaultVerbose                    bool
	defaultDryRun                     bool
	defaultMin                        TriceID
	defaultMax                        TriceID
	defaultSearchMethod               string
	defaultLiPathIsRelative           bool
	defaultSrcs                       ArrayFlag
	defaultExtendMacrosWithParamCount bool
	defaultDefaultTriceBitWidth       string
	defaultDefaultStampSize           int
	defaultStampSizeId                string
	defaultTriceCacheEnabled          bool
	defaultSpaceInsideTriceBrace      bool
}

func (p *globalDefaults) GetGlobalVars() {
	// Keep default values of global variables.
	p.defaultVerbose = Verbose
	p.defaultDryRun = DryRun
	p.defaultMin = Min
	p.defaultMax = Max
	p.defaultSearchMethod = SearchMethod
	p.defaultLiPathIsRelative = LiPathIsRelative
	p.defaultSrcs = Srcs
	p.defaultExtendMacrosWithParamCount = ExtendMacrosWithParamCount
	p.defaultDefaultTriceBitWidth = DefaultTriceBitWidth
	p.defaultDefaultStampSize = DefaultStampSize
	p.defaultStampSizeId = StampSizeId
	p.defaultTriceCacheEnabled = TriceCacheEnabled
	p.defaultSpaceInsideTriceBrace = false
}

// SetGlobalVars sets all global variables in a definitive state.
// In Go, each package generates an individual test binary and they are tested parallel.
// All package tests are executed sequentially but use the same global variables.
// Therefore we have to reset the global variables in each test function.
func (p *globalDefaults) SetGlobalVars(t *testing.T) {
	Verbose = p.defaultVerbose
	DryRun = p.defaultDryRun
	Min = p.defaultMin
	Max = p.defaultMax
	SearchMethod = p.defaultSearchMethod
	LiPathIsRelative = p.defaultLiPathIsRelative
	Srcs = p.defaultSrcs
	ExtendMacrosWithParamCount = p.defaultExtendMacrosWithParamCount
	DefaultTriceBitWidth = p.defaultDefaultTriceBitWidth
	DefaultStampSize = p.defaultDefaultStampSize
	StampSizeId = p.defaultStampSizeId
	TriceCacheEnabled = p.defaultTriceCacheEnabled
	SpaceInsideTriceBrace = p.defaultSpaceInsideTriceBrace
}
