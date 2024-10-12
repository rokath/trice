package id_test

import (
	"bytes"
	"os"
	"path/filepath"
	"testing"

	"github.com/rokath/trice/internal/id"
	"github.com/rokath/trice/pkg/msg"
	"github.com/spf13/afero"
	"github.com/tj/assert"
)

var (
	err    error
	b      bytes.Buffer             // args.Handler io.Writer outout
	fSys   *afero.Afero             // ram file system for the tests
	home   = "/home/tst/"           // home folder for id package tests
	proj   = "/my/project/"         // project folder
	cache  = home + ".trice/cache/" // cache folder
	sFname string                   // source file name in test (changes with each test)
	cCache string                   // cleaned cache file name in test (changes with each test)
	iCache string                   // inserted file name in test (changes with each test)

)

func init() {
	fSys = &afero.Afero{Fs: afero.NewMemMapFs()}
	msg.FatalOnErr(fSys.MkdirAll(proj, os.ModeDir)) // create project folder
}

func setupCacheTest(t *testing.T) func() {
	// Setup code here ///////////////////

	id.UserHomeDir = home                            // overwrite global variable for the id package tests
	msg.FatalOnErr(fSys.MkdirAll(cache, os.ModeDir)) // create cache (it could be deleted in some tests)

	// create til.json and li.json
	id.FnJSON = proj + t.Name() + "_til.json"
	assert.Nil(t, fSys.WriteFile(id.FnJSON, nil, 0777))
	id.LIFnJSON = proj + t.Name() + "_li.json"
	assert.Nil(t, fSys.WriteFile(id.LIFnJSON, nil, 0777))

	// set file names
	sFname = proj + t.Name() + "_file.c"
	cCache = filepath.Join(cache, "/cleaned", sFname)
	iCache = filepath.Join(cache, "/inserted", sFname)

	// tear down later //////////////////
	return func() {
		// tear-down code here
		assert.Nil(t, fSys.Remove(id.FnJSON))
		assert.Nil(t, fSys.Remove(id.LIFnJSON))
		assert.Nil(t, fSys.Remove(sFname))
		fSys.Remove(cCache) // Remove without error check, because iCache could not exist.
		fSys.Remove(iCache) // Remove without error check, because iCache could not exist.
		id.ResetGlobalVars(t)
	}
}
