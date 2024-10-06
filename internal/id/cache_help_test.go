package id_test

import (
	"bytes"
	"os"
	"path/filepath"
	"testing"
	"time"

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

//  // assertFileExists asserts, that fn does not exist
//  func assertFileExists(t *testing.T, fSys *afero.Afero, fn string) {
//  	_, err := fSys.Stat(fn)
//  	assert.Nil(t, err)
//  }

// assertFileNotExists asserts, that fn does not exist
func assertFileNotExists(t *testing.T, fSys *afero.Afero, fn string) {
	_, err := fSys.Stat(fn)
	assert.Error(t, err)
}

// assertMTime compares mtime with fName modification times to be equal.
func assertMTime(t *testing.T, fSys *afero.Afero, mtime time.Time, fName string) {
	stat, err := fSys.Stat(fName)
	assert.Nil(t, err)
	assert.True(t, mtime == stat.ModTime())
}

// assertNotMTime compares mtime with fName modification times to be different.
func assertNotMTime(t *testing.T, fSys *afero.Afero, mtime time.Time, fName string) {
	stat, err := fSys.Stat(fName)
	assert.Nil(t, err)
	assert.False(t, mtime == stat.ModTime())
}

// assertEqualMTimes comparesfn0 and fn1 modification times to be equal.
func assertEqualMTimes(t *testing.T, fSys *afero.Afero, fn0, fn1 string) {
	fn0Stat, err := fSys.Stat(fn0)
	assert.Nil(t, err)
	fn1Stat, err := fSys.Stat(fn1)
	assert.Nil(t, err)
	assert.True(t, fn0Stat.ModTime() == fn1Stat.ModTime())
}

// assertEqualMTimes comparesfn0 and fn1 modification times to be equal.
func assertNotEqualMTimes(t *testing.T, fSys *afero.Afero, fn0, fn1 string) {
	fn0Stat, err := fSys.Stat(fn0)
	assert.Nil(t, err)
	fn1Stat, err := fSys.Stat(fn1)
	assert.Nil(t, err)
	assert.False(t, fn0Stat.ModTime() == fn1Stat.ModTime())
}

func assertFileContent(t *testing.T, fSys *afero.Afero, fName, exp string) {
	act, e := fSys.ReadFile(fName)
	assert.Nil(t, e)
	assert.Equal(t, exp, string(act))
}

func mTime(t *testing.T, fSys *afero.Afero, fName string) time.Time {
	stat, err := fSys.Stat(fName)
	assert.Nil(t, err)
	return stat.ModTime()
}

func assertFileCreate(t *testing.T, fSys *afero.Afero, fName, src string) {
	assert.Nil(t, fSys.WriteFile(fName, []byte(src), 0777)) // create src file
}
