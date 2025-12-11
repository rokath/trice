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
	g      globalDefaults  // g holds global vars default values
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

	// Reset additional global state which is modified through CLI-style flags in other tests.
	// This ensures each test starts from the same baseline configuration.
	// (IDRange, aliases, etc. are also restored via globalDefaults, but an explicit reset
	// does not hurt and documents the intention clearly.)
	// IDRange is a multi-flag container for tag-specific ID ranges.
	// Make sure IDRange has its initial value for each test.
	// If IDRange's zero value is not appropriate, it will be restored from g.defaultIDRange below.
	// The same applies for the alias and exclude lists.
	// (These lines can be removed if you prefer to rely solely on globalDefaults.)
	// IDRange = nil
	// TriceAliases = nil
	// TriceSAliases = nil
	// ExcludeSrcs = nil

	// set folders
	UserHomeDir = Home                          // overwrite global variable for the id package tests
	msg.FatalOnErr(FSys.MkdirAll(Proj, 0o700))  // create project folder
	msg.FatalOnErr(FSys.MkdirAll(Cache, 0o700)) // create cache (it could be deleted in some tests)

	// create til.json and li.json
	FnJSON = Proj + t.Name() + "_til.json"
	assert.Nil(t, FSys.WriteFile(FnJSON, nil, 0o777))
	LIFnJSON = Proj + t.Name() + "_li.json"
	assert.Nil(t, FSys.WriteFile(LIFnJSON, nil, 0o777))

	SFName = Proj + t.Name() + "_file.c"
	CCache = filepath.Join(Cache, "/cleaned", SFName)
	ICache = filepath.Join(Cache, "/inserted", SFName)

	fmt.Println(t.Name(), "...")
	// tear down later //////////////////
	return func() {
		// todo: assert.Equal(t, B.String(), "")

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

	// log.Println("Do stuff BEFORE the package tests!")
	g.GetGlobalVars()
	// Run the package tests sequentially in alphabetical order.
	exitVal := m.Run()

	//log.Println("Do stuff AFTER the package tests!")
	os.Exit(exitVal)
}

// SrcFile is used by some id tests.
type SrcFile struct {
	Fn          string
	Clean       string
	InsertedIDs string
}

// globalDefaults captures the default values of all global variables in the id package
// which are modified by tests or by CLI-style flag parsing (via the args package).
// Each test calls Setup(t), which in turn calls SetGlobalVars(t) to restore
// these defaults and ensure test independence.
type globalDefaults struct {
	defaultVerbose                    bool
	defaultDryRun                     bool
	defaultMin                        TriceID
	defaultMax                        TriceID
	defaultSearchMethod               string
	defaultLIPathKind                 string
	defaultSrcs                       ArrayFlag
	defaultExcludeSrcs                ArrayFlag
	defaultTriceAliases               ArrayFlag
	defaultTriceSAliases              ArrayFlag
	defaultExtendMacrosWithParamCount bool
	defaultDefaultTriceBitWidth       string
	defaultDefaultStampSize           int
	defaultStampSizeId                string
	defaultTriceCacheEnabled          bool
	defaultSpaceInsideParenthesis     bool
	defaultSkipAdditionalChecks       bool
	defaultIDRange                    ArrayFlag
	defaultLogging                    bool
}

func (p *globalDefaults) GetGlobalVars() {
	// Keep default values of global variables as they are after init/TestMain.
	p.defaultVerbose = Verbose
	p.defaultDryRun = DryRun
	p.defaultMin = Min
	p.defaultMax = Max
	p.defaultSearchMethod = SearchMethod
	p.defaultLIPathKind = LIPathKind
	p.defaultSrcs = Srcs
	p.defaultExcludeSrcs = ExcludeSrcs
	p.defaultTriceAliases = TriceAliases
	p.defaultTriceSAliases = TriceSAliases
	p.defaultExtendMacrosWithParamCount = ExtendMacrosWithParamCount
	p.defaultDefaultTriceBitWidth = DefaultTriceBitWidth
	p.defaultDefaultStampSize = DefaultStampSize
	p.defaultStampSizeId = StampSizeId
	p.defaultTriceCacheEnabled = TriceCacheEnabled
	p.defaultSpaceInsideParenthesis = SpaceInsideParenthesis
	p.defaultSkipAdditionalChecks = SkipAdditionalChecks
	p.defaultIDRange = IDRange
	p.defaultLogging = Logging
}

// SetGlobalVars sets all global variables in a definitive state.
// In Go, each package generates an individual test binary and they are tested in parallel.
// All package tests are executed sequentially but use the same global variables.
// Therefore we have to reset the global variables in each test function.
func (p *globalDefaults) SetGlobalVars(t *testing.T) {
	Verbose = p.defaultVerbose
	DryRun = p.defaultDryRun
	Min = p.defaultMin
	Max = p.defaultMax
	SearchMethod = p.defaultSearchMethod
	LIPathKind = p.defaultLIPathKind
	Srcs = p.defaultSrcs
	ExcludeSrcs = p.defaultExcludeSrcs
	TriceAliases = p.defaultTriceAliases
	TriceSAliases = p.defaultTriceSAliases
	ExtendMacrosWithParamCount = p.defaultExtendMacrosWithParamCount
	DefaultTriceBitWidth = p.defaultDefaultTriceBitWidth
	DefaultStampSize = p.defaultDefaultStampSize
	StampSizeId = p.defaultStampSizeId
	TriceCacheEnabled = p.defaultTriceCacheEnabled
	SpaceInsideParenthesis = p.defaultSpaceInsideParenthesis
	SkipAdditionalChecks = p.defaultSkipAdditionalChecks
	IDRange = p.defaultIDRange
	Logging = p.defaultLogging
}
